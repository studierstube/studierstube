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
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoTransform.h>
#include <stb/components/event/event.h>
//#include <stb/components/event/SoTrackedArtifactKit.h>
#include <cstdio>

CREATE_COMPONENT_FUNC(SimpleApp)

using namespace stb;

SimpleApp::SimpleApp()
{
   isInit=false;
}

SimpleApp::~SimpleApp()
{
}

void 
SimpleApp::setParameter(stb::string key, std::string value)
{
    if(key=="sceneFile")
    {
        sceneFile=value;
    }
    //else if()
    //{
    //}
}


/// Called before the application is destructed.
bool 
SimpleApp::init()
{
    if(isInit)
        return true;
 
     //get viewer's parameter
     retrieveParameter();

    //need tracking --> check if componentmanager has loaded the event component.
    if(!Kernel::getInstance()->getComponentManager()->load("Viewer"))
    {
        return false;
    }
    Event* event=(Event*)(Kernel::getInstance()->getComponentManager()->load("Event"));
    if(!event)
    {
        return false;
    }


    //root = new SoSeparator();
    //SoTransform *tran=new SoTransform;
    //tran->translation.setValue(0.0,0.0,-6.0);
    //root->addChild(tran);
    //root->addChild(new SoSphere);
    //SoTrakEngine* tre= event->createSoTrakEngine();
    //if(!tre)
    //{
    //    isInit=false;
    //    return false;
    //}
    ////SoTrakEngine *tre=new SoTrakEngine;
    //tre->key.set1Value(0,"blabla");
    //tre->value.set1Value(0,"hi");
    //SoTransform *trans=new SoTransform;
    //trans->translation.connectFrom(&tre->translation);
    //trans->rotation.connectFrom(&tre->rotation);
    //SoSeparator *treRoot=new SoSeparator;
    //treRoot->addChild(trans);
    //treRoot->addChild(new SoCube);
    //root->addChild(treRoot);
    //// test SoTrackedArtifactKit
    //SoTrackedArtifactKit *trak=new SoTrackedArtifactKit;
    //trak->key.set1Value(0,"blabla");
    //trak->value.set1Value(0,"hi");

    //trak->setGeometry(new SoCone);
    //SoSeparator *trakRoot=new SoSeparator;
    //trakRoot->addChild(trak);
    //root->addChild(trakRoot);

    if(sceneFile.size()<1)
        return false;
    SoInput::addDirectoryFirst("./");
    SoInput myinput;
    if (!myinput.openFile(sceneFile.c_str())) {

        stb::Kernel::getInstance()->log("STB_ERROR: can not open file:" + sceneFile + "\n");
        SoInput::removeDirectory("./");
        return false;
    }
    root=SoDB::readAll(&myinput);
    myinput.closeFile();
    SoInput::removeDirectory("./");
    if (root==NULL) 
    {
        stb::Kernel::getInstance()->log("STB_ERROR: problem reading file: " + sceneFile + "\n");
        return false;
    }

    registerScene();
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
	Kernel::getInstance()->log("update simple app\n");
}

void 
SimpleApp::registerScene()
{
    stb::Kernel::getInstance()->getSceneManager()->registerApp(root);
}
