//
// C++ Implementation: %{MODULE}
//
// Description:
//
//
// Author: sareika
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <iostream>

#include <stb/components/starlight/SoTimeOutSwitch.h>

using namespace std;

// #define TIMEOUTDEBUG

SO_NODEENGINE_SOURCE(SoTimeOutSwitch);

SoTimeOutSwitch::SoTimeOutSwitch()
{
    SO_NODEENGINE_CONSTRUCTOR(SoTimeOutSwitch);
    SO_NODE_ADD_FIELD(trigger, ());
    SO_NODE_ADD_FIELD(on, ("TRUE"));
    SO_NODE_ADD_FIELD(off, ("FALSE"));
    SO_NODE_ADD_FIELD(timeout, (1.0));

    SO_NODEENGINE_ADD_OUTPUT(out, SoSFString);

    timeOutSensor.setFunction(SoTimeOutSwitch::timeOutFired);
    timeOutSensor.setData(this);
    inputChanged(&trigger);
    switchOn=false;
}

void SoTimeOutSwitch::initClass(void)
{
    if( SoType::fromName("SoTimeOutSwitch").isBad())
    {
        SO_NODEENGINE_INIT_CLASS(SoTimeOutSwitch, SoNodeEngine, "NodeEngine");
    }
}

void SoTimeOutSwitch::inputChanged( SoField * which )
{
    if( which == &trigger )
    {
        trigger.getValue();
        if(timeOutSensor.isScheduled())
            timeOutSensor.unschedule();
        
        timeOutSensor.setTimeFromNow(timeout.getValue());
        timeOutSensor.schedule();
         
#ifdef TIMEOUTDEBUG
        logPrintD("trigger fired %i \n", switchOn);
#endif
    }
}

void SoTimeOutSwitch::evaluate(void)
{
#ifdef TIMEOUTDEBUG
    logPrintD("evaluate with %i and %i\n", timeOutSensor.isScheduled(),  switchOn);
#endif
    
    SbString temp;
    if( timeOutSensor.isScheduled())
        temp = on.getValue();
    else
        temp = off.getValue();
    SO_ENGINE_OUTPUT(out, SoSFString, setValue(temp));
}

void SoTimeOutSwitch::timeOutFired( void * data, SoSensor * sensor )
{
    assert( data != NULL );
    SoTimeOutSwitch * self = (SoTimeOutSwitch *) data;

    self->touch();

#ifdef TIMEOUTDEBUG
    logPrintD("timer fired\n");
#endif
}
