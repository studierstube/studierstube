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
  * ======================================================================== */

#ifndef _SOSPRING_H_
#define _SOSPRING_H_

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFTrigger.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <stb/components/starlight/SoMFVec2s.h>
#include <Inventor\sensors\SoTimerSensor.h>

#include "starlight.h"

/** 
@ingroup starlight

*/

class SpringMassPoint 
{
public:
    SbVec3f position, velocity, acceleration;
    float mass;
    float damp;

    SpringMassPoint() { }
    ~SpringMassPoint() {}
    void applyForce(SbVec3f force, float dt);
    void eulerIntegration(float dt);
};

class STARLIGHT_API SoSpring : public SoEngine 
{
   SO_ENGINE_HEADER(SoSpring);

 public:

     //SoMFVec2s links;
     SoSFVec3f v1;
     SoSFVec3f v2;
     SoSFVec3f force;
     SoSFFloat stiffness;
     SoSFFloat damp;
     SoSFFloat timeStep;
     SoSFTrigger trigger;

     SoEngineOutput v1out; 
     SoEngineOutput v2out; 
	
   static void initClass();

   SoSpring();

 private:

    void initializeSystem();
    void updateSystem();
    void calculateForces();
    void applyForces();
    void eulerIntegrations();
    static void timerFunc(void *data, SoSensor *);

    SoTimerSensor *timer;                                            // This is a physics model, so its based in time
    float dt;                                                       // Delta time

    // Backup of everything defined by the user, for internal usage
    SpringMassPoint internalPoints[2];
    float actualDistances;
    float restingDistances;
    float k;
    SbVec3f unitVectors;
    SbVec3f resultingForces;

    virtual ~SoSpring();

    virtual void inputChanged(SoField * whichField);
    virtual void evaluate();

};

#endif // _SOSPRING_H_
