/* ======================================================================== 
 * Copyright (C) 2006  Graz University of Technology
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
 * For further information please contact 
 * Dieter Schmalstieg
 * <schmalstieg@icg.tu-graz.ac.at>
 * Graz University of Technology, 
 * Institute for Computer Graphics and Vision,
 * Inffeldgasse 16a, 8010 Graz, Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/** 
  * 
  * @author Denis Kalkofen
  *
  * $Id: event.cpp 44 2005-07-18 15:03:44Z bara $
  * @file                                                                   */
 /* ======================================================================= */


#include <iostream>
#include <stb/components/event/event.h>
#include <stb/components/event/SoEventAction.h>
#include <stb/components/event/SoEventGroup.h>
#include <stb/components/event/SoEventKit.h>
#include <stb/components/event/SoEventNode.h>
#include <stb/components/event/SoEventRoot.h>
#include <stb/components/event/SoEventSeparator.h>
#include <stb/components/event/SoEventSource.h>
#include <stb/components/event/SoInputEvent.h>
#include <stb/components/event/SoOpenTrackerSource.h>
#include <stb/components/event/SoTrackedArtifactKit.h>
#include <stb/components/event/SoTrakEngine.h>
#include <stb/kernel/ComponentManager.h>
#include <stb/components/starlight/starlight.h>
#include <stb/kernel/Kernel.h>
#include <stb/kernel/ComponentManager.h>
#include <stb/kernel/SceneManager.h>

#ifdef HAVE_OPENVIDEO
#  include <openvideo/State.h>
#endif


#include <OpenTracker.h>
#include <core/Context.h>


// check if we actually have the correct OpenTracker version the users wants...
//
#ifdef USE_OT_1_1
#  if defined(OPENTRACKER_VERSION_MAJOR) && defined(OPENTRACKER_VERSION_MINOR)
#    if (OPENTRACKER_VERSION_MAJOR==1) && (OPENTRACKER_VERSION_MINOR==1)
#      pragma message(">>> Building against OpenTracker version 1.1 - version check OK")
#    else
#      pragma message ("ERROR: Wrong OpenTracker version. This is not OpenTracker 1.1")
#      error ("ERROR: Wrong OpenTracker version. This is not OpenTracker 1.1")
#    endif
#  else
#    pragma message ("ERROR: Should build against OpenTracker 1.1, but found unknown OpenTracker version. Please update to latest OpenTracker version")
#    error ("ERROR: Should build against OpenTracker 1.1, but found unknown OpenTracker version. Please update to latest OpenTracker version")
#  endif
#else
#  if defined(OPENTRACKER_VERSION_MAJOR) && defined(OPENTRACKER_VERSION_MINOR)
#    if (OPENTRACKER_VERSION_MAJOR==1) && (OPENTRACKER_VERSION_MINOR==2)
#      pragma message(">>> Building against OpenTracker version 1.2 - version check OK")
#    else
#      pragma message ("ERROR: Wrong OpenTracker version. This is not OpenTracker 1.2")
#      error ("ERROR: Wrong OpenTracker version. This is not OpenTracker 1.2")
#    endif
#  else
#    pragma message ("ERROR: Should build against OpenTracker 1.2, but found unknown OpenTracker version. Please update to latest OpenTracker version")
#    error ("ERROR: Should build against OpenTracker 1.2, but found unknown OpenTracker version. Please update to latest OpenTracker version")
#  endif
#endif


#ifdef HAVE_OPENVIDEO
#  pragma message(">>> Creating event system with video input support")
#else
#  pragma message(">>> Creating event system without video input support")
#endif


class Starlight;

CREATE_COMPONENT_FUNC(Event)

BEGIN_NAMESPACE_STB



Event::Event()
{
    configFile="";
	otSource = NULL;
}

Event::~Event()
{
    //nil
}


