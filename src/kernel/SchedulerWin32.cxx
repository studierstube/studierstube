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
/** The header file for the SchedulerWin32 class.
*
* @author Denis Kalkofen
*
* $Id: SchedulerWin32.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#ifndef LINUX

#include <stb/kernel/SchedulerWin32.h>
#include <tinyxml.h>
#include <stb/kernel/Kernel.h>


BEGIN_NAMESPACE_STB

SchedulerWin32::SchedulerWin32()
{
    curGuiBinding=SchedulerWin32::SOWIN;
    libHandle=NULL;
}

SchedulerWin32::~SchedulerWin32()
{
   //nil
}

void
SchedulerWin32::parseConfiguration(TiXmlAttribute* attribute)
{
    if(!stb::stricasecmp(attribute->Name(),"guiBinding"))
    {
        if(!stb::stricasecmp(attribute->Value(),"SoWin"))
            curGuiBinding=SchedulerWin32::SOWIN;		
        else if(!stb::stricasecmp(attribute->Value(),"SoQt"))
            curGuiBinding=SchedulerWin32::SOQT;	
    }
    //else if()
    //{

    //}
    else
        SchedulerBase::parseConfiguration(attribute);
}

void
SchedulerWin32::loadSoQt()
{
    logPrintE("SoQt is not supported yet! \n");
}

void 
SchedulerWin32::loadSoWin()
{
    Kernel::getInstance()->logDebug("INFO: load SoWin\n");
    //
#ifdef _DEBUG   
    std::string libFileName="SoWin1d.dll";
#else
    std::string libFileName="SoWin1.dll";
#endif	

    libHandle = LoadLibrary(libFileName.c_str());
    if (!libHandle){
        Kernel::getInstance()->log("ERROR: could not load " + libFileName);
        return;
    }

    //get pointer 
    void (*soGuiInitFunc)(const char *, const char*)=NULL;
    soGuiInitFunc = (void (*)(const char *, const char*)) 
        GetProcAddress(libHandle,"?init@SoWin@@SAPAUHWND__@@PBD0@Z");
    if(soGuiInitFunc == NULL)
        Kernel::getInstance()->logEx("STB_ERROR: could not find init() in %s",libFileName.c_str());

    //call SoGui::init 
    (*soGuiInitFunc)("Studierstube","SoWin"); 
}

void 
SchedulerWin32::init()
{
    switch(curGuiBinding){
        case SOWIN:
            loadSoWin();
            break;
        case SOQT:
            loadSoQt();
            break;
    }
    schedule();
}

void
SchedulerWin32::mainLoopSoWin()
{
    void (*mainLoopFunc)();
    mainLoopFunc = (void (*)()) GetProcAddress(libHandle,"?mainLoop@SoWin@@SAXXZ");
    (*mainLoopFunc)();
   
}

void 
SchedulerWin32::mainLoopSoQt()
{

}

void 
SchedulerWin32::mainLoop()
{
    if(!libHandle){
        Kernel::getInstance()->logDebug("Error: call soGui.init() before soGui.mainLoop. \n");
        return;
    }
    switch(curGuiBinding){
        case SOWIN:
            mainLoopSoWin();
            break;
        case SOQT:
            mainLoopSoQt();
            break;
    }
}
END_NAMESPACE_STB

#endif
//========================================================================
// End of SchedulerWin32.cxx
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
