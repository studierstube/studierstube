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
#include <stdarg.h>
#include <stdio.h>
#include <tinyxml.h>
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
    // 
    logMode=OFF;
	logFile="kernelLog.txt";
    //
    SoStbScene::initClass();
    config=new stb::Config();
    scheduler= new stb::Scheduler();
    sceneManager= new stb::SceneManager();
    componentManager= new stb::ComponentManager();

    //////
    stb_config_path1   = "/usr/share/stb/";
    stb_config_path2   = "/usr/local/share/stb/";
    stb_home           = ".stb/";
    kernel_config_file = "kernel.xml";
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
Kernel::getKernelConfig(int argc, char* argv[])
{
    if(argc > 1)
        return findConfigFile(stb::string(argv[1]));
    else
        return findConfigFile(kernel_config_file);
}

//
void 
Kernel::start(int argc, char* argv[])
{
    logPrint("****************************************\n");
    logPrint(STUDIERSTUBE_VERSION_STRING);logPrint("\n");
    logPrint("(C) ");logPrint(STUDIERSTUBE_YEAR_STRING);
    logPrint(" Graz University of Technology\n");
    logPrint("****************************************\n\n");
    
    stb::string kernelConfigFile=getKernelConfig(argc,argv);
    config->parseXML(kernelConfigFile);
    scheduler->init();
    scheduler->mainLoop();
}

//static
void 
Kernel::stop()
{
}

//////////////////////////////////////// LOGGING ////////////////////////
void
Kernel::log(stb::string nStr)
{
	//printf("%s",nStr.c_str());
	logPrint(nStr);
}


void 
Kernel::logDebug(stb::string nStr)
{
#ifdef _DEBUG
	logPrint(nStr);
#endif
}

void
Kernel::logEx(const char* nStr, ...)
{
    char tmpString[1024]; 
    va_list marker;

    va_start(marker, nStr);
    vsprintf(tmpString, nStr, marker);

	logPrint(nStr);
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
        if(!stb::stricasecmp(attribute->Name(),"logMode"))
        {
            if(!stb::stricasecmp(attribute->Value(),"file"))
                logMode=FILE;		
            else if(!stb::stricasecmp(attribute->Value(),"console"))
                logMode=CONSOLE;
            else if(!stb::stricasecmp(attribute->Value(),"off"))
                logMode=OFF;
        }
        ///////////////// logFile /////////////////
        else if(!stb::stricasecmp(attribute->Name(),"logFile"))
        {
            logFile=attribute->Value();
        }
        else 
        {
            scheduler->parseConfiguration(attribute);
        }
	    /////////////////// ------- /////////////////
	    ////else if(!stricmp(attribute->Name( 00000000000000000000000000000000000000000000000000000000000000000000000),"----"))
	    ////{		
	    ////}
        attribute = attribute->Next();
    }
}

stb::string
Kernel::findConfigFile(const stb::string& cfgfile)
{
    stb::string ret(cfgfile);

#ifdef LINUX
    using namespace std;
    ifstream in;

    // first attempt: HOME dir
    char *home_dir = 0;
    home_dir = getenv("HOME");

    ostringstream fn;
    fn << home_dir << "/" << stb_home << cfgfile;

    logPrint("Search for %s config file in %s ... ", cfgfile.c_str(), fn.str().c_str());
    in.open(fn.str().c_str(), ios::in);

    // if not found, use global1
    if (!in.is_open()) {
        logPrint("not found.\n");
        fn.str("");
        fn << stb_config_path1 << cfgfile;

        logPrint("Search for %s config file in %s ... ", cfgfile.c_str(), fn.str().c_str());
        in.open(fn.str().c_str(), ios::in);

        // if not found, use global2
        if (!in.is_open()) {
            logPrint("not found.\n");
            fn.str("");
            fn << stb_config_path2 << cfgfile;

            logPrint("Search for %s config file in %s ... ", cfgfile.c_str(), fn.str().c_str());
            in.open(fn.str().c_str(), ios::in);
            if (!in.is_open()) {
                logPrint("not found.\n");
                logPrintE(LOG_ERROR_FILE_NOT_FOUND);
                return ret;
            }
        }
    }
    logPrint("found.\n");
    in.close();
    ret = fn.str();
#endif

    return ret;
}

void 
Kernel::update( void * data, SoSensor * sensor)
{
    instance->sceneManager->update();
    instance->componentManager->update();
}


void 
Kernel::addComponent(ComponentInfo* compInfo)
{
    componentManager->addComponent(compInfo);
}

stb::SceneManager* 
Kernel::getSceneManager()
{
    return sceneManager;
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
