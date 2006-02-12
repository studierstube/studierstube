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

#include "Kernel.h"
#include "studierstube.h"
#include <stdarg.h>
#include <stdio.h>
#include <tinyxml.h>
#include <ace/ACE.h>
#include <Inventor/SoDB.h> 
#include <Inventor/sensors/SoSensor.h>
//
#include "Config.h"
#include "UpdateManager.h"
//#include "SceneManager.h"
//#include "ComponentManager.h"
//#include "ComponentInfo.h"
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
	soGui =new stb::SoGui();
    updateManager= new stb::UpdateManager();
	//sceneManager= new stb::SceneManager();
	//componentManager= new stb::ComponentManager();
	//////
}

Kernel::~Kernel()
{
	//printf("destructor\n");
	//ACE::fini();
	delete config;
	delete soGui;
	delete updateManager;
	//delete sceneManager;
	//delete componentManager;
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
Kernel::start(int argc,char* argv[])
{
	log("****************************************\n");
	logEx("%s \n",STUDIERSTUBE_VERSION_STRING);
	logEx("(C) %s Graz University of Technology\n",STUDIERSTUBE_YEAR_STRING);
	log("****************************************\n\n");

	config->parseXML("kernel.xml");
	soGui->init();
	
	updateManager->schedule();

	soGui->mainLoop();
}

//static
void 
Kernel::stop()
{
}

void
Kernel::log(const char* nStr)
{
	printf("%s",nStr);
}

void 
Kernel::logEx(const char* nStr ...)
{
	char tmpString[512];
	va_list vaList;
	va_start(vaList, nStr);
	vsprintf(tmpString, nStr,vaList);
	log(tmpString);
}

void 
Kernel::logDebug(const char* nStr ...)
{
#ifdef _DEBUG
	char tmpString[512];
	va_list vaList;
	va_start(vaList, nStr);
	vsprintf(tmpString, nStr,vaList);
	log(tmpString);
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
		if(!stricmp(attribute->Name(),"logMode"))
		{
			if(!stricmp(attribute->Value(),"file"))
				logMode=FILE;		
			else if(!stricmp(attribute->Value(),"console"))
				logMode=CONSOLE;
			else if(!stricmp(attribute->Value(),"off"))
				logMode=OFF;
		}
		///////////////// logFile /////////////////
		else if(!stricmp(attribute->Name(),"logFile"))
        {
            logFile=attribute->Value();
		}
		else if(!stricmp(attribute->Name(),"guiBinding"))
		{		
			soGui->parseConfiguration(attribute);
		}
		else if(!stricmp(attribute->Name(),"updateMode"))
		{		
			updateManager->parseConfiguration(attribute);
		}
		else if(!stricmp(attribute->Name(),"updateRate"))
		{		
            updateManager->parseConfiguration(attribute);
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
	printf("Kernel::update()\n");
}

//void 
//Kernel::addApplication(ComponentInfo* compInfo)
//{
//	/*componentManager->addApplication(compInfo);*/
//}
//
//void 
//Kernel::addComponent(ComponentInfo* compInfo)
//{
//	/*componentManager->addComponent(compInfo);*/
//}

END_NAMESPACE_STB