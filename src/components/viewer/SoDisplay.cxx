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
/** The header file for the SoDisplay class.
*
* @author Denis Kalkofen
*
* $Id: SoDisplay.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */
#include "SoDisplay.h"

#include "kernel/Studierstube.h"
#include "guiDefines.h"
#include "SoStudierstubeViewer.h"

#include "SoStbCamera.h"
#include "SoOffAxisCamera.h"

#include SOGUI_H
#include SOGUI_CURSOR_H

#include <Inventor/actions/SoSearchAction.h>


SO_NODE_SOURCE(SoDisplay);

//----------------------------------------------------------------------------
/** Inventor class initialization */
void
SoDisplay::initClass()
{	
	SO_NODE_INIT_CLASS(SoDisplay, SoNode, "SoGroup");
}

//----------------------------------------------------------------------------
/** The constructor inits all fields. */
SoDisplay::SoDisplay()
: viewer(NULL)
{
    SO_NODE_CONSTRUCTOR(SoDisplay);
    
    SO_NODE_ADD_FIELD(content, (NULL));
    SO_NODE_ADD_FIELD(stbCameraList, (NULL));
    
    //////// StudierstubeViewer Configurations
    SO_NODE_ADD_FIELD(xoffset, (0)); 
    SO_NODE_ADD_FIELD(yoffset, (0)); 
    SO_NODE_ADD_FIELD(width, (640)); 
    SO_NODE_ADD_FIELD(height, (480)); 
    SO_NODE_ADD_FIELD(transparencyType, (SoDisplay::BLEND)); 
    SO_NODE_ADD_FIELD(headlight, (TRUE)); 
    SO_NODE_ADD_FIELD(headlightIntensity, (1.0f)); 
    SO_NODE_ADD_FIELD(backgroundColor, (0,0,0)); 
    SO_NODE_ADD_FIELD(showMouse, (TRUE));
    SO_NODE_ADD_FIELD(windowBorder, (TRUE));
    SO_NODE_ADD_FIELD(decoration, (TRUE));
    SO_NODE_ADD_FIELD(windowOnTop, (FALSE));
    SO_NODE_ADD_FIELD(stencilBuffer, (FALSE));
    
    SO_NODE_DEFINE_ENUM_VALUE(TransparencyType, SCREEN_DOOR);
    SO_NODE_DEFINE_ENUM_VALUE(TransparencyType, ADD);
    SO_NODE_DEFINE_ENUM_VALUE(TransparencyType, DELAYED_ADD);
    SO_NODE_DEFINE_ENUM_VALUE(TransparencyType, SORTED_OBJECT_ADD);
    SO_NODE_DEFINE_ENUM_VALUE(TransparencyType, BLEND);
    SO_NODE_DEFINE_ENUM_VALUE(TransparencyType, DELAYED_BLEND);
    SO_NODE_DEFINE_ENUM_VALUE(TransparencyType, SORTED_OBJECT_BLEND);
    SO_NODE_DEFINE_ENUM_VALUE(TransparencyType, NONE);
    SO_NODE_DEFINE_ENUM_VALUE(TransparencyType, SORTED_OBJECT_SORTED_TRIANGLE_ADD);
    SO_NODE_DEFINE_ENUM_VALUE(TransparencyType, SORTED_OBJECT_SORTED_TRIANGLE_BLEND);
    SO_NODE_DEFINE_ENUM_VALUE(TransparencyType, SORTED_LAYERS_BLEND);
    SO_NODE_SET_SF_ENUM_TYPE(transparencyType, TransparencyType);
}

//----------------------------------------------------------------------------
/** The destructor */
SoDisplay::~SoDisplay()
{
	if(viewer)
    {
		viewer->hide();
		delete viewer;
		displayRoot->unref();
	}
}

//----------------------------------------------------------------------------
//


