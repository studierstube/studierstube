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
 * Institut for Computer Graphics and Vision,
 * Inffeldgasse 16a, 8010 Graz, Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/** Header file for the BasicEventFilter and EventAdapter classes
  * 
  * @author Gerhard Reitmayr
  *
  * $Id: EventContextFilter.h 44 2005-07-18 15:03:44Z bara $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef __EVENTCONTEXTFILTER_H__
#define __EVENTCONTEXTFILTER_H__

#include "event.h"
#include "tools.h"
#include "EventBus.h"

class EVENT_API BasicEventFilter :  public EventFilter
{
public:
    BasicEventFilter(void);
    virtual ~BasicEventFilter(void);

    virtual bool accept( SoInputEvent * event );
    
    void setPredicate( const SbName & name, const SbString & value );

protected:
    NameStringMap predicates;
};

template <typename T> class EventAdapter : public EventSubscriber
{
public:
    EventAdapter( T * myParent ) : parent(myParent)
    {
        int i;
        for( i = 0; i < parent->key.getNum(); i++ )
            filter.setPredicate( parent->key[i], parent->value[i]);
        EventBus::getSingleton().addSubscriber( this, &filter );
    }

    virtual ~EventAdapter()
    {
        EventBus::getSingleton().removeSubscriber( this );
        parent = NULL;
    }

    virtual void newEvent( SoInputEvent * event )
    {
        parent->processEvent( event );
    }

protected:
    T * parent;
    BasicEventFilter filter;
};
#endif
