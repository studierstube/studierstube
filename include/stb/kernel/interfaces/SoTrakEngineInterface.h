/* ========================================================================
 * Copyright (C) 2000-2005  Vienna University of Technology
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
/** Header file for SoTrakEngineInterface
  *
  * @author Denis Kalkofen
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SOTRAKENGINEINTERFACE_H_
#define _SOTRAKENGINEINTERFACE_H_
#include <stb/kernel/Studierstube.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFRotation.h>
#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/fields/SoMFName.h>
#include <Inventor/fields/SoMFString.h>
//#include <Inventor/sensors/SoFieldSensor.h>
#include <stb/base/macros.h>

BEGIN_NAMESPACE_STB

class STB_API SoTrakEngineInterface : public SoEngine
{
    SO_ENGINE_HEADER(SoTrakEngineInterface);

public:
    SoMFName key;
    SoMFString value;

    SoSFVec3f       translationIn;  // input translation
    SoSFRotation    rotationIn;     // input rotation
    SoSFBool        buttonIn0;      // input button 0
    SoSFBool        buttonIn1;      // input button 1
    SoSFBool        buttonIn2;      // input button 2
    SoSFBool        buttonIn3;      // input button 3
    SoSFBool        buttonIn4;      // input button 4
    SoSFBool        buttonIn5;      // input button 5
    SoSFBool        buttonIn6;      // input button 6
    SoSFBool        buttonIn7;      // input button 7

    SoEngineOutput  translation;    // (SoSFVec3f) tracker translation
    SoEngineOutput  rotation;       // (SoSFRotation) tracker rotation
    SoEngineOutput  button0;        // (SoSFBool) tracker button 0 value
    SoEngineOutput  button1;        // (SoSFBool) tracker button 1 value
    SoEngineOutput  button2;        // (SoSFBool) tracker button 2 value
    SoEngineOutput  button3;        // (SoSFBool) tracker button 3 value
    SoEngineOutput  button4;        // (SoSFBool) tracker button 4 value
    SoEngineOutput  button5;        // (SoSFBool) tracker button 5 value
    SoEngineOutput  button6;        // (SoSFBool) tracker button 6 value
    SoEngineOutput  button7;        // (SoSFBool) tracker button 7 value

    SoTrakEngineInterface(void);
    static void initClass(void);

protected:
    virtual ~SoTrakEngineInterface();
    virtual void evaluate();        // engine's evaluate method

};
END_NAMESPACE_STB
#endif
