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
#include <stb/components/viewer/SoDisplay.h>

#include <stb/kernel/Studierstube.h>
#include <stb/kernel/Kernel.h>
#include <stb/components/viewer/Viewer.h>
#include <stb/components/viewer/guiDefines.h>
#include <stb/components/viewer/SoStudierstubeViewer.h>

#include <stb/components/viewer/SoStbCamera.h>
#include <stb/components/viewer/SoOffAxisCamera.h>
#include <stb/components/viewer/SoViewport.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include <iostream>

#include SOGUI_H
#include SOGUI_CURSOR_H

#ifdef USE_QUARTER
#  include <QDesktopWidget>
#endif

#include <Inventor/actions/SoSearchAction.h>


BEGIN_NAMESPACE_STB

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
    
    SO_NODE_ADD_FIELD(sceneGraph, (NULL));    
    //////// StudierstubeViewer Configurations
	//SO_NODE_ADD_FIELD(name, ("")); 
    SO_NODE_ADD_FIELD(xoffset, (0)); 
    SO_NODE_ADD_FIELD(yoffset, (0)); 
    SO_NODE_ADD_FIELD(width, (640)); 
    SO_NODE_ADD_FIELD(height, (480)); 
    SO_NODE_ADD_FIELD(fullscreen, (FALSE));
#ifdef USE_QUARTER
    SO_NODE_ADD_FIELD(screen, (0));
#endif
    SO_NODE_ADD_FIELD(transparencyType, (SoDisplay::BLEND)); 
    SO_NODE_ADD_FIELD(headlight, (TRUE)); 
    SO_NODE_ADD_FIELD(headlightIntensity, (1.0f)); 
    SO_NODE_ADD_FIELD(backgroundColor, (0,0,0)); 
    SO_NODE_ADD_FIELD(clearBackGround, (TRUE));
    SO_NODE_ADD_FIELD(showMouse, (TRUE));
    SO_NODE_ADD_FIELD(windowBorder, (TRUE));
    SO_NODE_ADD_FIELD(decoration, (TRUE));
    SO_NODE_ADD_FIELD(windowOnTop, (FALSE));
    SO_NODE_ADD_FIELD(stencilBuffer, (FALSE));
    SO_NODE_ADD_FIELD(useRefCamera, (TRUE));
    SO_NODE_ADD_FIELD(isViewing,(TRUE));
    SO_NODE_ADD_FIELD(showFrameRate,(FALSE));
    SO_NODE_ADD_FIELD(showTriangleCount,(FALSE));
	SO_NODE_ADD_FIELD(quadBuffering,(FALSE));
	SO_NODE_ADD_FIELD(antialiasingLevel,(1));
	SO_NODE_ADD_FIELD(sampleBuffers,(1));

    
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

    examCam=NULL;


	#ifndef USE_QUARTER
	backgroundSensor = NULL; 
	quadBufferingChangedSensor = NULL; 
	transparencyTypeChangedSensor = NULL; 
	xOffsetChangedSensor = NULL; 
	yOffsetChangedSensor = NULL; 
	widthChangedSensor =  NULL;
	heightChangedSensor =  NULL;
	fullscreenChangedSensor =  NULL; 
	headLightOnChangedSensor =  NULL; 
	headlightIntensityChangedSensor =  NULL; 
	clearBackgroundChangedSensor =  NULL; 
	showMouseChangedSensor = NULL; 
	windowBorderChangedSensor =  NULL; 
	decorationChangedSensor =  NULL; 
	windowOnTopChangedSensor =  NULL; 
	useRefCameraChangedSensor =  NULL; 
	stencilBufferChangedSensor =  NULL; 
	isViewingChangedSensor =  NULL; 
	showFramerateChangedSensor =  NULL; 
	showTriangleCountChangedSensor =  NULL; 
	antiAliasingLevelChangedSensor =  NULL; 
	sampleBuffersChangedSensor =  NULL; 
