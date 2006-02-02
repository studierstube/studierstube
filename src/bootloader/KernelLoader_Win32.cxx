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
/** The impl. file for the KernelLoader_Win32 class.
*
* @author Denis Kalkofen
*
* $Id: KernelLoader_Win32.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */
#include "KernelLoader_Win32.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <string.h>
using namespace stb;

KernelLoader_Win32::KernelLoader_Win32()
{
	strcpy(execFuncName,"");
	strcpy(libName,"");
}

KernelLoader_Win32::~KernelLoader_Win32()
{
}
void 
KernelLoader_Win32::setLibName(const char* aLibName)
{
	strcpy(libName,aLibName);
#ifdef _DEBUG
	strcat(libName,"d");
#endif //_DEBUG
	strcat(libName,".dll");
}

void 
KernelLoader_Win32::setExecFuncName(const char* aFuncName)
{
	strcpy(execFuncName,aFuncName);
}

void 
KernelLoader_Win32::runKernel(int argc, char* argv[])
{
	HINSTANCE libHandle;
	libHandle = LoadLibrary(libName);
	if(libHandle==NULL){
		printf("ERROR: couldn't load %s\n",libName);
		return ;
	}
	void (*startKernel)(int,char**);
	startKernel=(void(*)(int, char**))GetProcAddress(libHandle,execFuncName);
	//call startkernel
	(*startKernel)(argc,argv);
	//clean up 
	FreeLibrary(libHandle);
}