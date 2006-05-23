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
#include <stb/components/viewer/Viewer.h>
#include <stb/base/macros.h>
#include <stb/base/OS.h>

#include <stb/kernel/Kernel.h>
#include <stb/kernel/SceneManager.h>

#include <iostream>
#include <sstream>

#include <Inventor/SoInput.h> 
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/nodes/SoNode.h>

#include <stb/components/viewer/SoDisplay.h>
#include <stb/components/viewer/SoStbCamera.h>
#include <stb/components/viewer/SoOffAxisCamera.h>
#include <stb/components/viewer/SoVideoBackground.h>
#include <stb/components/viewer/SoViewport.h>


#include <stb/components/viewer/controlmode/SoDesktopControlMode.h>
#include <stb/components/viewer/controlmode/SoStbCameraControlMode.h>
#include <stb/components/viewer/controlmode/SoTrackedDisplayControlMode.h>
#include <stb/components/viewer/controlmode/SoTrackedViewpointControlMode.h>
#include <stb/components/viewer/controlmode/SoTrackedViewpointMobileDisplayControlMode.h>
#include <stb/components/viewer/controlmode/MultRotRot.h>

#include <stb/components/viewer/displaymode/SoGLPolygonStippleElement.h>
#include <stb/components/viewer/displaymode/SoGLColorBufferElement.h>
#include <stb/components/viewer/displaymode/SoDisplayMode.h>
#include <stb/components/viewer/displaymode/SoAnaglyphDisplayMode.h>
#include <stb/components/viewer/displaymode/SoLineSequentialDisplayMode.h>
#include <stb/components/viewer/displaymode/SoFieldSequentialDisplayMode.h>



#ifdef USE_SOWIN
#include <Inventor/Win/SoWin.h>
#elif USE_SOSIMPLE
#  include "../gui/SoSimple/public.h"
#elif USE_SOQT
#include <Inventor/Qt/SoQt.h>
#else
#  pragma error("unkown GUI binding")
#endif


BEGIN_NAMESPACE_STB

std::vector<SoDisplay*> Viewer::displayList;

CREATE_COMPONENT_FUNC(Viewer)


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
    //FIXME: insert log message as soon as the logger is done      
//    Kernel::getInstance()->logDebug("init Viewer\n");

    if(isInit)
        return true;
    
    // init coin stuff
    SoDisplay::initClass();
    SoOffAxisCamera::initClass();
    SoStbCamera::initClass();
    SoStbCameraControlMode::initClass();
    MultRotRot::initClass();
    SoDesktopControlMode::initClass();

    SoTrackedDisplayControlMode::initClass();
    SoTrackedViewpointControlMode::initClass();
    SoTrackedViewpointMobileDisplayControlMode::initClass();

    SoDisplayMode::initClass();
    SoGLColorBufferElement::initClass();
    SoGLPolygonStippleElement::initClass();
    SoLineSequentialDisplayMode::initClass();
    SoFieldSequentialDisplayMode::initClass();
    SoAnaglyphDisplayMode::initClass();

    SoViewport::initClass();
    SoVideoBackground::initClass();

    //get viewer's parameter
    retrieveParameter();
 
#ifdef STB_IS_LINUX
    using namespace std;
    SoInput::addDirectoryFirst(Kernel::getInstance()->getBaseDir().c_str());
#endif
    SoInput::addDirectoryFirst("./");

    SoInput myinput;
    if (!myinput.openFile(configFile.c_str())) {
		logPrintE("can not open file:" + configFile + "\n");
        SoInput::removeDirectory("./");
        return false;
    }
    SoSeparator *fileRoot=SoDB::readAll(&myinput);
    myinput.closeFile();
    SoInput::removeDirectory("./");
    if (fileRoot==NULL) 
    {
		logPrintE("problem reading file: " + configFile + "\n");
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
            addSoDisplay(display);
            Kernel::getInstance()->getSceneManager()->setDisplay(display);
            // add display to kernel's scenemanager 
        }
    }
   fileRoot->unref();  

   isInit=true;
   return isInit;
}

void 
Viewer::setParameter(string key, std::string value)
{
    if(key=="configFile")
    {
        configFile=value;
    }
    //else if()
    //{
    //}
}

void
Viewer::addSoDisplay(SoDisplay* dsp)
{
    displayList.push_back(dsp);
}

void
Viewer::removeSoDisplay(SoDisplay*)
{
    //displayList. (dsp);
}

SoDisplay* 
Viewer::findSoDisplay(SoNode* node)
{
    for(int i=0;i<(int)displayList.size();i++)
    {
        if(displayList[i]->find(node))
        {
            return displayList[i];
        }
    }
    return NULL; 
}

/// Called before the application is destructed.
void 
Viewer::shutDown()
{

}


void
Viewer::gb_init(const char*appname)
{
#ifdef USE_SOWIN
	SoWin::init(appname, "SoWin");
#elif USE_SOSIMPLE
	SoSimple_init(appname, "SoSimple");
#elif USE_SOQT
	SoQt::init(appname, "SoQt");
#endif
}


void
Viewer::gb_mainloop()
{
#ifdef USE_SOWIN
	SoWin::mainLoop();
#elif USE_SOSIMPLE
	SoSimple_mainLoop();
#elif USE_SOQT
	SoQt::mainLoop();
#endif

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
