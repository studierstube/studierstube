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
* $Id: SoGuiWin32.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#include "SoGuiWin32.h"
#include "Kernel.h"
#include <tinyxml.h>
#include <string>

using namespace stb;

SoGuiWin32::SoGuiWin32()
{
	setDefaultBinding();
	libHandle=NULL;
}

SoGuiWin32::~SoGuiWin32()
{
   //nil
}

void 
SoGuiWin32::setDefaultBinding()
{
   curGuiBinding=SoGuiWin32::SOWIN;
}

void
SoGuiWin32::readXMLConfig(TiXmlAttribute* attribute)
{
	if(!stricmp(attribute->Value(),"SoWin"))
		curGuiBinding=SoGuiWin32::SOWIN;		
	else if(!stricmp(attribute->Value(),"SoQt"))
		curGuiBinding=SoGuiWin32::SOQT;		
}

void
SoGuiWin32::loadSoQt()
{
	printf("SoQt is not supported yet! \n");
}

void 
SoGuiWin32::loadSoWin()
{
	Kernel::getInstance()->logDebug("INFO: load SoWin\n");
	//
#ifdef _DEBUG   
	std::string libFileName="SoWin1d.dll";
#else
	std::string libFileName="SoWin1.dll";
#endif	

	libHandle = LoadLibrary(libFileName.c_str());
	if (!libHandle){
		Kernel::getInstance()->logEx("ERROR: could not load %s",libFileName.c_str());
		return;
	}

	//get pointer 
	void (*soGuiInitFunc)(const char *, const char*)=NULL;
	soGuiInitFunc = (void (*)(const char *, const char*)) 
					GetProcAddress(libHandle,"?init@SoWin@@SAPAUHWND__@@PBD0@Z");
	if(soGuiInitFunc == NULL)
		printf("STB_ERROR: could not find init() in %s",libFileName.c_str());

	//call SoGui::init 
	(*soGuiInitFunc)("Studierstube","SoWin"); 
}

void 
SoGuiWin32::init()
{
	switch(curGuiBinding){
		case SOWIN:
			loadSoWin();
			break;
		case SOQT:
			loadSoQt();
			break;
	}
}

void
SoGuiWin32::mainLoopSoWin()
{
	void (*mainLoopFunc)();
	mainLoopFunc = (void (*)()) GetProcAddress(libHandle,"?mainLoop@SoWin@@SAXXZ");
	(*mainLoopFunc)();
}

void 
SoGuiWin32::mainLoopSoQt()
{

}

void 
SoGuiWin32::mainLoop()
{
	if(!libHandle){
		Kernel::getInstance()->logDebug("Error: call soGui.init() before soGui.mainLoop. \n");
		return;
	}
	switch(curGuiBinding){
		case SOWIN:
			mainLoopSoWin();
			break;
		case SOQT:
			mainLoopSoQt();
			break;
	}
}