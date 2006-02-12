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

#include "OS.h"
#include <cstdio>
#include <sstream>
#include <iostream>

//---------------------------------------
/**
*/
hModule 
os_LoadLibrary(const char* fileName)
{
#ifdef WIN32
    using namespace std;
    ostringstream name;
    name  << fileName;
    #ifdef _DEBUG
        name << "d";
    #endif
        name <<".dll";
    return LoadLibrary(name.str().c_str());
#else //LINUX
    using namespace std;
    ostringstream name;
	if (lt_dlinit()) {
            cerr << "STB_ERROR: Initialisation of ltdl failed" << endl;
	}

        name  << "lib" << fileName << ".so" << endl;
	return lt_dlopenext(name.str().c_str());
#endif
}

//----------------------------------------
/**
*/
int*
os_GetProcAddress(hModule moduleHandle,const char*   procName)
{
#ifdef WIN32
    return (int*)GetProcAddress(moduleHandle,procName);
#else
    return (int*)lt_dlsym(moduleHandle,procName);
#endif
}

bool    
os_FreeLibrary(hModule libHandle)
{
#ifdef WIN32
    if(FreeLibrary(libHandle))//if else to avoid warning C4800:forcing value to bool 'true' or 'false' 
	return true;
    else 
	return false;
#else
    lt_dlclose(libHandle);
    return true;
#endif
}

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

