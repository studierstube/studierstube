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
* $Id: SimpleApp.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#include "SimpleApp.h"
#include <stb/kernel/Kernel.h>
#include <stb/kernel/ComponentManager.h>
#include <stb/kernel/SceneManager.h>
#include <Inventor/nodes/SoCube.h>

#include <cstdio>

CREATE_COMPONENT_FUNC(SimpleApp)

using namespace stb;

SimpleApp::SimpleApp()
{
   //nil
}

SimpleApp::~SimpleApp()
{
      printf("delete SimpleApp\n");
}

/// Called before the application is destructed.
bool 
SimpleApp::init()
{
    printf("init SimpleApp\n");
    if(isInit)
        return true;
    //need tracking --> check if componentmanager has loaded the event component.
    if(!Kernel::getInstance()->getComponentManager()->load("Viewer"))
        return false;


    root = new SoSeparator();
    
    registerScene();

    root->addChild(new SoCube());

    isInit=true;
	return isInit;
}

/// Called before the application is destructed.
void 
SimpleApp::shutDown()
{
    root->unref();
}


void 
SimpleApp::update()
{
	printf("update simple app\n");
}

void 
SimpleApp::registerScene()
{
    stb::Kernel::getInstance()->getSceneManager()->registerApp(root);
}
