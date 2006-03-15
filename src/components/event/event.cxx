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

class Starlight;

CREATE_COMPONENT_FUNC(Event)

BEGIN_NAMESPACE_STB

Event::Event()
{
    configFile="";
}

Event::~Event()
{
    //nil
}

/// Called before the application is destructed.
bool 
Event::init()
{    
    stb::Kernel::getInstance()->log("Event::init\n");
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



    SoOpenTrackerSource *otSource=new SoOpenTrackerSource;
    otSource->ref();
    otSource->configuration.setValue(stb::Kernel::getInstance()->findConfigFile(configFile).c_str());

	// FIXME: This looks hardcoded instead of retrieved from the config file. 
	// unless it is changed on runtime and this is only initialization. Mendez 20060315
    otSource->processing=SoOpenTrackerSource::TIME;
    //otSource->processing=SoOpenTrackerSource::POLL;
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

