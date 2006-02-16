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

/** The impl file for the KernelLoader class.
*
* @author Bernhard Reitinger
*
* $Id$
* @file                                                                   */
/* ======================================================================= */

#include <stb/bootloader/KernelLoader.h>
#include <stb/base/OS.h>

#include <iostream>

BEGIN_NAMESPACE_STB

KernelLoader::KernelLoader() {    
}

KernelLoader::~KernelLoader() {    
}

void KernelLoader::setLibName(stb::string aLibName) 
{
    libName = aLibName;
}

void KernelLoader::setExecFuncName(stb::string aFuncName) 
{
    execFuncName = aFuncName;
}

bool KernelLoader::runKernel(int argc, char* argv[]) 
{
    using namespace std;
    hModule libHandle;
    libHandle = os_LoadLibrary(libName.c_str());
    
    if (!libHandle) 
    {
        cerr << "ERROR: couldn't load library " << libName << endl;
        return false;
    }

    // get function pointer
    void (*startKernel)(int, char**);
    startKernel = (void(*)(int, char**))os_GetProcAddress(libHandle, execFuncName.c_str());

    //call startkernel
    (*startKernel)(argc,argv);
    
    //clean up 
    os_FreeLibrary(libHandle);

    return true;
}

END_NAMESPACE_STB

//========================================================================
// End of KernelLoader.cxx
//========================================================================
// Local Variables:
// mode: c++
// c-basic-offset: 4
// eval: (c-set-offset 'substatement-open 0)
// eval: (c-set-offset 'case-label '+)
// eval: (c-set-offset 'statement 'c-lineup-runin-statements)
// eval: (setq indent-tabs-mode nil)
// End:
//========================================================================
