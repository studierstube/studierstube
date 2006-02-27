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
/** Header file for SoInputEvent type
  *
  * @author   Gerhard Reitmayr
  *
  * $Id: SoInputEvent.h 44 2005-07-18 15:03:44Z bara $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef __SOINPUTEVENT_H__
#define __SOINPUTEVENT_H__

#include <exception>
#include <vector>
#include <typeinfo>

#include <Inventor/Sb.h>
#include <Inventor/events/SoEvent.h>
#include <Inventor/events/SoSubEvent.h>

#include "event.h"

class EventData;
//class EventContext;
class EventProducer;

// TODO make a full POD
class EVENT_API SoInputEvent : public SoEvent
{
    SO_EVENT_HEADER();

public:
    
    class ElementNotFound : public std::exception
    {
    };

    class ElementOfDifferentType : public std::exception
    {
    };

    SoInputEvent(/*EventContext * myContext,*/ EventProducer * prod = NULL);
    virtual ~SoInputEvent();
    static void initClass(void);

    // possible types
    // all of these could be implemented inline and with template and overloading.
    // however, this would introduce dependencies on boost in this header file.

    // void set( const SbName & name, const SbBool item );
    void set( const SbName & name, const int32_t item );
    void set( const SbName & name, const float item );
    void set( const SbName & name, const double item );
    void set( const SbName & name, const SbTime & item );
    void set( const SbName & name, const SbVec2f & item );
    void set( const SbName & name, const SbVec3f & item );
    void set( const SbName & name, const SbVec4f & item );  
    void set( const SbName & name, const SbRotation & item );
    void set( const SbName & name, const SbMatrix & item );
    void set( const SbName & name, const SbString & item );
    void set( const SbName & name, const SbColor & item );

    // void set( const SbName & name, const std::vector<SbBool> & item );
    void set( const SbName & name, const std::vector<int32_t> & item );
    void set( const SbName & name, const std::vector<float> & item );
    void set( const SbName & name, const std::vector<double> & item );
    void set( const SbName & name, const std::vector<SbTime> & item );
    void set( const SbName & name, const std::vector<SbVec2f> & item );
    void set( const SbName & name, const std::vector<SbVec3f> & item );
    void set( const SbName & name, const std::vector<SbVec4f> & item ); 
    void set( const SbName & name, const std::vector<SbRotation> & item );
    void set( const SbName & name, const std::vector<SbMatrix> & item );
    void set( const SbName & name, const std::vector<SbString> & item );
    void set( const SbName & name, const std::vector<SbColor> & item );

    // need the complementary getXXX functions
    // all of these could be implemented inline and with template and overloading.
    // however, this would introduce dependencies on boost in this header file.

    const SbVec3f  getSFVec3f( const SbName & name ) const throw(ElementNotFound, ElementOfDifferentType);
    const SbRotation  getSFRotation( const SbName & name ) const throw(ElementNotFound, ElementOfDifferentType);
    const SbString  getSFString( const SbName & name ) const throw(ElementNotFound, ElementOfDifferentType);
    const float getSFFloat( const SbName & name ) const throw(ElementNotFound, ElementOfDifferentType);
    const SbBool getSFBool( const SbName & name ) const throw(ElementNotFound, ElementOfDifferentType);
    const SbTime  getSFTime( const SbName & name ) const throw(ElementNotFound, ElementOfDifferentType);

    const std::vector<SbVec3f> & getMFVec3f( const SbName & name ) const throw(ElementNotFound, ElementOfDifferentType);

    // provides an Open Inventor style representation of the entry with key name
    const SbString getString( const SbName & name ) const;

    const std::vector<SbString> & getKeys(void) const;

    const SbBool containsKey( const SbName & name ) const;

    const EventProducer * getProducer(void) const
    {
        return producer;
    }

    void setProducer( EventProducer * prod )
    {
        producer = prod;
    }

    const SbBool isOfType( const SbName & name, const std::type_info & type ) const;
/*
    const SbString & getContext( const SbName & key ) const;
    const SbBool containsKeyContext( const SbName & key ) const;

    const EventContext * getContext(void) const
    {
        return context;
    }
*/
    SoInputEvent & operator =( SoInputEvent & );

protected:
    EventData * data;
    // EventContext * context;
    EventProducer * producer;
};

#endif
