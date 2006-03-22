/* ========================================================================
* Copyright (C) 2005  Graz University of Technology
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
* <schmalstieg@icg.tu-graz.ac.at> or write to Dieter Schmalstieg,
* Graz University of Technology, Inffeldgasse 16a, A8010 Graz,
* Austria.
* ========================================================================
* PROJECT: Studierstube
* ======================================================================== */
/** The header file for the SoMagicStylesKit
*
* @author Erick Mendez
* @ingroup vidente
*
* $Id: SoMagicStylesKit.h 2006-03-10 mendez $
* @file                                                                   */
/* ======================================================================= */

#ifndef _SOMAGIKSTYLESKIT_H_
#define _SOMAGIKSTYLESKIT_H_



/*
 * --------------------------------------------------------------------------------
 * Includes
 * --------------------------------------------------------------------------------
 */
#ifdef WIN32
#include <SoWinEnterScope.h>
#include <windows.h>
#endif

#include <Inventor/actions/SoGLRenderAction.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

#include <time.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoMFNode.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <stb/components/magicLens/CgFboManager.h>
#include <stb/components/starlight/starlight.h>

//#define PRINT_FRAMERATE

/**
 * Class that handles whatever is necessary for the implementation of the Magic Lens
 * for example, Cg context initialization, Fbo initialization, and SoContext for the
 * propagation of the file names.
 *
 * @author Erick Mendez
 * @ingroup vidente
 */

class SoMagicStylesKit: public SoBaseKit
{
    SO_KIT_HEADER(SoMagicStylesKit);

public:

#if (defined(PRINT_FRAMERATE) && defined(WIN32))
	DWORD thisTime;
	DWORD lastTime;
	float diffTime;
	float framerate;
#endif

	/// The Style names
	SoMFString styleNames;

	/// The Styles
	SoMFNode stylesInside;

	/// The Styles
	SoMFNode stylesOutside;

	/// Hey! it's a Singleton
	static SoMagicStylesKit *getInstance();

	/// Destructor, deletes the sensors
    ~SoMagicStylesKit();

	///  Initializes the node kit
    static void initClass();

	/// Sets the rendering style for inside the Lens
	void setStyle(char *strStyleName, SoNode *innerStyle, SoNode *outerStyle);

	/// Refreshes the styles
	void refresh();

private:
     static SoMagicStylesKit* pinstance;
protected:

	/// The constructor of the class, initializes the catalog
    SoMagicStylesKit();

	/// Flag for initialization
	bool isLoaded;

	/// The number of styles
	int nNumberOfStyles;

	/// Sensors
	SoFieldSensor *styleNamesSensor;
	SoFieldSensor *stylesInsideSensor;
	SoFieldSensor *stylesOutsideSensor;

	/// The parts of the catalog
	SO_KIT_CATALOG_ENTRY_HEADER(sepTop);
	SO_KIT_CATALOG_ENTRY_HEADER(cxnRenderStyles);

	/// Attaches and detaches the sensors and does a couple of one time operations
    virtual SbBool setUpConnections(SbBool onoff, SbBool doitalways);

	/// Calls the function that refreshes the styles
	static void stylesCB(void* data, SoSensor* sensor);

	///  Sets an specific rendering style
	void setStyle(char *strStyleName, SoNode *style);

	/// Adds a style to our SoContext
	void addStyle(char *strStyleName, SoNode *style);

	/// Updates a style from our SoContext
	void updateStyle(int nIndex, SoNode *style);

	virtual void GLRender(SoGLRenderAction * action);

};


#endif