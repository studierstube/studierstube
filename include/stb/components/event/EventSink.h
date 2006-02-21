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
/** Header file for EventSink Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Id: EventSink.h 44 2005-07-18 15:03:44Z bara $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef __EVENTSINK_H__
#define __EVENTSINK_H__

#include <OpenTracker/OpenTracker.h>

#include <stb/components/event/SoInputEvent.h>
#include <stb/components/event/event.h>
#include <stb/components/event/tools.h>

class EventModule;

/**
 * This class implements a simple node that stores a copy of the last
 * event it received and passed on for output to Studierstube. The TrackerOT
 * class copies the data from this node into Studierstube data structures.
 * The corresponding OpenTracker configuration element  has the following attributes :
 * @li @c station integer starting from 0 giving the station number to use in Stb
 * @li @c event on|off whether this station should emit events or not, default is off
 *
 * An example element looks like this :
 * @verbatim
<EventSink attr1="value1" attr2="value2" >
    <Any EventGenerator element type>
</EventSink>@endverbatim
 *
 * @author Gerhard Reitmayr
 * @ingroup tracking
 */
class EVENT_API EventSink : public ot::Node
{

public:
    /// association with parent to pass the information on
    EventModule * parent;

// Methods
public:
    /** constructor sets the number of the station
     * @param parent_ the parent TrackerOT object 
     * @param number the station number we output to
     * @param events_ flag whether 3D events are enabled or not 
     */
    EventSink( EventModule * parent_ );

    /** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    virtual int isEventGenerator();
    
    /**
     * this method notifies the object that a new event was generated.
     * It stores a copy of the received event and passes the event on
     * to its observers.
     * @param event reference to the new event. Do not change the
     *        event values, make a copy and change that !
     * @param generator reference to the EventGenerator object that
     *        notified the EventObserver.
     */
    virtual void onEventGenerated( ot::State& event, ot::Node& generator);

    NameStringMap attributes;
};

#endif