#endif
}

//----------------------------------------------------------------------------
/** The destructor */
SoDisplay::~SoDisplay()
{
    if(viewer)
    {
        viewer->hide();
        delete viewer;

	#ifndef USE_QUARTER
		delete backgroundSensor;
		delete quadBufferingChangedSensor;
		delete transparencyTypeChangedSensor;
		delete xOffsetChangedSensor;
		delete yOffsetChangedSensor;
		delete widthChangedSensor;
		delete heightChangedSensor;
		delete fullscreenChangedSensor;
		delete headLightOnChangedSensor;
		delete headlightIntensityChangedSensor;
		delete clearBackgroundChangedSensor;
		delete showMouseChangedSensor;
		delete windowBorderChangedSensor;
		delete decorationChangedSensor;
		delete windowOnTopChangedSensor;
		delete useRefCameraChangedSensor;
		delete stencilBufferChangedSensor;
		delete isViewingChangedSensor;
		delete showFramerateChangedSensor;
		delete showTriangleCountChangedSensor;
		delete antiAliasingLevelChangedSensor;
		delete sampleBuffersChangedSensor;

		backgroundSensor = NULL; 
		quadBufferingChangedSensor = NULL; 
		transparencyTypeChangedSensor = NULL; 
		xOffsetChangedSensor = NULL; 
		yOffsetChangedSensor = NULL; 
		widthChangedSensor =  NULL;
		heightChangedSensor =  NULL;
		fullscreenChangedSensor =  NULL; 
		headLightOnChangedSensor =  NULL; 
		headlightIntensityChangedSensor =  NULL; 
		clearBackgroundChangedSensor =  NULL; 
		showMouseChangedSensor = NULL; 
		windowBorderChangedSensor =  NULL; 
		decorationChangedSensor =  NULL; 
		windowOnTopChangedSensor =  NULL; 
		useRefCameraChangedSensor =  NULL; 
		stencilBufferChangedSensor =  NULL; 
		isViewingChangedSensor =  NULL; 
		showFramerateChangedSensor =  NULL; 
		showTriangleCountChangedSensor =  NULL; 
		antiAliasingLevelChangedSensor =  NULL; 
		sampleBuffersChangedSensor =  NULL; 
	#endif
    }
}

//----------------------------------------------------------------------------
//
void 
SoDisplay::exitViewer(void*, SoGuiComponent* viewer)
{
	// This is a cleaner way to exit. Mendez
	Kernel::getInstance()->stop();

    //SoDisplay* display=Viewer::findSoDisplay(((SoStudierstubeViewer*)viewer)->getSceneGraph());
    //Viewer::removeSoDisplay(display);
    //    delete viewer;
}


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
    displayRoot=new SoSeparator();
    this->addChild(displayRoot);

	// set this displays name
	//this->setName( SbName(name.getValue()) );

	// on wince, the viewport is (for whatever reason) not correctly set up -- 20060914 flo
#ifdef STB_IS_WINCE
	SoViewport * viewport = new SoViewport();
	viewport->setViewportRegion(SbVec2s(0,0),SbVec2s(640,480));
	displayRoot->addChild(viewport);
#endif

    if(useRefCamera.getValue()){
        examCam= new SoPerspectiveCamera();
        displayRoot->addChild(examCam);
		
    }
    if(sceneGraph.getValue()){
		displayRoot->addChild(sceneGraph.getValue());
    }

    ////////////////////////////////////////
    // create StudierstubeViewer
    ////////////////////////////////////////
#ifdef USE_QUARTER
    QDesktopWidget dwidget;
    viewer = new SoStudierstubeViewer(dwidget.screen(screen.getValue()));
    printf("GL Context: %x\n",viewer->context());
#else
    
    viewer=new SoStudierstubeViewer(NULL);
