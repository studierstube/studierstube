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


#include "Logging.h"
#include <tinyxml.h>
#include "kernel/Kernel.h"
using namespace stb;


Logging::Logging()
{
	mode=OFF;
	strcpy(filename,"");
}
Logging::~Logging()
{
	//nil
}

void
Logging::parseAttributes(TiXmlElement* element)
{
	TiXmlAttribute* attribute = element->FirstAttribute();
	while(attribute)
	{
		///////////////// Logging.mode /////////////////
		if(!stricmp(attribute->Name(),"mode"))
		{
			if(!stricmp(attribute->Value(),"file"))
				mode=FILE;		
			else if(!stricmp(attribute->Value(),"console"))
				mode = CONSOLE;
			else if(!stricmp(attribute->Value(),"off"))
				mode=OFF;
		}
		///////////////// Logging.filename /////////////////
		else if(!stricmp(attribute->Name(),"filename"))
			strcpy(filename,attribute->Value());
		///////////////// Logging. /////////////////
		//else if(!stricmp(attribute->Name(),"----"))
		//{		
		//}
		attribute = attribute->Next();
	}
}
