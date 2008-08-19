//
// C++ Interface: SoTimeOutSwitch
//
// Description:
//
//
// Author: sareika
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef _SOTIMEOUTSWITCH_H
#define _SOTIMEOUTSWITCH_H

#include <Inventor/engines/SoNodeEngine.h>
#include <Inventor/engines/SoSubNodeEngine.h>
#include <Inventor/engines/SoEngineOutput.h>
#include <Inventor/fields/SoSFTrigger.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/sensors/SoAlarmSensor.h>

#include "starlight.h"


class STARLIGHT_API SoTimeOutSwitch : public SoNodeEngine
{
    SO_NODEENGINE_HEADER(SoTimeOutSwitch);

public:
    SoTimeOutSwitch();
    static void initClass(void);

    SoSFTrigger trigger;
    SoSFString  on;
    SoSFString  off;
    SoSFTime    timeout;

    SoEngineOutput out;  // SoSFString

protected:
    virtual void inputChanged(SoField * which);
    virtual void evaluate(void);

    SoAlarmSensor timeOutSensor;
    static void timeOutFired( void * data, SoSensor * sensor );
    bool switchOn;
};

#endif // _SOTIMEOUTSWITCH_H
