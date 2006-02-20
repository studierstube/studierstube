/* ========================================================================
 * Copyright (C) 2000-2003  Vienna University of Technology
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
/** header file for SoEventAction node
  *
  * @author   Gerhard Reitmayr
  *
  * $Id: SoOpenTrackerSource.cpp 52 2006-02-02 20:14:26Z bara $
  * @file                                                                   */
 /* ======================================================================= */

#include <algorithm>

#include <Inventor/sensors/SoIdleSensor.h>
#include <Inventor/sensors/SoTimerSensor.h>

#include <OpenTracker.h>

#include <stb/components/event/SoOpenTrackerSource.h>
#include <stb/components/event/EventModule.h>
#include <stb/components/event/EventSchema.h>


SO_NODE_SOURCE(SoOpenTrackerSource);

void SoOpenTrackerSource::initClass(void)
{
    if( SoType::fromName("SoOpenTrackerSource").isBad() )
    {
        SoEventSource::initClass();
        SO_NODE_INIT_CLASS(SoOpenTrackerSource, SoEventSource, "SoEventSource");
    }
}

SoOpenTrackerSource::SoOpenTrackerSource(void) :
    context(NULL),
    eventHandler(NULL),
    runSensor(NULL)
{
    SO_NODE_CONSTRUCTOR(SoOpenTrackerSource);

    SO_NODE_DEFINE_ENUM_VALUE(PROCESSING, IDLE);
    SO_NODE_DEFINE_ENUM_VALUE(PROCESSING, TIME);
    SO_NODE_DEFINE_ENUM_VALUE(PROCESSING, POLL);

    SO_NODE_SET_SF_ENUM_TYPE( processing, PROCESSING );

    SO_NODE_ADD_FIELD( configuration, (""));
    SO_NODE_ADD_FIELD( processing, (TIME));
    SO_NODE_ADD_FIELD( interval, (0.01));
    SO_NODE_ADD_FIELD( shouldStop, (FALSE));

    configSensor.setFunction( SoOpenTrackerSource::configChanged );
    configSensor.setData( this );
    configSensor.attach( &configuration );

    processSensor.setFunction( SoOpenTrackerSource::processChanged );
    processSensor.setData( this );
    processSensor.attach( &processing );

    // setup the timer sensor for running the show
    SoOpenTrackerSource::processChanged( this, NULL );
}

SoOpenTrackerSource::~SoOpenTrackerSource()
{
    if(context)
    {
        context->close();
        delete context;
        active.setValue(FALSE);
        context = NULL;
        eventHandler = NULL;
    }
    if(runSensor)
    {
        runSensor->unschedule();
        delete runSensor;
        runSensor = NULL;
    }
}

void SoOpenTrackerSource::configChanged( void * data, SoSensor * sensor)
{
    assert(data);
    SoOpenTrackerSource * self = (SoOpenTrackerSource *)data;

    if(self->context)
    {
        self->context->close();
        delete self->context;
        self->context = NULL;
        self->eventHandler = NULL;
        self->active.setValue(FALSE);
        self->shouldStop.setValue(FALSE);
    }
    SbString configFile = self->configuration.getValue();
    if(configFile.getLength() > 0)
    {
        self->context = new ot::Context(1);
        self->eventHandler = new EventModule(self);
        self->context->addModule( "EventConfig", *self->eventHandler );
        self->context->addFactory( *self->eventHandler );
        self->context->parseConfiguration( configFile.getString() );
        self->context->start();
        self->active.setValue(TRUE);
    }
}

void SoOpenTrackerSource::processChanged( void * data, SoSensor * sensor )
{
    assert(data);
    SoOpenTrackerSource * self = (SoOpenTrackerSource *)data;

    if(self->runSensor)
    {
        self->runSensor->unschedule();
        delete self->runSensor;
        self->runSensor = NULL;
    }
    switch(self->processing.getValue()){
        case SoOpenTrackerSource::IDLE: 
            {
                SoIdleSensor * idle = new SoIdleSensor( SoOpenTrackerSource::runTrackerCB, self);
                idle->schedule();
                self->runSensor = idle;
            }
            break;
        case SoOpenTrackerSource::TIME:
            {
                SoTimerSensor * timer = new SoTimerSensor( SoOpenTrackerSource::runTrackerCB, self );
                timer->setInterval( self->interval.getValue());
                timer->schedule();
                self->runSensor = timer;
            }
            break;
        case SoOpenTrackerSource::POLL:
            // do nothing here, because the tracker source is polled by the application
            break;
    }
}

void SoOpenTrackerSource::runTrackerCB( void * data, SoSensor * sensor )
{
    assert(data);
    ((SoOpenTrackerSource *)data)->runTracker();
}

void SoOpenTrackerSource::runTracker( void )
{
    if(active.getValue())
    {
        //assert(context);
        if (context) {
            context->pushStates();
            context->pullStates();
            if(context->stop() && !shouldStop.getValue()) shouldStop.setValue(TRUE);
         }
    }
}

void SoOpenTrackerSource::handleEvent(SoHandleEventAction * action)
{
    assert(eventHandler);
    eventHandler->handleEvent(action);
}

struct Setter {
public:
    SoInputEvent & event;
    Setter( SoInputEvent & ev_ ) : event( ev_ ) {};
    void operator()(const std::pair<const SbName, SbString> & pair)
    {
        event.set(pair.first, pair.second);
    };
};

void SoOpenTrackerSource::processEvent( const ot::State * state, const NameStringMap * attributes )
{
    SoInputEvent event(this);

    // copy the state to the event using the EventSchema
    EventSchema schema(event);
    schema.position( state->position );
    schema.orientation( state->orientation );
    schema.confidence( state->confidence );
    schema.time( state->time / 1000.0 );
    for( unsigned int i = 0, j = 1; i < 16; i++, j *= 2 )
        schema.button( i, !!(state->button & j) );

    // copy attributes, if present
    if( attributes )
        std::for_each(attributes->begin(), attributes->end(), Setter(event));

    // publish
    publish( &event );
}
