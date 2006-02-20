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
 * $Id: Config.cpp 131 2005-11-03 20:53:09Z denis $
 * @file                                                                   */
/* ======================================================================= */


#include <stb/kernel/Config.h>

#include <tinyxml.h>
#include <stb/kernel/Kernel.h>
#include <stb/kernel/ComponentInfo.h>
BEGIN_NAMESPACE_STB

//================Config===========
Config::Config()
{
}

Config::~Config()
{
}

/// parse the xml file
bool
Config::parseXML(stb::string filename)
{   
	Kernel::getInstance()->logDebug("parsing " + filename + " ...\n");
	TiXmlDocument* document = new TiXmlDocument();
	
	if(!document->LoadFile(filename.c_str()))
	{
		Kernel::getInstance()->log("An error occured during parsing " + filename + "\n");
		Kernel::getInstance()->log(" Message: ");
        Kernel::getInstance()->log(document->ErrorDesc());
        Kernel::getInstance()->log("\n");
		return false;
	}

	TiXmlElement* root = document->RootElement();
	if(!stb::stricasecmp(root->Value(),"studierstube"))
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
	if(!stb::stricasecmp(element->Value(),"Kernel"))
	{
		Kernel::getInstance()->parseConfiguration(element);
	}
	///////////////// ________ /////////////////
	else if(!stb::stricasecmp(element->Value(),"Component")
         || !stb::stricasecmp(element->Value(),"Application")
           )
	{
		ComponentInfo *compInfo=new ComponentInfo();
		compInfo->parseConfiguration(element);
		Kernel::getInstance()->addComponent(compInfo);
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

END_NAMESPACE_STB