#endif
    
    if (quadBuffering.getValue())
    {
#ifndef USE_QUARTER
        viewer->setQuadBufferStereo(true);
#else
        // nothing to do here -> do it in the display modes directly ...
        //viewer->getSoRenderManager()->setStereoMode(SoRenderManager::QUAD_BUFFER);
        //viewer->setStereoMode(SIM::Coin3D::Quarter::QuarterWidget::MONO);
#endif
    }        
    ///////////////////////////////////////////////////
    // add all the stbCameras to the viewer's root node
    // for (int i=0;i<stbCameraList.getNum();i++)
    // {
    //    if(stbCameraList[i]!=NULL 
    //    && stbCameraList[i]->isOfType(SoStbCamera::getClassTypeId()))
    //    {
    //     SoStbCamera *stbCamera=(SoStbCamera *)stbCameraList[i];
    //     //Set reference camera's parameter to match with the OffAxisCamera of the first StbCamera 
    //     if(	i==0 
    //      && stbCamera->getCamera()
    //      && stbCamera->getCamera()->isOfType(SoOffAxisCamera::getClassTypeId())
    //      )
    //     {
    //      SoPerspectiveCamera *refCam = (SoPerspectiveCamera*)examCam;
    //      SoOffAxisCamera *cam=(SoOffAxisCamera*)stbCamera->getCamera();
    //      refCam->aspectRatio		=	cam->size.getValue()[0]/cam->size.getValue()[1];
    //      refCam->focalDistance	=	cam->focalDistance;
    //      refCam->nearDistance	=	cam->nearDistance;
    //      refCam->farDistance		=	cam->farDistance;
    //      refCam->viewportMapping =	cam->viewportMapping;
    //      refCam->orientation		=	cam->orientation;
    //     }

    //     ///////////////////////////////////////////////////////////////////////////////////////
    //   ///////////// Tell the StbCamera about SoDisplay's reference camera  ///////////////////
    //     stbCamera->setSoDisplay(this);
    //     stbCamera->setReferenceCamera(examCam);

    //     ///////////////////////////////////////////////
    //     ///////////// Set Content  ///////////////////
    //     //set the displays content only if no content has been defined for the StbCamera
    //     if(!stbCamera->hasContent())
    //     {
    //      stbCamera->setContent(content.getValue());
    //     }
    //        stbCamera->activateControlMode();
    //        stbCamera->activateDisplayMode(viewer);
    //     ///////////////////////////////////////////////////////////
    //     //////////// Add StbCamera to StudierstubeViewer's root ////
    //     displayRoot->addChild(stbCamera);	
    //    }
    //}
  
    //////////////////////////////////////////
    ////  Configure  StudierstubeViewer 
    //////////////////////////////////////////

	//windowBorder
	//viewer->setWindowDecoration(windowBorder.getValue());
	//if (!windowBorder.getValue())
	//	flags|=Qt::FramelessWindowHint;

	//set chosen flags for qt window
	//mainWin->setWindowFlags(flags);

    ////windowDecoreation
#ifndef USE_QUARTER
    viewer->setWindowDecoration(windowBorder.getValue());
    if( viewer->isDecoration() != decoration.getValue())
        viewer->setDecoration( decoration.getValue());
#else
    if (decoration.getValue())
    {
        viewer->setWindowFlags(Qt::Window);
    }
    else
    {
        viewer->setWindowFlags(Qt::SplashScreen);   
    }
#endif

	//widownOnTop
	viewer->setWindowOnTop(windowOnTop.getValue());

#ifndef USE_QUARTER	
	//antialiasing
	viewer->setAntialiasing(antialiasingLevel.getValue());

	//samplebuffers
	//viewer->setSampleBuffers(sampleBuffers.getValue());

    //stencilBuffer
    if (stencilBuffer.getValue() == TRUE)
    {
        viewer->setStencilBuffer(TRUE);
    }
#endif

    //window pos & size
