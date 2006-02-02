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
#include <stdarg.h>
#include <stdio.h>
#include <tinyxml.h>

#include "Config.h"
#include "ComponentManager.h"
#include "ComponentInfo.h"

using namespace stb;

Kernel*	Kernel::instance=NULL;

Kernel::Kernel()
{
	logMode=OFF;
	//
	logFile=new char(14);
	strcpy(logFile,"kernelLog.txt");
	//
	config=new stb::Config();
	//
	compManager=new ComponentManager();
}

Kernel::~Kernel()
{
	delete logFile;
	delete config;
	delete compManager;
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
	config->readConfigFile("kernel.xml");

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
Kernel::parseXMLAttributes(TiXmlElement* element)
{
	TiXmlAttribute* attribute = element->FirstAttribute();
	while(attribute)
	{
		///////////////// Logging.mode /////////////////
		if(!stricmp(attribute->Name(),"logMode"))
		{
			if(!stricmp(attribute->Value(),"file"))
				logMode=FILE;		
			else if(!stricmp(attribute->Value(),"console"))
				logMode=CONSOLE;
			else if(!stricmp(attribute->Value(),"off"))
				logMode=OFF;
		}
		///////////////// Logging.filename /////////////////
		else if(!stricmp(attribute->Name(),"logFile")){
			if(logFile)
				delete logFile;
			const char *tempName=attribute->Value();
			logFile=new char((int)strlen(tempName)+1);
			strcpy(logFile,tempName);
		}
		///////////////// Logging. /////////////////
		//else if(!stricmp(attribute->Name(),"----"))
		//{		
		//}
		attribute = attribute->Next();
	}
}
void
Kernel::addComponent(ComponentInfo* compInfo)
{
	compManager->addComponent(compInfo);
}