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
/** Class definition for the SoLineStringKit
*
*
* @author Erick Mendez
*
* $Id: $
* @file                                                                   */
/* ======================================================================= */

#ifndef _SOLINESTRINGKIT_H_
#define _SOLINESTRINGKIT_H_

/*
 * --------------------------------------------------------------------------------
 * Includes
 * --------------------------------------------------------------------------------
 */
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/fields/SoMFVec2f.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include "starlight.h"

class STARLIGHT_API SoLineStringKit: public SoBaseKit
{
    SO_KIT_HEADER(SoLineStringKit);
	typedef SoBaseKit inherited;

public:
	/// Initializes the node kit
    static void initClass();

	/// The constructor of the class, initializes the catalog
    SoLineStringKit();

	/// Destructor, deletes the sensors
    ~SoLineStringKit();

	/// The input vertices of the line string
	SoMFVec2f vertices;

	/// Width of the line
	SoSFFloat width;

protected:

	/// Sensors
	SoFieldSensor *verticesSensor;
	SoFieldSensor *widthSensor;

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
    static void widthCB(void* data, SoSensor* sensor);

	/// Rebuilds the Extrusion Object
	void refresh();

};

#endif