/// Called before the application is destructed.
bool 
Event::init()
{   
    //FIXME: insert log message as soon as the logger is done      
	logPrintS("Event Initialized\n");
    if(isInit)
        return isInit;
    isInit=true;
    //get viewer's parameter
    retrieveParameter();

    Starlight* starlight=(Starlight*)(stb::Kernel::getInstance()->getComponentManager()->load("Starlight"));
    if(!starlight)
    {
		// FIXME: Error Message should be displayed here. Mendez 20060315
        return false;
    }

    SoTrackedArtifactKit::initClass();
    SoTrakEngine::initClass();
    SoOpenTrackerSource::initClass();
    SoEventGroup::initClass();
    SoEventKit::initClass();
    SoEventNode::initClass();
    SoEventSeparator::initClass();
    SoEventSource::initClass();
    SoInputEvent::initClass();
    SoEventRoot::initClass();
    SoEventAction::initClass();


    otSource=new SoOpenTrackerSource;
    otSource->ref();
    otSource->configuration.setValue(stb::Kernel::getInstance()->getConfig(configFile).c_str());

	// FIXME: This looks hardcoded instead of retrieved from the config file. 
	// unless it is changed on runtime and this is only initialization. Mendez 20060315
    otSource->processing=SoOpenTrackerSource::TIME;
    //otSource->processing=SoOpenTrackerSource::POLL;
	//otSource->processing=SoOpenTrackerSource::IDLE;
    otSource->interval=SbTime(0.01f);

    stb::Kernel::getInstance()->getSceneManager()->setTrackerSource(otSource);

    return isInit;
}

void 
Event::setParameter(stb::string key, std::string value)
{
    if(key=="configFile")
    {
        configFile=value;
    }
    //else if()
    //{
    //}
}

/// Called before the application is destructed.
void 
Event::shutDown()
{

}

SoTrakEngine*
Event::createSoTrakEngine()
{
    return new SoTrakEngine();
}

SoTrackedArtifactKit* 
Event::createSoTrackedArtifactKit()
{
    return new SoTrackedArtifactKit();
}


SoOpenTrackerSource* 
Event::createSoOpenTrackerSource()
{
    return new SoOpenTrackerSource();
}

SoEventGroup* 
Event::createSoEventGroup()
{
    return new SoEventGroup();
}

SoEventKit* 
Event::createSoEventKit()
{
    return new SoEventKit();
}

SoEventNode* 
Event::createSoEventNode()
{
    return new SoEventNode();
}

SoEventSeparator* 
Event::createSoEventSeparator()
{
    return new SoEventSeparator();
}

SoEventSource* 
Event::createSoEventSource()
{
    return new SoEventSource();
}

SoInputEvent* 
Event::createSoInputEvent()
{
    return new SoInputEvent();
}

SoEventRoot* 
Event::createSoEventRoot()
{
    return new SoEventRoot();
}

SoEventAction* 
Event::createSoEventAction()
{
    return new SoEventAction();
}


void
Event::vu_init(const openvideo::Buffer& frame)
{
}


void
Event::vu_update(const openvideo::Buffer& frame)
{
#ifdef HAVE_OPENVIDEO
	assert(otSource);

	if(ot::Context* context = otSource->getContext())
	{
		static unsigned int updateCtr = 0;

		// only update if this is a fresh video image!
		//
		if(frame.getUpdateCounter() != updateCtr)
		{
			// OpenTracker's pixel-format was carefully chosen to be compatible to OpenVideo's.
			// Since the Stb4 pixel-format is compatible to OpenVideo's pixel-format too, we can
			// simple cast here...
			//
			context->newVideoFrame(frame.getPixels(), frame.getWidth(), frame.getHeight(), static_cast<ot::PIXEL_FORMAT>(frame.getFormat()));
			updateCtr = frame.getUpdateCounter();
		}
	}
#endif
}



END_NAMESPACE_STB

//========================================================================
// End of Event.cxx
//========================================================================
// Local Variables:
// mode: c++
// c-basic-offset: 4
// eval: (c-set-offset 'substatement-open 0)
// eval: (c-set-offset 'case-label '+)
// eval: (c-set-offset 'statement 'c-lineup-runin-statements)
// eval: (setq indent-tabs-mode nil)
// End:
//========================================================================

