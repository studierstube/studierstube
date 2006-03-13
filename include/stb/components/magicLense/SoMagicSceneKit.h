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
/** The header file for the SoMagicSceneKit
*
* @author Erick Mendez
* @ingroup vidente
*
* $Id: SoMagicSceneKit.h 2006-03-10 mendez $
* @file                                                                   */
/* ======================================================================= */


#ifndef _SOMAGICSCENEKIT_H_
#define _SOMAGICSCENEKIT_H_

#ifdef USE_VIDENTE

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

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/fields/SoSFString.h>
#include <stb/components/starlight/SoNodeContextReport.h>

#include "SoMagicScene.h"
#include "SoMagicStylesKit.h"
#include "starlight.h"

/**
 * This class is the kit the user will be using for defining a the scene of a magic lens
 *
 * @author Erick Mendez
 * @ingroup vidente
 */

class STARLIGHT_API SoMagicSceneKit: public SoBaseKit
{
	/// Parts of the catalog
	SO_KIT_HEADER(SoMagicSceneKit);
	SO_KIT_CATALOG_ENTRY_HEADER(sepTop);

	SO_KIT_CATALOG_ENTRY_HEADER(sepBehind);
	SO_KIT_CATALOG_ENTRY_HEADER(behindRenderStyle);
	SO_KIT_CATALOG_ENTRY_HEADER(content); //Assumed to be behind

	SO_KIT_CATALOG_ENTRY_HEADER(sepInside);
	SO_KIT_CATALOG_ENTRY_HEADER(insideRenderStyle);
	SO_KIT_CATALOG_ENTRY_HEADER(contentInside);

	SO_KIT_CATALOG_ENTRY_HEADER(sepInfront);
	SO_KIT_CATALOG_ENTRY_HEADER(contentInfront);
	SO_KIT_CATALOG_ENTRY_HEADER(infrontRenderStyle);


public:

	/// This holds the style to be used
	SoSFString styleName;

	/// The constructor of the class, initializes the catalog
	SoMagicSceneKit();

	/// Destructor, deletes the sensors
	~SoMagicSceneKit();

	/// Initializes the node kit
	static void initClass();

	/// Sets the content 
	void setContent(SoNode *newContent);

	/// Propagates the content 
	void propagateContent();

	/// Disables or Enables Magic rendering
	void goMagic(bool flag);

	/// Sets what style should we use for rendering
	void setStyle(char *strStyleName);

protected:

	/// Sensor to refresh the style
	SoFieldSensor *styleNameSensor;

	// Calls the function that refreshes the style
	static void stylesHandleCB(void* data, SoSensor* sensor);

	// Calls the function that refreshes the style
    static void styleNameCB(void* data, SoSensor* sensor);

	/// Attaches and detaches the sensors and does a couple of one time operations
    virtual SbBool setUpConnections(SbBool onoff, SbBool doitalways);

	/// Sets the Cg parameters and renders down its tree
	virtual void GLRender(SoGLRenderAction * action);

};


#endif //USE_VIDENTE
#endif