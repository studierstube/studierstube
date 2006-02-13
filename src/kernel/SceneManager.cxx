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
/* @author Denis Kalkofen
*
* $Id: SceneManager.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#include "SceneManager.h"
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/SbName.h>
#include <Inventor/nodes/SoNode.h>

BEGIN_NAMESPACE_STB

SceneManager::SceneManager()
{
   root=new SoSeparator();

   appRoot=new SoSeparator();
   trackRoot=new SoSeparator();
   displayRoot=new SoSeparator();

   //set up the scene graph
   //
   //              root
   //               |
   //       |--------------------------------|
   //    appRoot                        displayRoot	
   //       |                                |
   //  |-----------|-----|                 SoDisplay
   // trackRoot   
   //
   root->ref();
   root->addChild(appRoot);
   appRoot->addChild(trackRoot);
   root->addChild(displayRoot);
   displayRoot->addChild(new SoSeparator); 
   touchRoot=root;
}

SceneManager::~SceneManager()
{
   //nil
}

void
SceneManager::update()
{
	touchRoot->touch();
}

void 
SceneManager::registerApp(SoSeparator *appRoot)
{
	
}
SoSeparator* 
SceneManager::getDisplayRoot()
{
	return displayRoot;
}

SoSeparator* 
SceneManager::getAppRoot()
{
	return appRoot;
}

void 
SceneManager::setDisplay(SoNode *display)
{
    displayRoot->replaceChild(0,display);
    setTouchRoot(DISPLAY);
}

void
SceneManager::setTouchRoot(TOUCHROOT touchNode)
{
	switch(touchNode)
	{
	case ROOT:
		touchRoot=root;
		break;
	case APP:
		touchRoot=appRoot;
		break;
	case DISPLAY:
		touchRoot=displayRoot;
		break;

	}
}

END_NAMESPACE_STB