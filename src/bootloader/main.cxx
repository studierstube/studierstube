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
/** The source file for the stbbootloader.
*
* @author Denis Kalkofen
*
* $Id: main.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <stdio.h>

#define EXEC_KERNEL_FUNC "executeStudierstube"
#ifdef WIN32
	#ifdef _DEBUG
		#define KERNEL_DLL  "stbkerneld.dll"
		#define ERR_MSG "STB_ERROR: couldn't load kerneld.dll\n"
	#else
		#define KERNEL_DLL  "stbkernel.dll"
		#define ERR_MSG "STB_ERROR: couldn't load kernel.dll\n"
	#endif //_DEBUG
#else //WIN32
	#include <ltdl.h>
	#define KERNEL_DLL  "libstbkernel"
	#define ERR_MSG "STB_ERROR: couldn't load libstbkernel\n"
#endif


void (*startKernel)(int argc,char* argv[]);


int 
main(int argc,char* argv[])
{
	///// load library 
#ifdef WIN32
	HINSTANCE libHandle;
	libHandle = LoadLibrary(KERNEL_DLL);
#else //LINUX
	// initialise libltdl
	if (lt_dlinit())
	{
		printf("STB_ERROR: Initialisation of ltdl failed!\n");
	}
	lt_dlhandle libHandle;
	libHandle = lt_dlopenext(KERNEL_DLL);
#endif

	if(libHandle==NULL){
		printf(ERR_MSG);
		return 0;
	}

	//get function pointer to startKernel
#ifdef WIN32
	startKernel=(void(*)(int, char**))GetProcAddress(libHandle,EXEC_KERNEL_FUNC);
#else  //LINUX
//	startSAM=(void(*)(int, char**))lt_dlsym(libHandle, "_ZN9StbKernel11startKernelEiPPc");
	;
#endif
	if(startKernel==NULL){
		printf("STB_ERROR: can't find executeSAM(...) in %s \n",KERNEL_DLL);
		return 0;
	}

	//call startKernel
	(*startKernel)(argc,argv);

#ifdef WIN32
	FreeLibrary(libHandle);
#else  //LINUX
#endif

	return 1;
}

