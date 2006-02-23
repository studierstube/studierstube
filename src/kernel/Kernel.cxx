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
#include <stb/kernel/interfaces/SoTrakEngineInterface.h>
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
    config=new stb::Config();
	
    scheduler= new stb::Scheduler();
	sceneManager= new stb::SceneManager();
	componentManager= new stb::ComponentManager();
    stb::SoTrakEngineInterface::initClass();
	//////
}

Kernel::~Kernel()
{
	printf("destructor\n");
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


//static
void 
Kernel::start(int argc, char* argv[])
{
	log("****************************************\n");
	log(STUDIERSTUBE_VERSION_STRING);log("\n");
	log("(C) ");log(STUDIERSTUBE_YEAR_STRING);log(" Graz University of Technology\n");
	log("****************************************\n\n");

    scheduler->init();
	config->parseXML("kernel.xml");

	
	scheduler->schedule();
	scheduler->mainLoop();
}

//static
void 
Kernel::stop()
{
}

void
Kernel::log(stb::string nStr)
{
	printf("%s",nStr.c_str());
}


void 
Kernel::logDebug(stb::string nStr)
{
#ifdef _DEBUG
	log(nStr);
#endif
}

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
	//	///////////////// ------- /////////////////
	//	//else if(!stricmp(attribute->Name(),"----"))
	//	//{		
	//	//}
		attribute = attribute->Next();
	}
}

void 
Kernel::update( void * data, SoSensor * sensor)
{
    instance->sceneManager->update();
    instance->componentManager->update();
	printf(".");
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


stb::SoTrakEngineInterface*
Kernel::createSoTrakEngine()
{
    stb::SoTrakEngineInterface* tEngine=NULL;
    //get handle to event system
    Component* comp=componentManager->load("Event");
    if(!comp)
    {
        printf("failed to load event system\n");
        return tEngine;
    }
    // //os_GetProcAddress
    stb::SoTrakEngineInterface* (*createTrakEngine)()=NULL;
    createTrakEngine = (stb::SoTrakEngineInterface*(*)())os_GetProcAddress(comp->getInfo()->getLibHandle(),"createTrakEngine");
    if(createTrakEngine)
        tEngine=(*createTrakEngine)();
    return tEngine;
}

END_NAMESPACE_STB
