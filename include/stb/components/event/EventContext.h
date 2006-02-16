#ifndef _EVENTCONTEXT_H_
#define _EVENTCONTEXT_H_

#include "event.h"
#include "tools.h"

class EVENT_API EventContext
{
protected:

	NameStringMap data;

    static SbString emptyString;

public:
	EventContext(void);
	virtual ~EventContext();

	const SbString & get(const SbName & name)
    {
        NameStringMap::iterator it = data.find( name );
        if(  it != data.end())
            return (*it).second;
        return EventContext::emptyString;
    }

    void set( const SbName & name, const SbString & string )
    {
        data[name] = string;
    }

    bool containsKey( const SbName & name )
    {
        return( data.find(name) != data.end());
    }

    const NameStringMap & getData(void) const
    {
        return data;
    }
};

#endif _EVENTCONTEXT_H_
