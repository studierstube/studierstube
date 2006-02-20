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
/** Source file for EventSink Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Id: EventSink.cpp 52 2006-02-02 20:14:26Z bara $
  * @file                                                                   */
 /* ======================================================================= */

// #include "../tool/disable4786.h"
#include <stb/components/event/EventSink.h>
#include <stb/components/event/EventModule.h>


#ifdef WIN32
#pragma warning(disable:4786)
#endif

EventSink::EventSink( EventModule * parent_ ) :
    parent( parent_ )
{
    // TODO: walk through the attributes and set them on the event as meta data
}

int EventSink::isEventGenerator()
{
    return 1;
}

void EventSink::onEventGenerated( ot::State& event, ot::Node& generator)
{
    parent->processEvent( &event, this );
    updateObservers( event );
}
