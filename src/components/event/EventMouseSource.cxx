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
/** Source file for EventMouseSource class.
*
* @author Gerhard Reitmayr
*
* $Id: EventMouseSource.cpp 44 2005-07-18 15:03:44Z bara $
* @file                                                                   */
/* ======================================================================= */

// #include "../tool/disable4786.h"
#include <Inventor/SbVec2f.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/events/SoMouseButtonEvent.h>
#include <stb/components/event/EventMouseSource.h>


EventMouseSource::EventMouseSource( Mode mode_, int window_ ) :
    ot::Node(),
    mode( mode_ ),
    window( window_ ),
    changed( false )
{
    state.confidence = 1;
}

void EventMouseSource::processEvent( const SoEvent & event, const SbViewportRegion & region )
{
    SbVec2f pos;
    if( mode == StbAbsolute )
    {           
        pos[0] = event.getPosition(region)[0];
        pos[1] = (float)(region.getWindowSize()[1] - event.getPosition(region)[1]);
    } else 
    {            
        pos = event.getNormalizedPosition(region);
        pos[1] = 1 - pos[1];
    }
    if( state.position[0] != pos[0] ||
        state.position[1] != pos[1] )
    {
        state.position[0] = pos[0];
        state.position[1] = pos[1];
        // FIXME: event->getTime() is about 300 ms behind system time in Coin3D!
        // Therefore we set the time of the event to the current system time.
        // This is done for all implementations to guarantee identical behaviour.
        // It was also used in the rest of this method.
        // gerhard [8/26/2003]
        // state.time = event->getTime().getValue()*1000;
        state.timeStamp();  
        changed = true;
    }
    if( event.getTypeId() == SoMouseButtonEvent::getClassTypeId())
    {
        if( SoMouseButtonEvent::isButtonPressEvent(&event,SoMouseButtonEvent::BUTTON1))
        {
            state.button |= 1;
            state.timeStamp();
            changed = true;
        }
        else if( SoMouseButtonEvent::isButtonReleaseEvent(&event,SoMouseButtonEvent::BUTTON1))
        {
            state.button &= 0xfffe;
            state.timeStamp();
            changed = true;
        }
        else if( SoMouseButtonEvent::isButtonPressEvent(&event,SoMouseButtonEvent::BUTTON2))
        {
            state.button |= 2;
            state.timeStamp();
            changed = true;
        }
        else if( SoMouseButtonEvent::isButtonReleaseEvent(&event,SoMouseButtonEvent::BUTTON2))
        {
            state.button &= 0xfffd;
            state.timeStamp();
            changed = true;
        }
        else if( SoMouseButtonEvent::isButtonPressEvent(&event,SoMouseButtonEvent::BUTTON3))
        {
            state.button |= 1;
            state.timeStamp();
            changed = true;
        }
        else if( SoMouseButtonEvent::isButtonReleaseEvent(&event,SoMouseButtonEvent::BUTTON3))
        {
            state.button &= 0xfffb;
            state.timeStamp();
            changed = true;
        }

#ifdef __COIN__
        // if Button 4 or 5 are pressed (usually mousewheel interaction)
        // modify the Z-Axis accordingly.
        // mousewheel rotated down - 
        else if( SoMouseButtonEvent::isButtonPressEvent(&event,SoMouseButtonEvent::BUTTON4))
        {
            state.position[2] -= 0.1f;
            state.timeStamp();
            changed = true;
        }
        // mousewheel rotated up
        else if( SoMouseButtonEvent::isButtonPressEvent(&event,SoMouseButtonEvent::BUTTON5))
        {
            state.position[2] += 0.1f;
            state.timeStamp();
            changed = true;
        }
#endif
    }
}
