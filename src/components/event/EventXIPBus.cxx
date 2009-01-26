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
/** Source file for the EventProducer, EventSubscriber, EventFilter
  * and EventBus classes
  * 
  * @author Gerhard Reitmayr
  *
  * $Id: EventBus.cpp 52 2006-02-02 20:14:26Z bara $
  * @file                                                                   */
 /* ======================================================================= */

// #include "../tool/disable4786.h"
#include <algorithm>

#include <stb/components/event/EventXIPBus.h>
//#include <stb/kernel/Kernel.h>
//#include <stb/kernel/SceneManager.h>
//#include <stb/kernel/ComponentManager.h>
#include <stb/components/event/SoOpenTrackerSource.h>
#include <stb/components/event/EventContextFilter.h>
#include <stb/components/event/event.h>
#include <Inventor/fields/SoMFName.h>

#include <iostream>

EventXIPBus *EventXIPBus::singleton = NULL;

EventXIPBus & EventXIPBus::getSingleton(void)
{
  using namespace std;
  if(singleton==NULL)
    {
		printf("creating new Singleton\n");
		singleton = new EventXIPBus();
    }    
  return *singleton;
}

EventXIPBus::EventXIPBus(void)
{
}

EventXIPBus::~EventXIPBus(void)
{
}

void EventXIPBus::addSubscriber( EventXIPSubscriber * subscriber , EventXIPFilter * filter)
{
    std::map<EventXIPSubscriber *, EventXIPFilter *>::value_type temp(subscriber, filter);
    subscriptions.insert( temp );
}

void EventXIPBus::removeSubscriber( EventXIPSubscriber * subscriber )
{
    subscriptions.erase( subscriber );
}


class XIPSender
{
public:
    SoInputEvent * event;
    stb::Event_XIP * eventComponent;
    BasicEventFilter wildcardFilter;
    XIPSender(SoInputEvent * myEvent, stb::Event_XIP * myEventComponent) : event(myEvent), eventComponent(myEventComponent)
    {
        for( int i = 0; i < eventComponent->getKey()->getNum(); i++ )
            wildcardFilter.setPredicate( (*eventComponent->getKey())[i], (*eventComponent->getValue())[i]);
    };

    void operator()(const std::map<EventXIPSubscriber *, EventXIPFilter *>::value_type & pair)
    {

		/// Verifies that there is at least one filter
        if( pair.second )
        {    
            // Check if it is paused
            if (!eventComponent->isPaused())
            {
                // If is not continue normally
                /// Queries if the event matches the filter
                if( pair.second->accept( event )){
                    /// Notifies the subscriber about this new event
                    pair.first->newEvent( event );
                }
            }
            else
            {
                // If it is paused the check if the wildcard is the one being passed around
                if (wildcardFilter.accept(event))
                {
                    /// Queries if the event matches the filter
                    if( pair.second->accept( event )){
                        /// Notifies the subscriber about this new event
                        pair.first->newEvent( event );
                    }
                }
            }
        }
    }
};



void EventXIPBus::setEventInstance( stb::Event_XIP* eventInstance_ )
{
  eventInstance = eventInstance_;
}

void EventXIPBus::publish( SoInputEvent * event )
{
	// in XIP we set the Event Instance as single instance of XIPStbEvent
	//stb::Event* eventInstance=(stb::Event*)(stb::Kernel::getInstance()->getComponentManager()->load("Event"));

    XIPSender sender( event, eventInstance );
    /// Try to notify all the subscribers
    
    std::for_each( subscriptions.begin(), subscriptions.end(), sender );
}

