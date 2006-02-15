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
/** The header file for the Viewer class.
*
* @author Denis Kalkofen
*
* $Id: Viewer.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */
#include "Viewer.h"
#include "kernel/Kernel.h"
#include "kernel/SceneManager.h"
#include <iostream>
#include <Inventor/SoInput.h> 
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/actions/SoSearchAction.h>

#include "SoDisplay.h"
#include "SoStbCamera.h"
#include "SoOffAxisCamera.h"
#include "SoVideoBackground.h"

#include "controlmode/SoDesktopControlMode.h"
#include "controlmode/SoStbCameraControlMode.h"
#include "controlmode/MultRotRot.h"

#include "displaymode/SoGLPolygonStippleElement.h"
#include "displaymode/SoGLColorBufferElement.h"
#include "displaymode/SoDisplayMode.h"
#include "displaymode/SoAnaglyphDisplayMode.h"
#include "displaymode/SoLineSequentialDisplayMode.h"
#include "displaymode/SoFieldSequentialDisplayMode.h"

CREATE_COMPONENT_FUNC(Viewer)

BEGIN_NAMESPACE_STB

Viewer::Viewer()
{
   configFile="";
}

Viewer::~Viewer()
{
   //nil
}

/// Called before the application is destructed.
bool 
Viewer::init()
{
    // init coin stuff
    SoDisplay::initClass();

    SoOffAxisCamera::initClass();
    SoStbCamera::initClass();
    SoVideoBackground::initClass();

    SoGLPolygonStippleElement::initClass();
    SoGLColorBufferElement::initClass();
    SoDisplayMode::initClass();
    SoAnaglyphDisplayMode::initClass();
    SoLineSequentialDisplayMode::initClass();
    SoFieldSequentialDisplayMode::initClass();
    SoDesktopControlMode::initClass();
    SoStbCameraControlMode::initClass();
    MultRotRot::initClass();
    SoStbCameraControlMode::initClass();
    SoDesktopControlMode::initClass();

    //get viewer's parameter
    retrieveParameter();
    //load .iv file 
    std::cout<<"init Viewer\n";

    SoFieldSequentialDisplayMode::initClass();

    //get viewer's parameter
    retrieveParameter();
    //load .iv file 
    std::cout<<"init Viewer\n";

    SoInput myinput;
    SoInput::addDirectoryFirst("./");
    if (!myinput.openFile(configFile.c_str())) 
    {
        stb::Kernel::getInstance()->log("STB_ERROR: can not open file:" + configFile + "\n");
        SoInput::removeDirectory("./");
        return false;
    }
    SoSeparator *fileRoot=SoDB::readAll(&myinput);
    myinput.closeFile();
    SoInput::removeDirectory("./");
    if (fileRoot==NULL) 
    {
        stb::Kernel::getInstance()->log("STB_ERROR: problem reading file: " + configFile + "\n");
        return false;
    }
    fileRoot->ref();
    // search for SoDisplay nodes
    SoSearchAction sAction;
    sAction.setType(SoDisplay::getClassTypeId());
    sAction.setSearchingAll(TRUE);
    sAction.setInterest(SoSearchAction::ALL);
    sAction.apply(fileRoot);
    SoPathList paths = sAction.getPaths();
    for (int i=0;i<paths.getLength();i++)
    {
        if (paths[i]->getTail()->isOfType(SoDisplay::getClassTypeId()))
        {
            SoDisplay *display =(SoDisplay *)paths[i]->getTail();
            // add content to display
            display->setContent(stb::Kernel::getInstance()->getSceneManager()->getSceneRoot());
            stb::Kernel::getInstance()->getSceneManager()->setDisplay(display);
            // add display to kernel's scenemanager 
        }
    }
   fileRoot->unref();  

	return true;
}

void 
Viewer::setParameter(stb::string key, std::string value)
{
    if(key=="configFile")
    {
        configFile=value;
    }
    //else if()
    //{
    //}
}

/// Called before the application is destructed.
void 
Viewer::shutDown()
{

}

END_NAMESPACE_STB
//========================================================================
// End of Viewer.cxx
//========================================================================
// Local Variables:
// mode: c++
// c-basic-offset: 4
// eval: (c-set-offset 'substatement-open 0)
// eval: (c-set-offset 'case-label '+)
// eval: (c-set-offset 'statement 'c-lineup-runin-statements)
// eval: (setq indent-tabs-mode nil)
// End:
//========================================================================
