/* ========================================================================
 * Copyright (C) 2004-2005  Graz University of Technology
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
 * Graz University of Technology, Institut für Maschinelles Sehen und Darstellen,
 * Inffeldgasse 16a, 8010 Graz, Austria.
 * ========================================================================
 * PROJECT: 
 * ======================================================================== */
/** @author   Denis Kalkofen
 *
 * $Id: Config.cpp 131 2005-11-03 20:53:09Z daniel $
 * @file                                                                   */
/* ======================================================================= */


#include "Config.h"
#include <tinyxml.h>
#include "Kernel.h"
#include "ComponentInfo.h"
using namespace stb;

//================Config===========
Config::Config()
{
}

Config::~Config()
{
//nil
}

/// parse the xml file
bool
Config::readConfigFile(const char* filename)
{   
	Kernel::getInstance()->logDebug("parsing %s ...\n",filename);
	TiXmlDocument* document = new TiXmlDocument();
	
	if(!document->LoadFile(filename))
	{
		Kernel::getInstance()->logEx("An error occured during parsing %s\n", filename);
		Kernel::getInstance()->logEx(" Message: %s\n", document->ErrorDesc());
		return false;
	}

	TiXmlElement* root = document->RootElement();
	if(!stricmp(root->Value(),"studierstube"))
	{
		////parse studierstube parameter
		TiXmlElement* element = root->FirstChildElement();
		while(element)
		{
			parseXMLElement(element);
			element = element->NextSiblingElement();
		}
	}
	/////////	parsing is done -> clean up 
	document->Clear();
	delete document;
	return true;
}


bool
Config::parseXMLElement(TiXmlElement* element)
{
	////////Element -> Logging /////////////////
	if(!stricmp(element->Value(),"Kernel"))
	{
		Kernel::getInstance()->parseXMLConfig(element);
	}
	///////////////// ________ /////////////////
	else if(!stricmp(element->Value(),"Component"))
	{
		ComponentInfo *compInfo=new ComponentInfo();
		compInfo->parseXMLAttributes(element);
		Kernel::getInstance()->addComponent(compInfo);
	}
	else if(!stricmp(element->Value(),"Application"))
	{
		ComponentInfo *compInfo=new ComponentInfo();
		compInfo->parseXMLAttributes(element);
		Kernel::getInstance()->addApplication(compInfo);
	}
	/////////////////// ________ /////////////////
	////else if(!stricmp(element->Value(),"________"))
	////{
	////}

	/////////// Config does not allow children /////
	//TiXmlElement * childElement = element->FirstChildElement();
	//while(childElement)
	//{
	//	parseXMLElement(childElement);
	//	childElement = childElement->NextSiblingElement();
	//}
	return true;
}