#ifndef USE_QUARTER
    viewer->setWindowPosSize(xoffset.getValue(), yoffset.getValue(),
                             width.getValue()  , height.getValue());
#else
    if (fullscreen.getValue())
    {
        viewer->showFullScreen();
    }
    else
    {
        viewer->setWindowPosSize(xoffset.getValue(), yoffset.getValue(),
                                 width.getValue()  , height.getValue());
    }
#endif
   
    //transparency
#ifndef USE_QUARTER
    viewer->setTransparencyType(
                                (SoGLRenderAction::TransparencyType)transparencyType.getValue());
#else
    viewer->setTransparencyType(
                                (SIM::Coin3D::Quarter::QuarterWidget::TransparencyType)transparencyType.getValue());
#endif
    //title
#ifndef USE_QUARTER	
    viewer->setTitle(STUDIERSTUBE_VERSION_STRING);
#endif

    // scene lighting
    //viewer->setHeadlight(headlight.getValue());
    //if (viewer->getHeadlight())
    // viewer->getHeadlight()->intensity.setValue(headlightIntensity.getValue());
       
#ifndef USE_QUARTER
    //background color 
    viewer->setBackgroundColor(backgroundColor.getValue());

    //clearing background on/off
    viewer->setClearBeforeRender(clearBackGround.getValue());
#endif

    // set mouse cursor
    if( showMouse.getValue() == FALSE )
    {
#ifndef USE_QUARTER	
        viewer->setComponentCursor(SoGuiCursor::getBlankCursor());
#else
        printf("viewer->setComponentCursor(SoGuiCursor::getBlankCursor()); not implemented in Quarter\n");
#endif
        viewer->showMouse(false);
    }

    // Set the Viewing
#ifndef USE_QUARTER	
    if(!isViewing.getValue())
        viewer->setViewing(isViewing.getValue());
