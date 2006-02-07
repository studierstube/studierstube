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
* $Id: ComponentRetrieverWin32.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#include "ComponentRetrieverWin32.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "Kernel.h"
using namespace stb;

ComponentRetrieverWin32::ComponentRetrieverWin32()
{
   //nil
}

ComponentRetrieverWin32::~ComponentRetrieverWin32()
{
   //nil
}

Component* 
ComponentRetrieverWin32::getComponent(ComponentInfo *compInfo)
{
	////load dll
	char* libName=compInfo->getLibName();
	if(!libName)
		return NULL;

	HINSTANCE libHandle;
	libHandle = LoadLibrary(libName);
	if(!libHandle){
		Kernel::getInstance()->logEx("ERROR: couldn't load %s\n",libName);
		return NULL;
	}
	compInfo->setHINSTANCE(libHandle);
	Component* (*createComponent)()=(Component*(*)())GetProcAddress(libHandle,"createComponent");
	Component* newComponent=(*createComponent)();
	
	return newComponent;
}