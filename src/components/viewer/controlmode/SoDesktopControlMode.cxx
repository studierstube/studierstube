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
/** The header file for the SoDesktopControlMode class.
*
* @author Denis Kalkofen
*
* $Id: SoDesktopControlMode.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#include <stb/components/viewer/controlmode/SoDesktopControlMode.h>
#include <stb/components/viewer/Viewer.h>
#include <stb/components/viewer/SoDisplay.h>
#include <Inventor/nodes/SoPerspectiveCamera.h> 
#include <stb/components/viewer/SoOffAxisCamera.h>
#include <stb/components/viewer/controlmode/MultRotRot.h>
#include <stb/kernel/Kernel.h>
//----------------------------------------------------------------------------

SO_NODE_SOURCE(SoDesktopControlMode);

//----------------------------------------------------------------------------
// Inventor class initialization 

void 
SoDesktopControlMode::initClass()
{
    SO_NODE_INIT_CLASS(SoDesktopControlMode, SoStbCameraControlMode, "SoStbCameraControlMode");
}

//----------------------------------------------------------------------------
// Abstract constructor. 

SoDesktopControlMode::SoDesktopControlMode()
{
  SO_NODE_CONSTRUCTOR(SoDesktopControlMode);;
}

//----------------------------------------------------------------------------
// Abstract destructor. 

SoDesktopControlMode::~SoDesktopControlMode()
{
    // nil
}


bool 
SoDesktopControlMode::activate()
{
	if(stbCamera==NULL)
		return false;
	
	// set offsets 
	//get values
	//SoPerspectiveCamera *refCam = stbCamera->getReferenceCamera();
    //SoPerspectiveCamera*
    //    SoStbCamera::getReferenceCamera()
    //{
   SoDisplay *dsp=stb::Viewer::findSoDisplay(this);
        if(!dsp){
            return false;
        }
     SoPerspectiveCamera *refCam = dsp->getReferenceCamera();
    //}
	//SbVec3f eposLeft;  
	//SbVec3f posLeft;
    //SoPerspectiveCamera*
    //    SoStbCamera::getReferenceCamera()
    //{
    //    SoDisplay *dsp=stb::Viewer::findSoDisplay(this);
    //    if(!dsp){
    //        printf("!dsp");
    //        return false;
    //    }
    //    return dsp->getReferenceCamera();
    //}
	//if(stbCamera->getCamera()!=NULL)
	//{
	//	if(stbCamera->getCamera()->isOfType(SoOffAxisCamera::getClassTypeId()))
	//	{
	//		eposLeft  = ((SoOffAxisCamera*)stbCamera->getCamera())->eyepointPosition.getValue();
	//		posLeft	  = ((SoOffAxisCamera*)stbCamera->getCamera())->position.getValue();
	//	}
	//	else if(stbCamera->getCamera()->isOfType(SoCamera::getClassTypeId()))
	//	{
	//		stb::Kernel::getInstance()->log("STB_ERROR: can't active SoDesktopControlMode for none SoOffAxisCamera's\n");
 //              stbCamera->getTransform()->translation.connectFrom(&refCam->position);
 //              stbCamera->getTransform()->rotation.connectFrom(&refCam->orientation);
 //            
	//		return true;
	//	}
	//}

	//SbVec3f posRef	  = refCam->position.getValue();//= (eposLeft + eposRight)/2.0f;
	//SbRotation rotRef = refCam->orientation.getValue();// = diff * rotRight;	
	////
	//SbVec3f temp;
	//rotRef.inverse().multVec((eposLeft - posRef),temp);
	//eyeOffset = temp;
	//rotRef.inverse().multVec((posLeft - posRef),temp);
	//displayOffset = temp;
	//
	////set up connection
	//connectHeadTracker(&refCam->position, &refCam->orientation);
	//connectDisplayTracker(&refCam->position, &refCam->orientation);
    
    stbCamera->getTransform()->translation.connectFrom(&refCam->position);
    stbCamera->getTransform()->rotation.connectFrom(&refCam->orientation);

	return true;
}
