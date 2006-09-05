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
/** Header file for SoMultimodalEngine engine
  *
  * @author Eduardo Veas
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SOMULTIMODALENGINE_H_
#define _SOMULTIMODALENGINE_H_

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFShort.h>
#include <Inventor/fields/SoSFTime.h>

#include <Inventor/fields/SoSFUInt32.h>

#include <Inventor/fields/SoSFUShort.h>
#include <Inventor/fields/SoSFImage.h>
#include <Inventor/fields/SoSFImage3.h>
#include <Inventor/fields/SoSFMatrix.h>

#include <Inventor/fields/SoSFRotation.h>
#include <Inventor/fields/SoMFName.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoSFString.h>

#include "SoInputEvent.h"

template <typename T> class EventAdapter;

class EVENT_API SoMultimodalEngine : public SoEngine
{
    SO_ENGINE_HEADER(SoMultimodalEngine);

public:
    SoMFName key;
    SoMFString value;
    
    SoSFString attrName;
    SoSFString attrType;

    SoSFBool		buttonHisteresis;  // Flag to allow buttons to depend on their immediate history
	
	// Engine Input
    SoSFVec3f       vec3fIn;     // input vec3f
    SoSFRotation    rotationIn;        // input rotation
    SoSFBool        boolIn;            // input button 0
    SoSFFloat       floatIn;           // float input
    SoSFInt32       intIn;             // int input
    SoSFShort       shortIn;           // short input
    SoSFUInt32      uintIn;            // guess...yes unsigned int input
    SoSFUShort      ushortIn;          // unsigned short input
    SoSFString      stringIn;          // string input

    SoSFTime        timeIn;            // timestamp input

    // just in case, these are unused and will probably be removed in
    // the near future. 
    SoSFImage       imageIn;
    SoSFImage3      image3In;
    SoSFMatrix      matrixIn;

	// Engine Output
    SoEngineOutput  vec3fValue;       // (SoSFVec3f) tracker vec3f
    SoEngineOutput  rotationValue;          // (SoSFRotation) tracker rotation
    SoEngineOutput  boolValue;              // (SoSFBool) bool output
    SoEngineOutput floatValue;              // (SoSFFloat) float output
    SoEngineOutput intValue;                // (SoSFInt32) int output
    SoEngineOutput shortValue;              // (SoSFShort) short output
    SoEngineOutput uintValue;               // (SoSFUInt) unsigned int output
    SoEngineOutput ushortValue;             // (SoSFUShort) unsigned short output
    SoEngineOutput stringValue;             // (SoSFString) string output

    // just in case
    SoEngineOutput imageValue;
    SoEngineOutput image3Value;
    SoEngineOutput matrixValue;   



    SoMultimodalEngine(void);
    static void initClass(void);

protected:
    virtual ~SoMultimodalEngine();
    virtual void evaluate();        // engine's evaluate method

    virtual void inputChanged(SoField* whichField); // react to input changes field that has changed
    
    void processEvent(SoInputEvent *event);

    EventAdapter<SoMultimodalEngine> *adapter;
    friend class EventAdapter<SoMultimodalEngine>;

private:
	// FIXME: Should probably go as static inside evaluate(). Mendez 20060523

	// Used for histeresis
	SbBool        buttonHistory;      // history of button 

	// Used for detecting button changes 
	SbBool        buttonChange;       // Change of button

};


#endif