#endif
    
    //////////////////////////////////////////
    //// start StudierstubeViewer
    //// set viewer's root node and show the studierstubeViewer
    //////////////////////////////////////////
    viewer->setSceneGraph(displayRoot);

    viewer->printTriangles(showTriangleCount.getValue() == TRUE);
    viewer->printFrameRate(showFrameRate.getValue() == TRUE);

	//SoCamera *camera = myViewer->getCamera();

	//Changes by Felix Nairz (nairz@sbox.tugraz.at)
	//The following Sensors make a modification of the viewer fields during run-time possible. 
	#ifndef USE_QUARTER
	backgroundSensor = new SoFieldSensor(backgroundColorChangedCB, this);
	backgroundSensor->attach(&this->backgroundColor);

	quadBufferingChangedSensor = new SoFieldSensor(quadBufferingChangedCB, this); 
	quadBufferingChangedSensor->attach(&this->quadBuffering); 

	 transparencyTypeChangedSensor = new SoFieldSensor(TransparencyTypeChangedCB, this); 
	transparencyTypeChangedSensor->attach(&this->transparencyType); 

	 xOffsetChangedSensor = new SoFieldSensor(xoffsetChangedCB, this); 
	xOffsetChangedSensor->attach(&this->xoffset); 


	 yOffsetChangedSensor = new SoFieldSensor(yoffsetChangedCB, this); 
	yOffsetChangedSensor->attach(&this->yoffset); 

	 widthChangedSensor = new SoFieldSensor(widthChangedCB, this); 
	widthChangedSensor->attach(&this->width); 

	 heightChangedSensor = new SoFieldSensor(heightChangedCB, this); 
	heightChangedSensor->attach(&this->height); 

	 fullscreenChangedSensor = new SoFieldSensor(fullscreenChangedCB, this); 
	fullscreenChangedSensor->attach(&this->fullscreen); 

	 headLightOnChangedSensor = new SoFieldSensor(headLightOnChangedCB, this); 
	headLightOnChangedSensor->attach(&this->headlight); 

	 headlightIntensityChangedSensor = new SoFieldSensor(headLightIntensityChangedCB, this); 
	headlightIntensityChangedSensor->attach(&this->headlightIntensity); 

	 clearBackgroundChangedSensor = new SoFieldSensor(clearBackgroundChangedCB, this); 
	clearBackgroundChangedSensor->attach(&this->clearBackGround); 

	 showMouseChangedSensor = new SoFieldSensor(showMouseChangedCB, this); 
	showMouseChangedSensor->attach(&this->showMouse); 

	 windowBorderChangedSensor = new SoFieldSensor(windowBorderChangedCB, this); 
	windowBorderChangedSensor->attach(&this->windowBorder); 

	 decorationChangedSensor = new SoFieldSensor(decorationChangedCB, this); 
	decorationChangedSensor->attach(&this->decoration); 

	 windowOnTopChangedSensor = new SoFieldSensor(windowOnTopChangedCB, this); 
	windowOnTopChangedSensor->attach(&this->windowOnTop); 


	 useRefCameraChangedSensor = new SoFieldSensor(useRefCameraChangedCB, this); 
	useRefCameraChangedSensor->attach(&this->useRefCamera); 

	 stencilBufferChangedSensor = new SoFieldSensor(stencilBufferChangedCB, this); 
	stencilBufferChangedSensor->attach(&this->stencilBuffer); 

	 isViewingChangedSensor = new SoFieldSensor(isViewingChangedCB, this); 
	isViewingChangedSensor->attach(&this->isViewing); 

	 showFramerateChangedSensor = new SoFieldSensor(showFramerateChangedCB, this); 
	showFramerateChangedSensor->attach(&this->showFrameRate); 

	 showTriangleCountChangedSensor = new SoFieldSensor(showTriangleCountChangedCB, this); 
	showTriangleCountChangedSensor->attach(&this->showTriangleCount); 

	 antiAliasingLevelChangedSensor = new SoFieldSensor(antiAliasingLevelChangedCB, this); 
	antiAliasingLevelChangedSensor->attach(&this->antialiasingLevel); 

	 sampleBuffersChangedSensor = new SoFieldSensor(sampleBuffersChangedCB, this); 
	sampleBuffersChangedSensor->attach(&this->sampleBuffers); 
#endif
	







#ifndef USE_QUARTER	
    viewer->setWindowCloseCallback(exitViewer, NULL);
#endif
    viewer->show();
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


SoPerspectiveCamera* 
SoDisplay::getReferenceCamera()
{
    return examCam;
}


bool
SoDisplay::find(SoNode *node)
{
    SoSearchAction sAction;
    sAction.reset();
    sAction.SoSearchAction::setNode(node);
    sAction.setSearchingAll(TRUE);
    sAction.apply(displayRoot);
    SoPath *path = sAction.getPath();
    if(path==NULL){
        return false;
    }  
    return true;
}

SoNode*
SoDisplay::findType(const SoType type)
{
	SoSearchAction sAction;
	sAction.reset();
	sAction.setType(type);
	sAction.setSearchingAll(TRUE);
	sAction.apply( ((SoStudierstubeViewer*)viewer)->getSceneGraph() );
	SoPath *path = sAction.getPath();
	if(path!=NULL){
		return sAction.getNode();
	}  
	return false;
}

#ifndef USE_QUARTER
void SoDisplay::backgroundColorChangedCB( void* data, SoSensor* )
{
	SoDisplay *display = (SoDisplay*)data;
	SbColor background = display->backgroundColor.getValue(); 
	std::cout << "backgroundColorChangedCB called, new values are: " << background[0] << background[1] << background[2] << std::endl; 
	display->viewer->setBackgroundColor(background);
}

void SoDisplay::quadBufferingChangedCB( void* data, SoSensor* )
{
	std::cout <<"quadBufferChangedCB called" << std::endl; 
	SoDisplay *display = (SoDisplay*)data;
	SbBool quadBuffer = display->quadBuffering.getValue(); 
	display->viewer->setQuadBufferStereo(quadBuffer); 

}

