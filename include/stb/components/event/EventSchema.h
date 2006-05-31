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
/** Header file for the EventSchema class
  *
  * @author Gerhard Reitmayr
  *
  * $Id: EventSchema.h 44 2005-07-18 15:03:44Z bara $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef __EVENTSCHEMA_H__
#define __EVENTSCHEMA_H__

#include "event.h"
#include "SoInputEvent.h"

/** a purely inlined helper class to provide schema correct access to an SoInputEvent instance
 * we will use a standard well-known schema to describe event slots and types. To avoid confusion
 * this class encodes the used schema names and transforms it into something understandable by 
 * the IDE and the compilers... */
class EVENT_API EventSchema {

public:
    EventSchema( SoInputEvent & ev ) : event(ev)
    {
    };

    EventSchema( const EventSchema & sh ) : event( sh.event )
    {
    };

	/// Transforms the value of the "position" key to SbVec3f and returns it
	const SbVec3f position(void)
    {
        return event.getSFVec3f("event.position");
    };

	/// Sets the value of the "position" key
    void position(const SbVec3f & vec)
    {
        event.set("event.position", vec);
    };

#ifdef USE_OT_1_1
#else
        /// Sets the value of the "position" key (for OpenTracker-1.2)
    void position(const std::vector<float> & vec)
    {
        position(SbVec3f(&vec[0]));
    }
#endif
	/// Transforms the value of the "orientation" key to SbRotation and returns it
    const SbRotation orientation(void)
    {
        return event.getSFRotation("event.orientation");
    };

	/// Sets the value of the "orientation" key
    void orientation(const SbRotation & rot)
    {
        event.set("event.orientation", rot);
    };
#ifdef USE_OT_1_1
#else
	/// Sets the value of the "orientation" key (for OpenTracker-1.2)
    void orientation(const std::vector<float> & rot)
    {
        event.set("event.orientation", SbRotation(&rot[0]));
    };
#endif
	/// Transforms the value of the "confidence" key to float and returns it
    float confidence(void)
    {
        return event.getSFFloat("event.confidence");
    };

	/// Sets the value of the "confidence" key
    void confidence(float conf)
    {
        event.set("event.confidence", conf);
    };

	/// Transforms the value of the "time" key to const SbTime and returns it
	const SbTime time(void)
	{
		return event.getSFTime("event.time");
	};

	/// Sets the value of the "time" key
    void time( const SbTime & time)
    {
        event.set("event.time", time );
    };

	/// Transforms the value of the "button."+number key to SbBool and returns it
    SbBool button( int32_t number )
    {
        if( number >= 0 && number <= 16)
        {
            SbString index(number);
            SbString prefix("event.button.");
            prefix += index;
            return event.getSFBool(prefix.getString());
        }
        return false;
    };

	/// Sets the value of the "button."+number key
    void button( int32_t number, SbBool value )
    {
        if( number >= 0 && number <= 16 )
        {
            SbString index(number);
            SbString prefix("event.button.");
            prefix += index;
            event.set(prefix.getString(), value);
        }
    };

    /// and so on for all other types as well

protected:
    SoInputEvent & event;
};

#endif
