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
 * PROJECT: Studierstube Application Manager (StbAM, SAM)
 * ======================================================================== */
/** @author   Daniel Wagner
 *
 * $Id: Config.cpp 131 2005-11-03 20:53:09Z daniel $
 * @file                                                                   */
/* ======================================================================= */


#include "Config.h"
#include <tinyxml.h>
#include "Kernel.h"
using namespace STB;

Config::Config()
{

}

Config::~Config()
{

}

/// parse the xml file
bool
Config::readConfigFile(const char* filename)
{   
	Kernel::getInstance()->logDebug("Kernel: start to parse config file %s\n",filename);

	TiXmlDocument* document = new TiXmlDocument();
	if(!document->LoadFile(filename))
	{
		Kernel::getInstance()->logEx("An error occured during parsing\n   Message: %s\n",document->ErrorDesc());
		return false;
	}
	// get studierstube parametzer
	TiXmlElement* root = document->RootElement();
	TiXmlAttribute* attribute = root->FirstAttribute();
	while(attribute)
	{
		setConfig(attribute->Name(), attribute->Value());
		attribute = attribute->Next();
	}
	/////	parsing is done -> clean up 
	document->Clear();
	delete document;
	return true;
}

bool
Config::setConfig(const char* key, const char* value)
{
	if(!stricmp(key,"log"))
	{
		if(!stricmp(value,"file"))
		{
			logMode=FILE;
			return true;
		}
		else if(!stricmp(value,"console"))
		{
			logMode=CONSOLE;
			return true;
		}
		else if(!stricmp(value,"console_and_file"))
		{
			logMode=CONSOLE_AND_FILE;
			return true;
		}
		else if(!stricmp(value,"off"))
		{
			logMode=OFF;
			return true;
		}

		return false;
	}
	return false;
}

Config::LOG_MODE 
Config::getLogMode()
{
	return logMode;
}