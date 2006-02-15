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
/** The header file for the StbCamera class.
*
* @author Denis Kalkofen
*
* $Id: StbCamera.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#include "SoStbCamera.h"

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransformSeparator.h>

#include "SoStudierstubeViewer.h"
#include "SoDisplay.h"

#include "SoStudierstubeViewer.h"
#include "displaymode/SoDisplayMode.h"
#include "SoVideoBackground.h"
#include "controlmode/SoStbCameraControlMode.h"
#include "SoViewport.h"


SO_KIT_SOURCE(SoStbCamera);

//----------------------------------------------------------------------------
/** Inventor class initialization */

void
SoStbCamera::initClass()
{	
	SO_KIT_INIT_CLASS(SoStbCamera, SoBaseKit, "SoBaseKit");
}

//----------------------------------------------------------------------------
/** The constructor inits all fields. */
SoStbCamera::SoStbCamera() 
{
    SO_KIT_CONSTRUCTOR(SoStbCamera);
    
	// SO_KIT_ADD_CATALOG_ENTRY(
    // name,className,nullByDefault,parentName,rightSiblingName,isPublic
	//--------------- SoUser structure -------------------------------------------
	//						  kitRoot (SoStbCamera)
	//							 | 
	//                           | 
	//                           |
	//                 |----------------|
	//                 |				|			
	//				 camRoot(xfSep)		  content
	//                 |
	//		|--------------|-----------|-------------|------------|-------------|
	// cameraTransform  controlMode  viewport  videoBackground dispMode  camera(SoOffAxis)
	//----------------------------------------------------------------------------
	
	SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(content,SoNode,SoSeparator,TRUE, this,"", TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(xfSep,SoTransformSeparator,FALSE, this,content, FALSE);	
	
	SO_KIT_ADD_CATALOG_ENTRY(camera,SoCamera,TRUE, xfSep,"", TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(displayMode,SoDisplayMode,TRUE, xfSep,camera, TRUE);

	SO_KIT_ADD_CATALOG_ENTRY(videoBackground,SoVideoBackground,TRUE, xfSep,displayMode, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(viewport,SoViewport,TRUE, xfSep,videoBackground, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(controlMode,SoStbCameraControlMode,TRUE, xfSep,viewport, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(transform,SoTransform, FALSE , xfSep,controlMode, TRUE);


    SO_KIT_INIT_INSTANCE();

	referenceCamera=NULL;
}

/** The destructor */
SoStbCamera::~SoStbCamera()
{
	//nil
}

bool
SoStbCamera::hasContent()
{
	if(SO_CHECK_ANY_PART(this,"content",SoNode)==NULL)
		return false;

	return TRUE;
}

void 
SoStbCamera::setContent(SoNode *newContent)
{
	if(newContent)
		setAnyPart("content",newContent);
}

void 
SoStbCamera::setReferenceCamera(SoPerspectiveCamera *refCam)
{
	referenceCamera=refCam;
}

SoPerspectiveCamera*
SoStbCamera::getReferenceCamera()
{
	return referenceCamera;
}

SoCamera* 
SoStbCamera::getCamera()
{
	return (SoCamera*)getAnyPart("camera",false,false,false);
}

SoTransform* 
SoStbCamera::getTransform()
{
	return (SoTransform*)getAnyPart("transform",false,false,false);
}

bool
SoStbCamera::activateControlMode()
{	
	if(controlMode.getValue())
	{
		if(!(SoCamera*)getAnyPart("camera",false,false,false)){
			printf("STB_ERROR: StbCameraControlMode: 'no camera found to control'\n ");
			return false;
		}

		//((SoStbCameraControlMode*)controlMode.getValue())->setStbCamera(this);
		//((SoStbCameraControlMode*)controlMode.getValue())->activate();
		
		// activate update
		//if((controlMode.getValue())->isOfType(SoTrackedDisplayControlMode::getClassTypeId()))
		//{
		//	//soDisplay->startUpdate(30);
		//}

		return true;
	}
	return false;
}

bool
SoStbCamera::activateDisplayMode(SoStudierstubeViewer* aViewer)
{
	if(displayMode.getValue()){
		((SoDisplayMode*)(displayMode.getValue()))->setViewer(aViewer);
		return true;
	}
	return false;
}

void 
SoStbCamera::setSoDisplay(SoDisplay *aDisplay)
{
	soDisplay=aDisplay;
}

SoDisplay*
SoStbCamera::getSoDisplay()
{
	return soDisplay;
}

