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
/** The header file for the StbSink class.
*
* @author Denis Kalkofen
*
* $Id: StbSink.cpp 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#include "StbSink.h"
#include "StbModule.h"
#include <ace/Mutex.h>

#ifdef WIN32
#pragma warning(disable:4786)
#endif

StbSink::StbSink(const std::string & aName)  : 
		 ot::Node(),
		 stationName(aName)
{
	stateMutex= new ACE_Mutex();
	stateChanged=false;
}

StbSink::~StbSink()
{
	delete stateMutex;
}

int 
StbSink::isEventGenerator()
{
    return 1;
}

void 
StbSink::onEventGenerated( ot::State& event, ot::Node& generator)
{
	stateMutex->lock();
	stateChanged=true;
	state=event;
	stateMutex->release();

	updateObservers(event);
	
}

bool 
StbSink::getState(ot::State & retState)
{
	bool retVal=false;
	stateMutex->lock();
	
	if(stateChanged)
	{
		
		retVal=true;
		retState=state;
		stateChanged=false;
	}
	stateMutex->release();

	return retVal;
}