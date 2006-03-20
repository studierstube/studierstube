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
/** Header file for SoOpenTrackerSource node
  *
  * @author   Gerhard Reitmayr
  *
  * $Id: SoOpenTrackerSource.h 44 2005-07-18 15:03:44Z bara $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef __SOOPENTRACKERSOURCE_H__
#define __SOOPENTRACKERSOURCE_H__

#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include "event.h"
#include "SoEventSource.h"
#include "tools.h"

namespace ot {
class Context;
class State;
};

class EventModule;

class  SoOpenTrackerSource : public SoEventSource
{
    SO_NODE_HEADER( SoOpenTrackerSource );

public:
    SoOpenTrackerSource(void);
    static void initClass(void);

    /// name of the configuration file used
    SoSFString configuration;

    /// indicates if the tracker source is active or not
    SoSFBool active;

    enum PROCESSING { IDLE, TIME, POLL };
    /// enum field that defines how the tracking source operates
    SoSFEnum processing;

    /// interval length for TIME processing mode
    SoSFTime interval;

    /// signal by the underlying OpenTracker module that it should stop
    SoSFBool shouldStop;

    ot::Context * getContext(void)
    {
        return context;
    };

    void processEvent( const ot::State * state, const NameStringMap * attributes = NULL );

protected:
    virtual ~SoOpenTrackerSource();

    /// the underlying OpenTracker context
    ot::Context * context;

    /// interface to the OpenTracker graph
    EventModule * eventHandler;

    /// sensor for configuration field
    SoFieldSensor configSensor;

    /// callback for configuration field sensor
    static void configChanged( void * data, SoSensor * sensor);

    /// sensor for processing field
    SoFieldSensor processSensor;

    /// callback for processing field sensor
    static void processChanged( void * data, SoSensor * sensor);

    /// sensor triggering the operation of the tracking loop
    SoSensor * runSensor;

    /// processing callback used by various sensors
    static void runTrackerCB( void * data, SoSensor * sensor);

    /// run a single iteration of the event loop
    void runTracker(void);

    /// observe Events for the EventModule input handlers
    void handleEvent(SoHandleEventAction * action);
};
#endif //__SOOPENTRACKERSOURCE_H__
