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
* $Id: ComponentInfoWin32.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#include "ComponentInfoWin32.h"
#include "Kernel.h"
#include <tinyxml.h>

using namespace stb;

ComponentInfoWin32::ComponentInfoWin32()
{
	libHandle=NULL;
}

ComponentInfoWin32::~ComponentInfoWin32()
{
	if(libHandle)
		FreeLibrary(libHandle);
}
void 
ComponentInfoWin32::setHINSTANCE(HINSTANCE aLibHandle)
{
	if(libHandle)
		FreeLibrary(libHandle);
	libHandle=aLibHandle;
}
void
ComponentInfoWin32::parseXMLAttributes(TiXmlElement* element)
{
	TiXmlAttribute* attribute = element->FirstAttribute();
	while(attribute)
	{
		///////////////// Logging.mode /////////////////
		if(!stricmp(attribute->Name(),"name"))
		{
			if(name)
				delete name;
			const char *tempName=attribute->Value();
			name=new char[(int)strlen(tempName)+1];
			strcpy(name,tempName);
		}
		///////////////// Logging. /////////////////
		else if(!stricmp(attribute->Name(),"lib"))
		{	
			if(libName)
				delete libName;
			const char *tempName=attribute->Value();
			libName=new char[(int)strlen(tempName)+1];
			strcpy(libName,tempName);
		}
		///////////////// Logging. /////////////////
		//else if(!stricmp(attribute->Name(),"----"))
		//{		
		//}
		attribute = attribute->Next();
	}

	if(!name){
		Kernel::getInstance()->log("ERROR: missing attribute 'name' for Component\n");
	}
	if(!libName){
		Kernel::getInstance()->log("ERROR: missing attribute 'path' for Component\n");
	}

}