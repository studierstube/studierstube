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
/** Header file for the EventProducer, EventSubscriber, EventFilter
  * and EventBus classes
  * 
  * @author Gerhard Reitmayr
  *
  * $Id: EventBus.h 44 2005-07-18 15:03:44Z bara $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef __EVENTXIPBUS_H__
#define __EVENTXIPBUS_H__

#include <map>

#include "event_xip.h"

class SoInputEvent;
class EventContext;

class EVENT_API EventXIPProducer
{
};

/// Subscriber class, will get filtered updates directly from the publisher
class EVENT_API EventXIPSubscriber
{
public:
    virtual ~EventXIPSubscriber(){}
    virtual void newEvent( SoInputEvent * event ) = 0;
};

/// Filter class, in charge of accepting or rejecting events
class EVENT_API EventXIPFilter 
{
public: 
    virtual ~EventXIPFilter(){}
    virtual bool accept( SoInputEvent * event ) = 0;
};

class EVENT_API EventXIPBus
{
public:

	/// Adds a subscriber and its filter to our list
    void addSubscriber( EventXIPSubscriber * subscriber , EventXIPFilter * filter = NULL);

	/// Removes the subscriber from the list
    void removeSubscriber( EventXIPSubscriber * subscriber );

	/// Publish the events to the subscribers
    void publish( SoInputEvent * event );

	void setEventInstance( stb::Event_XIP* eventInstance_ );

	/// It's a singleton, this gets the proper instance
    static EventXIPBus & getSingleton(void);

protected:
	/// Constructor is protected
    EventXIPBus(void);

	/// The singleton instance
    static EventXIPBus *singleton;

	/// Destructor is protected
    virtual ~EventXIPBus(void);

	/// The list of subscribers and their filters
    std::map<EventXIPSubscriber *, EventXIPFilter *> subscriptions;

	stb::Event_XIP* eventInstance;
};

#endif //__EVENTBUS_H__
