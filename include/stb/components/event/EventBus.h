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

#ifndef __EVENTBUS_H__
#define __EVENTBUS_H__

#include <map>

#include <stb/components/event/event.h>

class SoInputEvent;
class EventContext;

class EVENT_API EventProducer
{
};

/// listener interface class
class EVENT_API EventSubscriber
{
public:
    virtual void newEvent( SoInputEvent * event ) = 0;
};

class EVENT_API EventFilter 
{
public: 
    virtual bool accept( SoInputEvent * event ) = 0;
};

class EVENT_API EventBus
{
public:

    void addSubscriber( EventSubscriber * subscriber , EventFilter * filter = NULL);
    void removeSubscriber( EventSubscriber * subscriber );

    void publish( SoInputEvent * event );

    static EventBus & getSingleton(void);

protected:
    EventBus(void);
    static EventBus *singleton;
    virtual ~EventBus(void);

    std::map<EventSubscriber *, EventFilter *> subscriptions;
};

#endif
