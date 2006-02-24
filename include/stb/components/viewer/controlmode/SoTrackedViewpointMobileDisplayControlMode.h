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
/** The header file for the SoTrackedViewpointMobileDisplayControlMode class.
*
* @author Denis Kalkofen
*
* $Id: SoTrackedViewpointMobileDisplayControlMode.h 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#ifndef _SOTRACKEDVIEWPOINTMOBILEDISPLAYCONTROLMODE_H_
#define _SOTRACKEDVIEWPOINTMOBILEDISPLAYCONTROLMODE_H_

#include <stb/components/viewer/controlmode/SoStbCameraControlMode.h>
#include <stb/kernel/interfaces/SoTrakEngineInterface.h>

#include <Inventor/fields/SoSFString.h>
#include <Inventor/engines/SoCompose.h> 
#include <Inventor/fields/SoSFVec3f.h> 

class  SoTrackedViewpointMobileDisplayControlMode
: public SoStbCameraControlMode
{
SO_NODE_HEADER(SoTrackedViewpointMobileDisplayControlMode);
public:
	SoSFString stbSinkNameViewpoint;

	SoSFString stbSinkNameDisplay;

	/** Offset between head tracker and eye
	* (in tracker space).
	*/
	SoSFVec3f eyeOffset;	

	/** Offset between display tracker and left display surface center
	(in display tracker space).
	*/
	SoSFVec3f displayOffset;	

	/** Rotation from display tracker orientation to 
	left display surface orientation
	*/
	SoSFRotation displayRotationOffset;

    static void initClass();

    SoTrackedViewpointMobileDisplayControlMode();

	bool activate();

private:
    ~SoTrackedViewpointMobileDisplayControlMode();
    
	void disconnectHeadTracker();
	void connectHeadTracker(SoSFVec3f *trackerTranslation,SoSFRotation *trackerRotation);
	void connectHeadTrackerStep2(SoComposeMatrix *ctw);

	void disconnectDisplayTracker();
	void connectDisplayTracker(SoSFVec3f *trackerTranslation,SoSFRotation *trackerRotation);
	void connectDisplayTrackerStep2(SoComposeMatrix *ctw);

    void connectHeadTracker(stb::SoTrakEngineInterface *tracker);
    void connectDisplayTracker(stb::SoTrakEngineInterface *tracker);

    stb::SoTrakEngineInterface* trHead;
    stb::SoTrakEngineInterface* trDisplay;
};
	

#endif//_SOTRACKEDVIEWPOINTMOBILEDISPLAYCONTROLMODE_H_

