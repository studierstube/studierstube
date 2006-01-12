/* ========================================================================
* Copyright (C) 2005  Graz University of Technology
*
* This framework is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This framework is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this framework; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
* For further information please contact Dieter Schmalstieg under
* <schmalstieg@icg.tu-graz.ac.at> or write to Dieter Schmalstieg,
* Graz University of Technology, Inffeldgasse 16a, A8010 Graz,
* Austria.
* ========================================================================
* PROJECT: Studierstube
* ======================================================================== */
/** The source file for the OpenTrackerContext class.
*
* @author Denis Kalkofen
*
* $Id: OpenTrackerContext.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#include "OpenTrackerContext.h"
#include <ace/ACE.h>
#include <ace/Thread.h>
#include <ace/Mutex.h>
#include "StbEvent.h"
#include "StbModule.h"
#include <OpenTracker.h>


OpenTrackerContext* OpenTrackerContext::instance=NULL;
ACE_Mutex* OpenTrackerContext::updateLock=NULL;
bool OpenTrackerContext::stopOTracker=false;

OpenTrackerContext* 
OpenTrackerContext::getInstance()
{
	if(!OpenTrackerContext::instance)
		OpenTrackerContext::instance = (new OpenTrackerContext);
	
	return OpenTrackerContext::instance;
}


OpenTrackerContext::OpenTrackerContext()
{ 
	ACE::init();
	printf("STB_INFO: OpenTracker - context established.\n");
	otConfigFileName="";
	OpenTrackerContext::updateLock=new ACE_Mutex();
	//add event module
}

OpenTrackerContext::~OpenTrackerContext()
{
	ACE::fini();
	delete updateLock;
}

StbSink* 
OpenTrackerContext::getStbSink(std::string stbSinkName)
{
	return eventHandler->getStbSink(stbSinkName);
}

bool
OpenTrackerContext::stopOpenTracker()
{
	updateLock->acquire();
	OpenTrackerContext::stopOTracker=true;
    updateLock->release();

	updateLock->acquire();
    while (OpenTrackerContext::stopOTracker )
    {
		updateLock->release();
		updateLock->acquire();
    }  
	updateLock->release();
	
	return true;
}
bool
OpenTrackerContext::startOpenTracker(const char* otConfigName)
{	
	printf("STB_INFO: start OpenTracker\n");
	otConfigFileName=otConfigName;
	ACE_hthread_t* threadHandle = new ACE_hthread_t();
	if(ACE_Thread::spawn((ACE_THR_FUNC)OpenTrackerContext::otMainLoop,
							0, 	
							THR_NEW_LWP|THR_JOINABLE, 	
							0, 	
							threadHandle,
							-1, 	
							0, 	
							0
						)==-1)
	{ 
		printf("STB_ERROR: Error in spawning thread\n"); 
		return false;
	}


	return true;
}

void*
OpenTrackerContext::otMainLoop(void*)
{
	OpenTrackerContext* self=OpenTrackerContext::getInstance();

	ot::Context* otContext=new ot::Context(1);

	self->eventHandler = new StbModule();
	otContext->addModule( "StbConfig", *(self->eventHandler) );
    otContext->addFactory( *(self->eventHandler) );

	otContext->parseConfiguration(self->otConfigFileName);
	printf("STB_INFO: parsing opentracker's configuration file complete\n\n");
	printf("STB_INFO: starting opentracker's mainloop !\n\n");

	//otContext->run();
	otContext->start();
	updateLock->acquire();
	OpenTrackerContext::stopOTracker=false;
    while (!OpenTrackerContext::stopOTracker )
    {
		updateLock->release();

        otContext->pushStates();
        otContext->pullStates();

		updateLock->acquire();
    }  
	updateLock->release();
    otContext->close();   
	
	
	
	printf("STB_INFO: exiting OpenTracker \n");

	OpenTrackerContext::stopOTracker=false;
	return 0;
}
