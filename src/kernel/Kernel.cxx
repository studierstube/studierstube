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
/** The header file for the StbKernel class.
*
* @author Denis Kalkofen
*
* $Id: Kernel.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#include "Kernel.h"
#include <stdarg.h>
#include <stdio.h>

#include "Config.h"
using namespace STB;

Kernel*	Kernel::instance=NULL;

Kernel::Kernel()
{
}

Kernel::~Kernel()
{
	config=new STB::Config();
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
	printf("start kernel \n");
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
#ifdef DEBUG
	char tmpString[512];
	va_list vaList;
	va_start(vaList, nStr);
	vsprintf(tmpString, nStr,vaList);
	log(tmpString);
#endif
}