void SoDisplay::TransparencyTypeChangedCB( void* data, SoSensor* )
{
	std::cout <<"TransparencyTypeChangedCB called" << std::endl; 
	SoDisplay *display = (SoDisplay*)data;
	display->viewer->setTransparencyType(
		(SoGLRenderAction::TransparencyType)display->transparencyType.getValue());

}

void SoDisplay::xoffsetChangedCB( void* data, SoSensor* )
{
	std::cout <<"xoffsetChangedCB called" << std::endl; 
	SoDisplay *display = (SoDisplay*)data;
	display->viewer->setWindowPosSize(display->xoffset.getValue(), display->yoffset.getValue(),
		display->width.getValue()  , display->height.getValue());

}

void SoDisplay::yoffsetChangedCB( void* data, SoSensor* )
{
	std::cout <<"yoffsetChangedCB called" << std::endl; 
	SoDisplay *display = (SoDisplay*)data;
	display->viewer->setWindowPosSize(display->xoffset.getValue(), display->yoffset.getValue(),
		display->width.getValue()  , display->height.getValue());
}

void SoDisplay::widthChangedCB( void* data, SoSensor* )
{
	std::cout <<"widthChangedCB called" << std::endl; 
	SoDisplay *display = (SoDisplay*)data;
	display->viewer->setWindowPosSize(display->xoffset.getValue(), display->yoffset.getValue(),
		display->width.getValue()  , display->height.getValue());
}

void SoDisplay::heightChangedCB( void* data, SoSensor* )
{
	std::cout <<"heightChangedCB called" << std::endl; 
	SoDisplay *display = (SoDisplay*)data;
	display->viewer->setWindowPosSize(display->xoffset.getValue(), display->yoffset.getValue(),
		display->width.getValue()  , display->height.getValue());
}

void SoDisplay::fullscreenChangedCB( void* data, SoSensor* )
{
	std::cout <<"fullscreenChangedCB called" << std::endl; 
	std::cout << "This mode is not supported by SoQT or not implemented by the SoDisplay class" << std::endl; 

}

void SoDisplay::headLightOnChangedCB( void* data, SoSensor* )
{
	std::cout <<"headLightOnChangedCB called" << std::endl; 
	std::cout << "This variable is not implemented in the SoDisplay class" << std::endl; 

}

void SoDisplay::headLightIntensityChangedCB( void* data, SoSensor* )
{
	std::cout <<"headLightIntensityChangedCB called" << std::endl; 
	std::cout << "This variable is not implemented in the SoDisplay class" << std::endl; 

}

void SoDisplay::clearBackgroundChangedCB( void* data, SoSensor* )
{
	std::cout <<"clearBackgroundChangedCB called" << std::endl; 
	SoDisplay *display = (SoDisplay*)data;
	display->viewer->setClearBeforeRender(display->clearBackGround.getValue());
}

void SoDisplay::showMouseChangedCB( void* data, SoSensor* )
{
	std::cout <<"showMouseChangedCB called" << std::endl; 
	SoDisplay *display = (SoDisplay*)data;
	if(display->showMouse.getValue() == false){
		display->viewer->setComponentCursor(SoGuiCursor::getBlankCursor());
	}
	else{
		display->viewer->setComponentCursor(SoGuiCursor::DEFAULT); 
	}
}

void SoDisplay::windowBorderChangedCB( void* data, SoSensor* )
{
	std::cout <<"windowBorderChangedCB called" << std::endl; 
	SoDisplay *display = (SoDisplay*)data;

	display->viewer->setWindowDecoration(display->windowBorder.getValue());
	if( display->viewer->isDecoration() != display->decoration.getValue())
		display->viewer->setDecoration( display->decoration.getValue());

}

