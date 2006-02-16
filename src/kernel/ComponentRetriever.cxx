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
* $Id: ComponentRetriever.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#include <stb/kernel/ComponentRetriever.h>
#include <stb/kernel/ComponentInfo.h>
#include <stb/kernel/Kernel.h>

BEGIN_NAMESPACE_STB

/**
*     The Constructor	
*/
ComponentRetriever::ComponentRetriever()
{

}

/**
*     The destructor.
*/
ComponentRetriever::~ComponentRetriever()
{
}


Component* 
ComponentRetriever::getComponent(ComponentInfo *compInfo)
{
	////load lib
    stb::string libName=compInfo->getLibName();
	if(libName=="")
		return NULL;
	hModule libHandle;
	libHandle = os_LoadLibrary(libName);
	if(!libHandle){
		Kernel::getInstance()->log("ERROR: couldn't load" + libName + "\n");
        return NULL;
	}
	compInfo->setHModule(libHandle);
	Component* (*createComponent)()=(Component*(*)())os_GetProcAddress(libHandle,"createComponent");
	Component* newComponent=(*createComponent)();
    newComponent->setComponentInfo(compInfo);
	return newComponent;
}

END_NAMESPACE_STB