//----------------------------------------------------------------------------
// see SoDisplay.h
void 
SoDisplay::createViewer()
{
	//	set up the viewer's basic graph
	//
    //							   root
	//								|
	//          |---------------------
	//         examCam    
	//               		
	//					
	//
	//
	displayRoot=new SoSeparator();
    this->addChild(displayRoot);
	examCam= new SoPerspectiveCamera();
	displayRoot->addChild(examCam);

	////////////////////////////////////////
	// create StudierstubeViewer
	////////////////////////////////////////
	viewer=new SoStudierstubeViewer(NULL);

	///////////////////////////////////////////////////
	// add all the stbCameras to the viewer's root node
    for (int i=0;i<stbCameraList.getNum();i++)
    {
        if(stbCameraList[i]!=NULL 
        && stbCameraList[i]->isOfType(SoStbCamera::getClassTypeId()))
        {
	        SoStbCamera *stbCamera=(SoStbCamera *)stbCameraList[i];
	        //Set reference camera's parameter to match with the OffAxisCamera of the first StbCamera 
	        if(	i==0 
		        && stbCamera->getCamera()
		        && stbCamera->getCamera()->isOfType(SoOffAxisCamera::getClassTypeId())
		        )
	        {
		        SoPerspectiveCamera *refCam = (SoPerspectiveCamera*)examCam;
		        SoOffAxisCamera *cam=(SoOffAxisCamera*)stbCamera->getCamera();

		        refCam->aspectRatio		=	cam->size.getValue()[0]/cam->size.getValue()[1];
		        refCam->focalDistance	=	cam->focalDistance;
		        refCam->nearDistance	=	cam->nearDistance;
		        refCam->farDistance		=	cam->farDistance;
		        refCam->viewportMapping =	cam->viewportMapping;
		        refCam->orientation		=	cam->orientation;
	        }

	        ///////////////////////////////////////////////////////////////////////////////////////
	        ///////////// Tell the StbCamera about SoDisplay's reference camera  ///////////////////
	        stbCamera->setSoDisplay(this);
	        stbCamera->setReferenceCamera(examCam);

	        ///////////////////////////////////////////////
	        ///////////// Set Content  ///////////////////
	        //set the displays content only if no content has been defined for the StbCamera
	        if(!stbCamera->hasContent())
	        {
		        stbCamera->setContent(content.getValue());
	        }
            stbCamera->activateControlMode();
            stbCamera->activateDisplayMode(viewer);
	        ///////////////////////////////////////////////////////////
	        //////////// Add StbCamera to StudierstubeViewer's root ////
	        displayRoot->addChild(stbCamera);	
        }
    }
  
	//////////////////////////////////////////
	////  Configure  StudierstubeViewer 
	//////////////////////////////////////////
	////windowDecoreation
	viewer->setWindowDecoration(windowBorder.getValue());
	if( viewer->isDecoration() != decoration.getValue())
	    viewer->setDecoration( decoration.getValue());
	
	//widownOnTop
	viewer->setWindowOnTop(windowOnTop.getValue());
	
	//stencilBuffer
	if (stencilBuffer.getValue() == TRUE)
	{
	    viewer->setStencilBuffer(TRUE);
	}

	//window pos & size
	viewer->setWindowPosSize(xoffset.getValue(), yoffset.getValue(),
                             width.getValue()  , height.getValue());
   
	//transparency
	viewer->setTransparencyType(
	    (SoGLRenderAction::TransparencyType)transparencyType.getValue());
	
	//title
	viewer->setTitle(STUDIERSTUBE_VERSION_STRING);
    
    // scene lighting
    //viewer->setHeadlight(headlight.getValue());
    //if (viewer->getHeadlight())
    // viewer->getHeadlight()->intensity.setValue(headlightIntensity.getValue());
       

	//background color 
	viewer->setBackgroundColor(backgroundColor.getValue());

	// set mouse cursor
	if( showMouse.getValue() == FALSE )
	{
		viewer->setComponentCursor(SoGuiCursor::getBlankCursor());
	}
	//////////////////////////////////////////
	//// start StudierstubeViewer
	//// set viewer's root node and show the studierstubeViewer
	//////////////////////////////////////////
	viewer->setSceneGraph(displayRoot);
	viewer->show();
	//////////////////////////////////////////
	//// start openvideo if nessesary
	//////////////////////////////////////////
}

SbBool 
SoDisplay::readInstance(SoInput *in, unsigned short flags)
{
    if(!SoNode::readInstance(in, flags))
		return false;

    createViewer();
      
    return true;
}

SoStudierstubeViewer* 
SoDisplay::getViewer()
{
	return this->viewer;
}

void 
SoDisplay::setContent(SoNode* _content)
{
    content.setValue(_content);
    // tell cameras about the new content
    for (int i=0;i<stbCameraList.getNum();i++)
    {
        if(stbCameraList[i]!=NULL 
            && stbCameraList[i]->isOfType(SoStbCamera::getClassTypeId()))
        {
            SoStbCamera *stbCamera=(SoStbCamera *)stbCameraList[i];
            ///////////////////////////////////////////////
            ///////////// Set Content  ///////////////////
            //set the displays content only if no content has been defined for the StbCamera
            if(!stbCamera->hasContent())
            {
                stbCamera->setContent(content.getValue());
            }
          }
    }
}


