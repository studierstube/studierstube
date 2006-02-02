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
/** The header file for the ComponentInfo class.
*
* @author Denis Kalkofen
*
* $Id: ComponentInfo.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#include "ComponentInfo.h"
#include "Kernel.h"
#include <tinyxml.h>

using namespace stb;

ComponentInfo::ComponentInfo()
{
   //nil
	compName=NULL;
	path=NULL;
}

ComponentInfo::~ComponentInfo()
{
   //nil
}

bool 
ComponentInfo::parseXMLAttributes(TiXmlElement* element)
{
	TiXmlAttribute* attribute = element->FirstAttribute();
	bool retValue=true;
	while(attribute)
	{
		///////////////// Logging.mode /////////////////
		if(!stricmp(attribute->Name(),"name"))
		{
			const char *tempName=attribute->Value();
			compName=new char(strlen(tempName));
			strcpy(compName,tempName);
		}
		/////////////// Logging. /////////////////
		else if(!stricmp(attribute->Name(),"----"))
		{		
		}
		///////////////// Logging. /////////////////
		//else if(!stricmp(attribute->Name(),"----"))
		//{		
		//}
		attribute = attribute->Next();
	}

	if(!compName){
		Kernel::getInstance()->log("ERROR: missing attribute 'name' for Component\n");
		retValue=false;
	}
	if(!path){
		retValue=false;
		Kernel::getInstance()->log("ERROR: missing attribute 'path' for Component\n");
	}

	return retValue;
}