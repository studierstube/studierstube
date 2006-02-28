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
* $Id: StbCamera.h 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#ifndef _SOSTBCAMERA_H_
#define _SOSTBCAMERA_H_

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoTransform.h>


class SoStudierstubeViewer;
class SoDisplay;

class SoStbCamera : public SoBaseKit
{
    SO_KIT_HEADER(SoStbCamera);
    SO_KIT_CATALOG_ENTRY_HEADER(root);
	SO_KIT_CATALOG_ENTRY_HEADER(content);
	SO_KIT_CATALOG_ENTRY_HEADER(xfSep);
	SO_KIT_CATALOG_ENTRY_HEADER(camera);

	SO_KIT_CATALOG_ENTRY_HEADER(transform);
	SO_KIT_CATALOG_ENTRY_HEADER(controlMode);
	SO_KIT_CATALOG_ENTRY_HEADER(displayMode);
	SO_KIT_CATALOG_ENTRY_HEADER(videoBackground);
	SO_KIT_CATALOG_ENTRY_HEADER(viewport);

public:
	/** The constructor */
    SoStbCamera();

    /** The destructor does nothing */
    ~SoStbCamera();

    /** Inventor class initialization */
    static void initClass();


	/** Returns true if the content was explicitly added */
	bool hasContent();

	/** 
	* Sets the SoStbCamera's content 
	*/
	void setContent(SoNode *newContent);

	/** 
	*	Sets the SoStbCamera's reference camera.
	*/
	void setReferenceCamera(SoPerspectiveCamera* refCam);

	/** 
	*	Get the SoStbCamera's reference camera.
	*/
	SoPerspectiveCamera* getReferenceCamera();

	/** 
	*	Return the SoStbCamera's camera.
	*/
	SoCamera* getCamera();

	/** 
	*	Return the SoStbCamera's transform node.
	*/
	SoTransform* getTransform();
	
	/** 
	* The StbCamera's controlMode needs to be activated. 
	*/
	bool activateControlMode();

	bool activateDisplayMode(SoStudierstubeViewer* aViewer);

	void setSoDisplay(SoDisplay *aDisplay);

	SoDisplay* getSoDisplay();

protected:
	SoDisplay* soDisplay;

	SoPerspectiveCamera* referenceCamera;
	
};

//----------------------------------------------------------------------------
#endif//_SOSTBCAMERA_H_
