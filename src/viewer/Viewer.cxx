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
/** The header file for the StbViewer class.
*
* @author Denis Kalkofen
*
* $Id: StbViewer.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#include "StbViewer.h"
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <Inventor/nodes/SoNode.h>
#include "SoDisplay.h"

#include "StbViewer/SoStbCamera.h"
#include "StbViewer/SoOffAxisCamera.h"
#include "StbViewer/SoViewport.h"
#include "StbViewer/StbCameraControlMode/SoStbCameraControlMode.h"
#include "StbViewer/StbCameraControlMode/SoDesktopControlMode.h"
#include "StbViewer/StbCameraControlMode/MultRotRot.h"
#include "StbViewer/StbCameraControlMode/SoTrackedDisplayControlMode.h"
#include "StbViewer/StbCameraControlMode/SoTrackedViewpointControlMode.h"
#include "StbViewer/StbCameraControlMode/SoTrackedViewpointMobileDisplayControlMode.h"

#include "StbViewer/DisplayMode/SoGLPolygonStippleElement.h"
#include "StbViewer/DisplayMode/SoGLColorBufferElement.h"
#include "StbViewer/DisplayMode/SoDisplayMode.h"
#include "StbViewer/DisplayMode/SoAnaglyphDisplayMode.h"
#include "StbViewer/DisplayMode/SoLineSequentialDisplayMode.h"
#include "StbViewer/DisplayMode/SoFieldSequentialDisplayMode.h"

#include "StbViewer/Video/SoVideoBackgroundOV.h"


StbViewer* StbViewer::instance=NULL;

void 
StbViewer::init()
{
	if(StbViewer::instance)
		return ;

	// Inventor inits
	SoDisplay::initClass();
	SoOffAxisCamera::initClass();
	SoStbCamera::initClass();
	SoStbCameraControlMode::initClass();
	MultRotRot::initClass();
	SoDesktopControlMode::initClass();
	SoTrackedDisplayControlMode::initClass();
	SoTrackedViewpointControlMode::initClass();
	SoTrackedViewpointMobileDisplayControlMode::initClass();

	SoGLColorBufferElement::initClass();
	SoGLPolygonStippleElement::initClass();
	SoDisplayMode::initClass();
	SoLineSequentialDisplayMode::initClass();
	SoFieldSequentialDisplayMode::initClass();
	SoAnaglyphDisplayMode::initClass();
	SoViewport::initClass();
	SoVideoBackgroundOV::initClass();


	StbViewer::instance=new StbViewer();
}

void 
StbViewer::fin()
{
	if(!StbViewer::instance)
		return ;
	
	StbViewer::instance->soDisplays.clear();
	
	delete StbViewer::instance;
	StbViewer::instance=NULL;
}

void 
StbViewer::addSoDisplay(SoDisplay* aSoDisplay)
{
	if(!StbViewer::instance)
		return ;
	StbViewer* self=StbViewer::instance;

	self->soDisplays.push_back(aSoDisplay);
}

void 
StbViewer::removeSoDisplay(SoDisplay* aSoDisplay)
{
	if(!StbViewer::instance)
		return ;

	StbViewer* self=StbViewer::instance;
	for(std::vector<SoDisplay*>::iterator it=self->soDisplays.begin();it!=self->soDisplays.end();it++)
	{
		if(aSoDisplay==(*it))
		{
			self->soDisplays.erase(it);
			break;
		}
	}
}

