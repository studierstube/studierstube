/* ========================================================================
 * Copyright (C) 2000-2005  Vienna University of Technology
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
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
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

#include <stb/components/event/EventBus.h>


EventBus *EventBus::singleton = 0;

EventBus & EventBus::getSingleton(void)
{
	if(!singleton)
		singleton = new EventBus();
    return *singleton;
}

EventBus::EventBus(void)
{
}

EventBus::~EventBus(void)
{
}

void EventBus::addSubscriber( EventSubscriber * subscriber , EventFilter * filter)
{
    std::map<EventSubscriber *, EventFilter *>::value_type temp(subscriber, filter);
    subscriptions.insert( temp );
}

void EventBus::removeSubscriber( EventSubscriber * subscriber )
{
    subscriptions.erase( subscriber );
}

class Sender
{
public:
    SoInputEvent * event;
    Sender(SoInputEvent * myEvent) : event(myEvent)
    {};

    void operator()(const std::map<EventSubscriber *, EventFilter *>::value_type & pair)
    {
        if( pair.second ){
            if( pair.second->accept( event )){
                pair.first->newEvent( event );
            }
        }
    }
};

void EventBus::publish( SoInputEvent * event )
{
    Sender sender( event );
    std::for_each( subscriptions.begin(), subscriptions.end(), sender );
}

