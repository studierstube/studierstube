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
/** The cxx file for the StbKernel class.
 *
 * @author Denis Kalkofen
 *
 * $Id: Kernel.cxx 25 2005-11-28 16:11:59Z denis $
 * @file                                                                   */
/* ======================================================================= */

#include <stb/kernel/Kernel.h>
#include <stb/kernel/Studierstube.h>
#include <stb/base/string.h>
#include <stdarg.h>
#include <stdio.h>
#include <tinyxml/tinyxml.h>
#include <stb/base/fixWin32Issues.h>
#include <ace/ACE.h>
#include <Inventor/SoDB.h> 
#include <Inventor/sensors/SoSensor.h>
//
#include <stb/kernel/Config.h>
#include <stb/kernel/Scheduler.h>
#include <stb/kernel/SceneManager.h>
#include <stb/kernel/ComponentManager.h>
#include <stb/kernel/ComponentInfo.h>
#include <stb/kernel/SoStbScene.h>

#include <fstream>
#include <sstream>
#include <iostream>

//
BEGIN_NAMESPACE_STB

Kernel*	Kernel::instance=NULL;


Kernel::Kernel()
{
    ACE::init();
    SoDB::init();
    //additional coin nodes
    SoStbScene::initClass();
    //
    scheduler= new stb::Scheduler();
    sceneManager= new stb::SceneManager();
    componentManager= new stb::ComponentManager();
    config=new stb::Config();
    //
    kernel_config_file = "kernel.xml"; //default kernel config
}

Kernel::~Kernel()
{
    ACE::fini();
    delete config;
    delete scheduler;
    delete sceneManager;
    delete componentManager;
}

//static
Kernel* 
Kernel::getInstance()
{
    if(instance == NULL)
        instance = new Kernel();

    return instance;
}

stb::string 
Kernel::getConfig(stb::string config)
{
    using namespace std;
    ostringstream tmp;
    tmp << base_dir << "/" << config;
    return tmp.str();
}

//
void 
Kernel::start(int argc, char* argv[])
{
    logPrint("*****************************************\n");
    logPrint(STUDIERSTUBE_VERSION_STRING);logPrint("\n");
    logPrint("(C) ");logPrint(STUDIERSTUBE_YEAR_STRING);
    logPrint(" Graz University of Technology\n");
    logPrint("*****************************************\n\n");
    
    // extract application base directory name as search path
    // only useful for linux
    if (argc > 1) {
        base_dir = stb::string(argv[1]);
    } else {
        base_dir = ".";
    }
    std::cerr << "BASEDIR = " << base_dir << std::endl;
    ////////////////////////////////////////////////////////
    stb::string kernelConfigFile=getConfig(kernel_config_file);
    // read the kernel's configuration file
    config->readKernelConfig(kernelConfigFile);

    scheduler->init();
    scheduler->mainLoop();
}

//
void 
Kernel::stop()
{
}

////////////////////////////////////////////////////////////////

void
Kernel::parseConfiguration(TiXmlElement* element)
{
    ////////////<logging mode="xxxY filename="xxx"/> /////////
    TiXmlAttribute* attribute = element->FirstAttribute();
    while(attribute) //////////// kernel's parameter
    {
        ///////////////// logMode /////////////////
        scheduler->readConfiguration(attribute);
	    /////////////////// ------- /////////////////
	    ////else if(!stricmp(attribute->Name( 00000000000000000000000000000000000000000000000000000000000000000000000),"----"))
	    ////{		
	    ////}
        attribute = attribute->Next();
    }
}

void 
Kernel::update( void * /*data*/, SoSensor * /*sensor*/)
{
    instance->sceneManager->update();
    instance->componentManager->update();
}



stb::SceneManager* 
Kernel::getSceneManager()
{
    return sceneManager;
}

stb::string
Kernel::getBaseDir() const {
    return base_dir;
}



END_NAMESPACE_STB

//========================================================================
// End of file
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
