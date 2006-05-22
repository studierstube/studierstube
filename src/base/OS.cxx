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
/** The header file for the OS class.
*
* @author Denis Kalkofen
*
* $Id: OS.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#include <stb/base/OS.h>
#include <gl/GL.h>
#include <cstdio>
#include <sstream>
#include <iostream>


BEGIN_NAMESPACE_STB


//---------------------------------------
/**
*/
hModule 
os_LoadLibrary(stb::string fileName)
{
#ifdef STB_IS_WINDOWS
    using namespace std;
    ostringstream name;
    name  << fileName;
    //#ifdef _DEBUG
    //    name << "d";
    //#endif
        name <<".dll";

    //FILE* fp = fopen(name.str().c_str(), "rb");
    //if(!fp)
    //{
    //    printf("can't open %s\n",name.str().c_str());
    //    return NULL;
    //}

	return ::LoadLibrary(name.str().c_str());
#else //!STB_IS_WINDOWS
    using namespace std;
    ostringstream name;

    name  << "lib" << fileName << ".so";

    cerr << "Dynamically loading >" << name.str() << "< ...";
    
    // load the library
    hModule p = dlopen(name.str().c_str(), RTLD_LAZY);
    if (!p) {
        cerr << " failed: cannot load library: " << dlerror() << '\n';
        return 0;
    }
    cerr << " done." << endl;
    return p;
#endif
}

//----------------------------------------
/**
*/
int*
os_GetProcAddress(hModule moduleHandle,const char*   procName)
{
#ifdef STB_IS_WINDOWS
    return (int*)GetProcAddress(moduleHandle, procName);
#else
    return (int*)dlsym(moduleHandle, procName);
#endif
}

bool    
os_FreeLibrary(hModule libHandle)
{
#ifdef STB_IS_WINDOWS
    if(FreeLibrary(libHandle))//if else to avoid warning C4800:forcing value to bool 'true' or 'false' 
	return true;
    else 
	return false;
#else
    dlclose(libHandle);
    return true;
#endif
}


bool
os_correctModuleName(stb::string& fileName, bool nMakeDebug, bool nMakeES)
{
	//std::string baseName, ext;
	//int dotPos=fileName.find_last_of('.');

	//if(dotPos<1)
	//	return false;

	//baseName = fileName.substr(0, dotPos-1);
	//ext = fileName.substr(dotPos+1, fileName.length());

	//fileName = baseName;

#if defined(_IS_KLIMTES_)
	if(nMakeES)
		fileName += "_es";
#endif

#if defined(STB_IS_WINDOWS) && defined(STB_IS_DEBUG)
	if(nMakeDebug)
		fileName += "d";
#endif

	//fileName += ".";
	//fileName += ext;

	return true;
}


END_NAMESPACE_STB


/* ========================================================================
 * End of file
 * ========================================================================
 * Local Variables:
 * mode: c++
 * c-basic-offset: 4
 * eval: (c-set-offset 'substatement-open 0)
 * eval: (c-set-offset 'case-label '+)
 * eval: (c-set-offset 'statement 'c-lineup-runin-statements)
 * eval: (setq indent-tabs-mode nil)
 * End:
 * ========================================================================
 */

