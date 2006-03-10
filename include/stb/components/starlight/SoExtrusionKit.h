/* ========================================================================
* Copyright (c) 2006,
* Institute for Computer Graphics and Vision
* Graz University of Technology
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are
* met:
*
* Redistributions of source code must retain the above copyright notice,
* this list of conditions and the following disclaimer.
*
* Redistributions in binary form must reproduce the above copyright
* notice, this list of conditions and the following disclaimer in the
* documentation and/or other materials provided with the distribution.
*
* Neither the name of the Graz University of Technology nor the names of
* its contributors may be used to endorse or promote products derived from
* this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
* IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
* TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
* OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
* PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
* ========================================================================
* PROJECT: OpenTracker
* ======================================================================== */
/** Class definition for the SoExtrusionKit
*
*
* @author Erick Mendez
*
* $Id: $
* @file                                                                   */
/* ======================================================================= */

#ifndef _SOEXTRUSIONKIT_H_
#define _SOEXTRUSIONKIT_H_

/*
 * --------------------------------------------------------------------------------
 * Includes
 * --------------------------------------------------------------------------------
 */
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/fields/SoMFVec3f.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include "starlight.h"

/** 
* Header for an Open Inventor Class that implements an extrusion 
* Kit given a set of points and a direction vector.
*
* Notes: The set of coordinates must be a closed loop, this is, the last
*	     element must be the same as the first
*
* @author Erick Mendez
* @ingroup vidente
*/

class STARLIGHT_API SoExtrusionKit: public SoBaseKit
{
    SO_KIT_HEADER(SoExtrusionKit);
	typedef SoBaseKit inherited;

public:
	 /// Initializes the node kit
    static void initClass();

	/// The constructor of the class, initializes the catalog
    SoExtrusionKit();

	 /// Destructor, deletes the sensors
    ~SoExtrusionKit();

	/// The input vertices that will be extruded
	SoMFVec3f vertices;

	/// The extrusion vector, giving the direction and magnitude of extrusion
	SoSFVec3f extrusionVector;

protected:

	/// Sensors
	SoFieldSensor *verticesSensor;
	SoFieldSensor *extrusionVectorSensor;

	/// Parts of the catalog
	SO_KIT_CATALOG_ENTRY_HEADER(topSeparator);
	SO_KIT_CATALOG_ENTRY_HEADER(coords);
	SO_KIT_CATALOG_ENTRY_HEADER(faces);
	SO_KIT_CATALOG_ENTRY_HEADER(shapeHints);

	/// Attaches and detaches the sensors and does a couple of one time operations
    virtual SbBool setUpConnections(SbBool onoff, SbBool doitalways);

	/// Calls the function that rebuilds the object
    static void verticesCB(void* data, SoSensor* sensor);

	/// Calls the function that rebuilds the object
    static void extrusionVectorCB(void* data, SoSensor* sensor);

	/// Rebuilds the Extrusion Object
	void refresh();

};

#endif