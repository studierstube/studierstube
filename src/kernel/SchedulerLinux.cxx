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
/** The header file for the SchedulerLinux class.
*
* @author Bernhard Reitinger
*
* $Id$
* @file                                                                   */
/* ======================================================================= */

#include <stb/kernel/SchedulerLinux.h>
#include <stb/kernel/Kernel.h>

#include <tinyxml.h>
#include <iostream>

#include <ace/Log_Msg.h>

BEGIN_NAMESPACE_STB

SchedulerLinux::SchedulerLinux()
{
}

SchedulerLinux::~SchedulerLinux()
{
   //nil
}

void
SchedulerLinux::parseConfiguration(TiXmlAttribute* attribute)
{
    using namespace std;

    if (!strcasecmp(attribute->Name(),"guiBinding"))
    {
        if (!strcasecmp(attribute->Value(),"SoQt")) {
            // do nothing
        } else {
            cerr << "ERROR: unknown guiBinding " << attribute->Value() << ", using default (SoQt)" << endl;
        }
    }
    SchedulerBase::parseConfiguration(attribute);
}

void 
SchedulerLinux::init()
{

    // no demangled function pointer available :-(
    stb::string initStr="_ZN4SoQt4initEPKcS1_";
    
    Kernel::getInstance()->logDebug("INFO: load SoQt\n");
    stb::string libFileName = "SoQt";

    libHandle = os_LoadLibrary(libFileName.c_str());
    if (!libHandle){
        Kernel::getInstance()->log("ERROR: could not load " + libFileName);
        return;
    }

    //get pointer 
    void (*soGuiInitFunc)(const char *, const char*)=NULL;
    soGuiInitFunc = (void (*)(const char *, const char*))os_GetProcAddress(libHandle, initStr.c_str());

    if (!soGuiInitFunc) {
        ACE_DEBUG((LM_ERROR, "could not get entry point of %s", libFileName.c_str()));
        return;
    }
    
    //call SoGui::init 
    (*soGuiInitFunc)("Studierstube","SoQt"); 

}

void 
SchedulerLinux::mainLoop()
{
    // no demangled function pointer available :-(
    stb::string mainLoopStr="_ZN4SoQt8mainLoopEv"; 

    if(!libHandle){
        Kernel::getInstance()->logDebug("Error: call soGui.init() before soGui.mainLoop. \n");
        return;
    }
    
    void (*mainLoopFunc)();
    mainLoopFunc = (void (*)())os_GetProcAddress(libHandle, mainLoopStr.c_str());

    if (!mainLoopFunc) {
        ACE_DEBUG((LM_ERROR, "could not get entry point of %s", mainLoopStr.c_str()));
        return;
    }

    (*mainLoopFunc)();
    
}

END_NAMESPACE_STB

//========================================================================
// End of SchedulerLinux.cxx
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
