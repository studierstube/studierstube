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
/** source file for SoInputEvent type
  *
  * @author   Gerhard Reitmayr
  *
  * $Id: SoInputEvent.cpp 52 2006-02-02 20:14:26Z bara $
  * @file                                                                   */
 /* ======================================================================= */

#include <map>
#include <boost/any.hpp>

#include <stb/components/event/SoInputEvent.h>

#include <stb/components/event/EventBus.h>
#include <stb/components/event/tools.h>


// #include "EventContext.h"

// hack, but cannot be avoided to decouple header from internal dependencies on boost
class EventData {
public:
    class lessName {
    public :
        bool operator()( const SbName & n1, const SbName & n2 ) const
        {
            return (n1.getString() < n2.getString());
        }
    };

    typedef std::map<SbName, boost::any, lessName> DataMap;
    
    DataMap data;

    boost::any & operator[](  const SbName & key )
    {
        return data[key];
    }

    template<typename ValueType> ValueType get( const SbName & key ) throw(SoInputEvent::ElementNotFound, SoInputEvent::ElementOfDifferentType)
    {
        DataMap::iterator it = data.find( key );
        if(  it == data.end()){
            printf("key %s not found\n",key.getString());
            throw SoInputEvent::ElementNotFound();
        }
        try {
            return boost::any_cast<ValueType>( (*it).second );
        }
        catch(const boost::bad_any_cast e)
        {
            throw SoInputEvent::ElementOfDifferentType();
        }
    }

    bool containsKey( const SbName & key ) const
    {
        return (data.find( key ) != data.end());
    }

    const bool isOfType( const SbName & key, const std::type_info & type ) const
    {
        if(containsKey( key ))
        {
            DataMap::const_iterator it = data.find( key );
            return ((*it).second.type() == type );
        }
        return false;
    }
};

SO_EVENT_SOURCE(SoInputEvent);

void SoInputEvent::initClass(void)
{
    if( SoType::fromName("SoInputEvent").isBad())
    {
        SO_EVENT_INIT_CLASS(SoInputEvent, SoEvent);
    }
}

SoInputEvent::SoInputEvent(/*EventContext * myContext,*/ EventProducer * prod)
{
//  context = myContext;
    data = new EventData();
    producer=prod;
}

SoInputEvent::~SoInputEvent()
{
    assert( data );
    delete data;
    data = NULL;
//  context = NULL;
    producer = NULL;
}

void SoInputEvent::set( const SbName & name, const SbVec3f & item )
{
    assert( data );
    (*data)[name] = item;
}

void SoInputEvent::set( const SbName & name, const SbRotation & item )
{
    assert( data );
    (*data)[name] = item;
}

void SoInputEvent::set( const SbName & name, const float item )
{
    assert( data );
    (*data)[name] = item;
}

void SoInputEvent::set( const SbName & name, const int32_t item )
{
    assert( data );
    (*data)[name] = item;
}

void SoInputEvent::set( const SbName & name, const SbTime & item )
{
    assert( data );
    (*data)[name] = item;
}

void SoInputEvent::set( const SbName & name, const SbString & item )
{
    assert( data );
    (*data)[name] = item;
}

const SbVec3f SoInputEvent::getSFVec3f( const SbName & name ) const throw(ElementNotFound, ElementOfDifferentType)
{
    assert( data );
    return data->get<SbVec3f>( name );
}

const SbString SoInputEvent::getSFString( const SbName & name ) const throw(ElementNotFound, ElementOfDifferentType)
{
    assert( data );
    return data->get<SbString>( name );
}

const SbRotation SoInputEvent::getSFRotation( const SbName & name ) const throw(ElementNotFound, ElementOfDifferentType)
{
    assert( data );
    return data->get<SbRotation>( name );
}

const float SoInputEvent::getSFFloat( const SbName & name ) const throw(ElementNotFound, ElementOfDifferentType)
{
    assert( data );
    return data->get<float>( name );
}

const SbBool SoInputEvent::getSFBool( const SbName & name ) const throw(ElementNotFound, ElementOfDifferentType)
{
    assert( data );
    return data->get<SbBool>( name );
}

const SbTime SoInputEvent::getSFTime( const SbName & name ) const throw(ElementNotFound, ElementOfDifferentType)
{
    assert( data );
    return data->get<SbTime>( name );
}

const SbString SoInputEvent::getString( const SbName & name ) const
{
    assert( data );
    SbString string;
    // long list of if statements to switch at runtime...
    if( data->isOfType(name, typeid(SbVec3f)))
    {
        // do something
        return string;
    }
    if( data->isOfType(name, typeid(SbRotation)))
    {
        // do something
        return string;
    }
    return "";
}

const SbBool SoInputEvent::containsKey( const SbName & name ) const
{
    assert( data );
    return data->containsKey( name );
}

const SbBool SoInputEvent::isOfType( const SbName & name, const std::type_info & type ) const
{
    assert( data );
    return data->isOfType(name, type);
}

/*
const SbString & SoInputEvent::getContext( const SbName & key ) const
{
    assert( context );
    return context->get(key);
}

const SbBool SoInputEvent::containsKeyContext( const SbName & key ) const
{
    assert( context );
    return context->containsKey(key);
}
*/

SoInputEvent & SoInputEvent::operator =( SoInputEvent & other )
{
    *((SoEvent *)this) = (SoEvent)other;
    *data = *other.data;
//  context = other.context;
    producer = other.producer;

    return *this;
}
