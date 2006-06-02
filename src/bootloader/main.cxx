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

#include <stb/bootloader/KernelLoader.h>
#include <stb/base/OS.h>


#ifdef STB_IS_WINCE


int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	const int argc = 2;
	char* argv[argc] = { "studierstube.exe", "/data" };
	stb::string libName="stbkernel";

	// make sure we load debug version in case of debug build...
	//
	stb::os_correctModuleName(libName, true, true);

	const char execFunc[]="executeStudierstube";
	stb::KernelLoader loader;
	loader.setExecFuncName(execFunc);
	loader.setLibName(libName);
	loader.runKernel(argc,argv);

	return 0;
}


#else //STB_IS_WINCE


int 
main(int argc,char* argv[])
{
    stb::string libName="stbkernel";

	// make sure we load debug version in case of debug build...
	//
	stb::os_correctModuleName(libName, true, true);

    const char execFunc[]="executeStudierstube";
    stb::KernelLoader loader;
    loader.setExecFuncName(execFunc);
    loader.setLibName(libName);
    loader.runKernel(argc,argv);
    
    return 0;
}


#endif //STB_IS_WINCE
