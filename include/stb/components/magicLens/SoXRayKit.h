/*
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
/** The header file for the SoXRayKit
*
* @author Erick Mendez
* @ingroup vidente
*
* $Id: SoXRayKit.h 2006-03-10 mendez $
* @file                                                                   */
/* ======================================================================= */

#ifndef _SOXRAYKIT_H_
#define _SOXRAYKIT_H_


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

#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <stb/components/starlight/starlight.h>
#include <stb/components/magicLens/SoMagicLensKit.h>

/**
* This class is the kit the user will be using for creating an XRay vision tunnel
*
* @author Erick Mendez
* @ingroup vidente
*/
class SoXRayKit: public SoBaseKit
{
protected:
	/// Parts of the catalog
	SO_KIT_HEADER(SoXRayKit);
	SO_KIT_CATALOG_ENTRY_HEADER(lensKit);

public:
	/// The constructor of the class, initializes the catalog
	SoXRayKit();

	/// Destructor, deletes the sensors
	~SoXRayKit();

	/// Initializes the node kit
	static void initClass();

	/// The size the tunnel should cover of the viewing frustum
	SoSFFloat ratio;

	/// A pointer to the camera we want to XRay
	SoSFNode camera;

protected:

	/// Shapehints of the form of the tunnel
	SoShapeHints *shapeHints;

	/// The coordinates of the tunnel object
	SoCoordinate3 *coords;

	/// The faces of the tunnel object
	SoIndexedFaceSet *faces;

	/// Sensor to refresh the tunnel when the ratio changes
	SoFieldSensor *ratioSensor;

	// Calls the function that refreshes the tunnel
	static void ratioCB(void* data, SoSensor* sensor);

	/// Rebuilds the tunnel
	void refresh();

	/// Attaches and detaches the sensors and does a couple of one time operations
	virtual SbBool setUpConnections(SbBool onoff, SbBool doitalways);

	/// Triggers a render action
	virtual void GLRender(SoGLRenderAction * action);
};

#endif //_SOXRAYKIT_H_