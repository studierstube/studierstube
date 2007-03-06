 /* ========================================================================
  * Copyright (C) 2000,2001  Vienna University of Technology
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 2.1 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this library; if not, write to the Free Software
  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  * For further information please contact Dieter Schmalstieg under
  * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ======================================================================== 
  * PROJECT: Studierstube
  * ======================================================================== */ 

#include <stb/base/OS.h>

#ifdef STB_IS_WINDOWS
#  include <windows.h>
#  include <math.h>
#endif

#include <stb/components/starlight/SoSpring.h>


// *********************** SpringMassPoint **************************

void SpringMassPoint::applyForce(SbVec3f force, float dt)
{
    // Integrate velocity
    acceleration=force*(1.0f/mass);
    velocity=velocity+(acceleration*dt);
    velocity*=damp;
}

void SpringMassPoint::eulerIntegration(float dt)
{
    // Integrate position
    position=position+(velocity*dt);
}

// *********************** SpringMassPoint **************************


SO_ENGINE_SOURCE(SoSpring);

void
SoSpring::initClass()
{
    SO_ENGINE_INIT_CLASS(SoSpring, SoEngine, "Engine");
}

SoSpring::SoSpring()
{
    SO_ENGINE_CONSTRUCTOR(SoSpring);

    SO_ENGINE_ADD_INPUT(v1,(0,0,0));
    SO_ENGINE_ADD_INPUT(v2,(0,0,0));
    SO_ENGINE_ADD_INPUT(force,(0,0,0));
    SO_ENGINE_ADD_INPUT(stiffness,(1.0f));
    SO_ENGINE_ADD_INPUT(damp,(1.0f));
    SO_ENGINE_ADD_INPUT(timeStep,(1.0f));
    SO_ENGINE_ADD_INPUT(trigger,());

    SO_ENGINE_ADD_OUTPUT(v1out, SoSFVec3f);
    SO_ENGINE_ADD_OUTPUT(v2out, SoSFVec3f);


    //links.set1Value(SbVec2s(0,1));

    // This timer will be used to update the system once it is been triggered
    timer=new SoTimerSensor(timerFunc,this);
}

SoSpring::~SoSpring()
{
}

void SoSpring::timerFunc(void *data, SoSensor *)
{
    SoSpring *self=(SoSpring *) data;
    self->updateSystem();
    self->evaluate();
}

void SoSpring::initializeSystem()
{
    // initialize the system
    // Set the initial positions
    internalPoints[0].velocity.setValue( 0.0f,0.0f,0.0f);					            // the initial velocity
    internalPoints[0].position=v1.getValue();		    			                    // the initial position
    internalPoints[0].damp=damp.getValue();                                             // initialize coefficient of restitution
    internalPoints[0].mass=1.0;                                                         // the mass of the object

    internalPoints[1].velocity.setValue( 0.0f,0.0f,0.0f);
    internalPoints[1].position=v2.getValue()+force.getValue();                          // It does not start from resting position, but already with the force applied
    internalPoints[1].damp=damp.getValue();
    internalPoints[1].mass=1.0;

    // Distances and other necessary terms
    restingDistances=((SbVec3f) v2.getValue()-v1.getValue()).length();                  // Resting distance of the spring

    // I think there is a serious bug in the unitVector calculation
    SbVec3f tmpActual=internalPoints[1].position-internalPoints[0].position;
    SbVec3f tmpRest=v2.getValue()-v1.getValue();
    unitVectors=tmpRest-tmpActual;
    unitVectors.normalize();

    k=stiffness.getValue();                                                             // Stiffness
    dt=timeStep.getValue();                                                             // the delta time to use
}

void SoSpring::calculateForces()
{
    // Calculate the forces according to Hooke's Law
    float tmpFloat;

    actualDistances=((SbVec3f)internalPoints[1].position-internalPoints[0].position).length();                           // Distance just after first force was applied

    // k * (DISTij-LENij) * Vij
    tmpFloat=-k*(actualDistances-restingDistances);
    resultingForces=tmpFloat*unitVectors;
}

void SoSpring::applyForces()
{
    // Apply Forces
    internalPoints[1].applyForce(resultingForces,dt);

    // If velocity is below a certain threshold just stop everything to avoid infinite oscillation
    // Here I set the threshold to be based on a percentage of the resting distance
    float threshold=restingDistances/10.0f;
    if ((internalPoints[1].velocity.length()<threshold)&(internalPoints[1].acceleration.length()<threshold))
    {
        internalPoints[1].position=v2.getValue();
        timer->unschedule();
    }
}

void SoSpring::updateSystem()
{
    eulerIntegrations();
    calculateForces();
    applyForces();
}

void SoSpring::eulerIntegrations()
{
    // Apply the euler integration given the delta time
    internalPoints[1].eulerIntegration(dt);
}

void SoSpring::inputChanged(SoField * whichField)
{
    if (whichField == &trigger)
    {
        // Start the thing!
        initializeSystem();
        updateSystem();
        timer->setInterval(dt);
        timer->schedule();
        evaluate();
    }
}

void SoSpring::evaluate()
{
    SO_ENGINE_OUTPUT(v2out, SoSFVec3f, setValue(internalPoints[1].position));
}
