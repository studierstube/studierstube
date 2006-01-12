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
* $Id: SoTrackedViewpointMobileDisplayControlMode.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#include "SoTrackedViewpointMobileDisplayControlMode.h"
#include "MultRotRot.h"
#include <Inventor/nodes/SoTransform.h>
#include "StbViewer/SoOffAxisCamera.h"
#include "StbViewer/SoDisplay.h"
#include "StbKernel/StbKernel.h"
#include "StbKernel/interfaces/SoTrackedItemInterface.h"
#include <Inventor/engines/SoTransformVec3f.h> 

SO_NODE_SOURCE(SoTrackedViewpointMobileDisplayControlMode);


void
SoTrackedViewpointMobileDisplayControlMode::initClass()
{
    SO_NODE_INIT_CLASS(SoTrackedViewpointMobileDisplayControlMode, SoStbCameraControlMode, "SoStbCameraControlMode");
}

SoTrackedViewpointMobileDisplayControlMode::SoTrackedViewpointMobileDisplayControlMode()
{
    SO_NODE_CONSTRUCTOR(SoTrackedViewpointMobileDisplayControlMode);

    SO_NODE_ADD_FIELD(stbSinkNameViewpoint, ("")); 
	SO_NODE_ADD_FIELD(stbSinkNameDisplay, ("")); 

	SO_NODE_ADD_FIELD(eyeOffset, (0.0, 0.0, 0.0));
	SO_NODE_ADD_FIELD(displayOffset, (0.0, 0.0, 0.0)); 
	SO_NODE_ADD_FIELD(displayRotationOffset, (SbVec3f( 0.f, 0.f, 1.f ), 0.f )); 


}


SoTrackedViewpointMobileDisplayControlMode::~SoTrackedViewpointMobileDisplayControlMode()
{
	//nil
}


bool 
SoTrackedViewpointMobileDisplayControlMode::activate()
{
	if(stbCamera==NULL)
		return false;

	StbKernel *theKernel=StbKernel::getInstance();
	SoDisplay* display = stbCamera->getSoDisplay();

	SoTrackedItemInterface* trackedItemViewpoint=theKernel->getSoTrackedItem();
	SoTrackedItemInterface* trackedItemDisplay=theKernel->getSoTrackedItem();

	if(!trackedItemViewpoint){
		printf("STB_ERROR: SoTrackedViewpointControlMode could not get the requested SoTrackedItem to track the viewpoint\n");
		return false;
	}

	if(!trackedItemDisplay){
		printf("STB_ERROR: SoTrackedViewpointControlMode could not get the requested SoTrackedItem to track the display\n");
		return false;
	}

	display->addTrackedItem(trackedItemViewpoint);
	display->addTrackedItem(trackedItemDisplay);

	trackedItemDisplay->stbSinkName.setValue(stbSinkNameDisplay.getValue().getString());
	trackedItemViewpoint->stbSinkName.setValue(stbSinkNameViewpoint.getValue().getString());

	//set up connection
	connectHeadTracker(&trackedItemViewpoint->translation, &trackedItemViewpoint->rotation);
	connectDisplayTracker(&trackedItemDisplay->translation, &trackedItemDisplay->rotation);


	return true;
}

//----------------------------------------------------------------------------
void
SoTrackedViewpointMobileDisplayControlMode::disconnectHeadTracker()
{
	((SoOffAxisCamera*)stbCamera->getCamera())->eyepointPosition.disconnect();
}


//----------------------------------------------------------------------------
void
SoTrackedViewpointMobileDisplayControlMode::connectHeadTracker(SoSFVec3f *trackerTranslation,
										 SoSFRotation *trackerRotation)
{
	disconnectHeadTracker();

	// use engine to create tracker to world transformation matrix
	SoComposeMatrix *ctw = new SoComposeMatrix;
	ctw->translation.connectFrom(trackerTranslation);
	ctw->rotation.connectFrom(trackerRotation);

	connectHeadTrackerStep2(ctw);
}


//----------------------------------------------------------------------------
void
SoTrackedViewpointMobileDisplayControlMode::connectHeadTrackerStep2(SoComposeMatrix *ctw)
{
	// use engines to transform offsets
	SoTransformVec3f *te;
	te = new SoTransformVec3f;
	te->vector.connectFrom(&eyeOffset);
	te->matrix.connectFrom(&ctw->matrix);

	// connect eyepointPositions to transformed offsets
	((SoOffAxisCamera*)stbCamera->getCamera())->eyepointPosition.connectFrom(&te->point);
}



//----------------------------------------------------------------------------

void
SoTrackedViewpointMobileDisplayControlMode::disconnectDisplayTracker()
{
	((SoOffAxisCamera*)stbCamera->getCamera())->position.disconnect();
	((SoOffAxisCamera*)stbCamera->getCamera())->orientation.disconnect();
}



//----------------------------------------------------------------------------
void
SoTrackedViewpointMobileDisplayControlMode::connectDisplayTracker(SoSFVec3f *trackerTranslation,
											SoSFRotation *trackerRotation)
{
	disconnectDisplayTracker();

	// use engine to create tracker to world transformation matrix
	SoComposeMatrix *ctw = new SoComposeMatrix;
	ctw->translation.connectFrom(trackerTranslation);
	ctw->rotation.connectFrom(trackerRotation);

	connectDisplayTrackerStep2(ctw);

	// use engines to calculate rotations	
	MultRotRot *md = new MultRotRot;
	md->rotationA.connectFrom(&displayRotationOffset);
	md->rotationB.connectFrom(trackerRotation);

	// connect orientations to calculated rotations
	((SoOffAxisCamera*)stbCamera->getCamera())->orientation.connectFrom(&md->product);

}

//----------------------------------------------------------------------------
void
SoTrackedViewpointMobileDisplayControlMode::connectDisplayTrackerStep2(SoComposeMatrix *ctw)
{
	// use engines to transform offsets
	SoTransformVec3f *tdo;
	tdo = new SoTransformVec3f;
	tdo->vector.connectFrom(&displayOffset);
	tdo->matrix.connectFrom(&ctw->matrix);

	// connect positions to transformed offsets
	((SoOffAxisCamera*)stbCamera->getCamera())->position.connectFrom(&tdo->point);		
}