void SoDisplay::decorationChangedCB( void* data, SoSensor* )
{
	std::cout <<"windowBorderChangedCB called" << std::endl; 
	SoDisplay *display = (SoDisplay*)data;
	display->viewer->setWindowDecoration(display->windowBorder.getValue());
	if( display->viewer->isDecoration() != display->decoration.getValue())
		display->viewer->setDecoration( display->decoration.getValue());

}

void SoDisplay::windowOnTopChangedCB( void* data, SoSensor* )
{
	std::cout <<"windowOnTopChangedCB called" << std::endl; 
	SoDisplay *display = (SoDisplay*)data;
	display->viewer->setWindowOnTop(display->windowOnTop.getValue());

}

void SoDisplay::stencilBufferChangedCB( void* data, SoSensor* )
{
	std::cout <<"stencilBufferChangedCB called" << std::endl; 
	SoDisplay *display = (SoDisplay*)data;
	display->viewer->setStencilBuffer(display->stencilBuffer.getValue());

}

void SoDisplay::useRefCameraChangedCB( void* data, SoSensor* )
{
	std::cout <<"useRefCameraChangedCB called" << std::endl; 
	SoDisplay *display = (SoDisplay*)data;

	if(display->useRefCamera.getValue()){
		if(display->examCam == NULL )
			display->examCam= new SoPerspectiveCamera();
		display->displayRoot->insertChild(display->examCam, 0);
	}
	else{
		display->displayRoot->removeChild(display->examCam);
	}

}

void SoDisplay::isViewingChangedCB( void* data, SoSensor* )
{
	std::cout <<"isViewingChangedCB called" << std::endl; 
	SoDisplay *display = (SoDisplay*)data;
	display->viewer->setViewing(display->isViewing.getValue());

}

void SoDisplay::showFramerateChangedCB( void* data, SoSensor* )
{
	std::cout <<"showFramerateChangedCB called" << std::endl; 
	SoDisplay *display = (SoDisplay*)data;
	display->viewer->printFrameRate(display->showFrameRate.getValue() == TRUE);
	
}

void SoDisplay::showTriangleCountChangedCB( void* data, SoSensor* )
{
	std::cout <<"showTriangleCountChangedCB called" << std::endl; 
	SoDisplay *display = (SoDisplay*)data;
	display->viewer->printTriangles(display->showTriangleCount.getValue() == TRUE);
}

void SoDisplay::antiAliasingLevelChangedCB( void* data, SoSensor* )
{
	std::cout <<"antiAliasingLevelChangedCB called" << std::endl; 
	SoDisplay *display = (SoDisplay*)data;
	display->viewer->setAntialiasing(display->antialiasingLevel.getValue());
}

void SoDisplay::sampleBuffersChangedCB( void* data, SoSensor* )
{
	std::cout <<"sampleBuffersChangedCB called" << std::endl; 
	std::cout << "This variable is not implemented in the SoDisplay class" << std::endl; 

}
#endif
//void
//SoDisplay::findStbCameras(SoPathList & list)
//{
//	SoType testType = SoStbCamera::getClassTypeId();
//	SoType testType2 = SoDisplay::getClassTypeId();
//	
//	SoSearchAction sAction;
//	sAction.reset();
//	sAction.setType(SoStbCamera::getClassTypeId());
//	sAction.setSearchingAll(TRUE);
//	sAction.apply( ((SoStudierstubeViewer*)viewer)->getSceneGraph() );
//	list = sAction.getPaths();
//}


END_NAMESPACE_STB

//----------------------------------------------------------------------
// End of SoDisplay.cxx
//----------------------------------------------------------------------
// Local Variables:
// mode: c++
// c-basic-offset: 4
// eval: (c-set-offset 'substatement-open 0)
// eval: (c-set-offset 'case-label '+)
// eval: (c-set-offset 'statement 'c-lineup-runin-statements)
// eval: (setq indent-tabs-mode nil)
// End:
//----------------------------------------------------------------------
