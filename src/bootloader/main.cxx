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
/** The source file for the studierstube bootloader.
*
* @author Denis Kalkofen
*
* $Id: main.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

 #include "KernelLoader.h"

int 
main(int argc,char* argv[])
{
	const char execFunc[]="executeStudierstube";
	const char libName[]="stbkernel";
	stb::KernelLoader loader;
	loader.setExecFuncName(execFunc);
	loader.setLibName(libName);
	loader.runKernel(argc,argv);

	return 1;
}

//#else //LINUX
// #include <ltdl.h>
// #define KERNEL_DLL  "libstbkernel"
// #define ERR_MSG "STB_ERROR: couldn't load libstbkernel\n"
//#endif
//	///// load library 
//#else //LINUX
//	// initialise libltdl
//	if (lt_dlinit())
//	{
//		printf("STB_ERROR: Initialisation of ltdl failed!\n");
//	}
//	lt_dlhandle libHandle;
//	libHandle = lt_dlopenext(KERNEL_DLL);
//#endif
//
//	if(libHandle==NULL){
//		printf(ERR_MSG);
//		return 0;
//	}
//
//	//get function pointer to startKernel
//#ifdef WIN32

//#else  //LINUX
//	startKernel=(void(*)(int, char**))lt_dlsym(libHandle, EXECFUNC);
//
//#endif
//	if(startKernel==NULL){
//		printf("STB_ERROR: can't find executeSAM(...) in %s \n",KERNEL_DLL);
//		return 0;
//	}
//