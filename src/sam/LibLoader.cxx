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
/** The header file for the StbLibLoader class.
*
* @author Denis Kalkofen
*
* $Id: StbLibLoader.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#include "StbLibLoader.h"
#include "StbKernel/interfaces/OpenTrackerContextInterface.h"
#include "StbKernel/interfaces/SoTrackedItemInterface.h"

StbLibLoader* StbLibLoader::instance=NULL;

StbLibLoader* 
StbLibLoader::getInstance()
{
	if(!instance)
		instance=new StbLibLoader();

	return instance;
}
StbLibLoader::StbLibLoader()
{
	guiBinding=SoWin;
	guiHandle=NULL;
	stbViewerHandle=NULL;
	stbEventHandle=NULL;
}

StbLibLoader::~StbLibLoader()
{
	if(guiHandle)
		os_FreeLibrary(guiHandle);
}


/////////////////////////////////////////////////////////
///////////////////// Load SoGui  ///////////////////////
////////////////////////////////////////////////////////

void 
StbLibLoader::setGuiBinding(StbLibLoader::GUI _guiBinding)
{
	guiBinding=_guiBinding;
}

StbLibLoader::GUI 
StbLibLoader::getCurrentGuiBinding()
{
	return guiBinding;
}


void 
StbLibLoader::startSoGui()
{
	(*soGuiMainLoopFunc)();
}

void
StbLibLoader::stopSoGui()
{
	(*soGuiExitMainLoopFunc)();
}

bool
StbLibLoader::initSoGui()
{
	//only load gui once
	if(guiHandle)
	   return false;

	switch(guiBinding){
		case SoWin:
		 #ifdef WIN32
			return loadSoWin();
		 #endif
			break;
		case SoQt:
			return loadSoQt();
			break;
	}
	return false;
}


bool
StbLibLoader::loadSoWin()
{
	printf("STB_INFO: load SoWin\n");
	void (*soGuiInitFunc)(const char *, const char*)=NULL;
	//
#ifdef _DEBUG   
	std::string libFileName="SoWin1d.dll";
#else
	std::string libFileName="SoWin1.dll";
#endif	
	
	guiHandle = os_LoadLibrary(libFileName.c_str());
	if (!guiHandle){
	    printf("STB_ERROR: could not load %s",libFileName.c_str());
	    return false;
	}
	
	//get pointer 
	soGuiInitFunc = (void (*)(const char *, const char*)) 
						os_GetProcAddress(guiHandle,"?init@SoWin@@SAPAUHWND__@@PBD0@Z");
	if(soGuiInitFunc == NULL)
		printf("STB_ERROR: could not find init() in %s",libFileName.c_str());


	//call SoGui::init 
	(*soGuiInitFunc)("Studierstube","SoWin"); 
	
	soGuiMainLoopFunc = (void (*)()) os_GetProcAddress(guiHandle,"?mainLoop@SoWin@@SAXXZ");
	soGuiExitMainLoopFunc=(void (*)()) os_GetProcAddress(guiHandle,"?exitMainLoop@SoWin@@SAXXZ");
	return true;
}

bool
StbLibLoader::loadSoQt()
{
    printf("STB_INFO: load SoQt\n");

    void (*soGuiInitFunc)(const char *)=NULL;
    //
#ifdef WIN32
#ifdef _DEBUG   
    std::string libFileName="SoQt1d.dll";
#else
    std::string libFileName="SoQt1.dll";
#endif
    std::string init="?init@SoQt@@SAPAVQWidget@@PBD0@Z";
    std::string mainLoop="?mainLoop@SoQt@@SAXXZ";

#else //LINUX
    std::string libFileName="libSoQt";
    std::string init="_ZN4SoQt4initEPKcS1_";
    std::string mainLoop="_ZN4SoQt8mainLoopEv"; 
#endif //WIN32
    guiHandle = os_LoadLibrary(libFileName.c_str());
    if (!guiHandle){
	printf("ERROR: could not load %s\n",libFileName.c_str());
	return false;
    }
    
    //// get pointer to SoGui::init  & SoGui::mainLoop   
    soGuiInitFunc = (void (*)(const char *)) os_GetProcAddress(guiHandle,init.c_str());
    if(soGuiInitFunc == NULL){
	printf("ERROR: could not find init() in %s",libFileName.c_str());
	return false;
    }
    
    //call SoGui::init 
    (*soGuiInitFunc)("stbBootLoader"); 
    //
    soGuiMainLoopFunc = (void (*)()) os_GetProcAddress(guiHandle,mainLoop.c_str());
    
    return 	true;

}

/////////////////////////////////////////////////////////
///////////////////// Load StbViewer  //////////////////
////////////////////////////////////////////////////////

bool
StbLibLoader::initStbViewer()
{
	if(stbViewerHandle)
		return true;
	
	std::string moduleName="stbviewer";
	switch(guiBinding){
		case SoWin:
			moduleName+="SoWin";
			break;
		case SoQt:
			moduleName+="SoQt";
			break;
	}
#ifdef WIN32
	std::string init="?init@StbViewer@@SAXXZ";
	//add file extensions
#ifdef _DEBUG   		
	moduleName +="d.dll";
#else
	moduleName +=".dll";
#endif //_DEBUG
#else
	moduleName="lib"+moduleName;
	std::string init="_ZN9StbViewer4initEv";
#endif //WIN32
//load stbviewer lib
	printf("STB_INFO: load %s \n",moduleName.c_str());
	stbViewerHandle = os_LoadLibrary(moduleName.c_str());
	if (!stbViewerHandle)
	{
	    printf("STB_ERROR: could not load %s\n",moduleName.c_str());
	    return false;
	}
	//call init() function
	void (*initFunc)()=NULL;
	initFunc=(void (*)()) os_GetProcAddress(stbViewerHandle,init.c_str());
	if(!initFunc){
	    printf("STB_ERROR: could not find init() in %s\n",moduleName.c_str());
	    return false;
	}
	(*initFunc)(); 

	return true;
}

void 
StbLibLoader::unloadStbViewer()
{
	if(!stbViewerHandle)
		return ;
	
	void (*finFunc)()=NULL;
	finFunc=(void (*)()) os_GetProcAddress(stbViewerHandle,"?fin@StbViewer@@SAXXZ");
	if(!finFunc){
		printf("STB_ERROR: could not unload StbViewer module \n");
		return ;
	}
	//// clean up stbviewer stuff
	(*finFunc)(); 

	//// unload lib
	os_FreeLibrary(stbViewerHandle);
	stbViewerHandle=NULL;
}

/////////////////////////////////////////////////////////
///////////////////// StbEvent  ////////////////////////
////////////////////////////////////////////////////////

bool
StbLibLoader::initStbEvent()
{
	if(stbEventHandle!=NULL)
		return false;

	std::string moduleName="stbevent";

#ifdef WIN32
	std::string init="?init@@YAXXZ";
	//add file extensions
	#ifdef _DEBUG   		
		moduleName +="d.dll";
	#else
		moduleName +=".dll";
	#endif
#else
	moduleName="lib"+moduleName;
	std::string init="_Z4initv";
#endif

	//load stbevent
	printf("STB_INFO: load %s \n",moduleName.c_str());
	stbEventHandle = os_LoadLibrary(moduleName.c_str());
	if (!stbEventHandle)
	{
		printf("STB_ERROR: could not load %s\n",moduleName.c_str());
		return false;
	}
	//call init() function
	void (*initFunc)()=NULL;
	initFunc=(void (*)()) os_GetProcAddress(stbEventHandle,init.c_str());
	if(!initFunc){
		printf("STB_ERROR: could not find init() in %s\n",moduleName.c_str());
		return false;
	}
	(*initFunc)(); 
	return true;
}

void 
StbLibLoader::unloadStbEvent()
{
    if(!isEventSystemInitialized)
	return ;
    
    ///// unload stbViewer lib
    os_FreeLibrary(stbEventHandle);
    stbEventHandle=NULL;
}


OpenTrackerContextInterface* 
StbLibLoader::getOpenTrackerContext()
{
#ifdef WIN32
    std::string func="?getInstance@OpenTrackerContext@@SAPAV1@XZ";
#else
    std::string func="_ZN18OpenTrackerContext11getInstanceEv";
#endif
    OpenTrackerContextInterface* (*getOTContext)()=NULL;
    getOTContext=(OpenTrackerContextInterface*  (*)())
	os_GetProcAddress(stbEventHandle,func.c_str());
    if(!getOTContext){
	printf("STB_ERROR: could not find OpenTrackerContext::getInstance() \n");
	return NULL;
    }
    
    return (*getOTContext)(); 
}


SoTrackedItemInterface* 
StbLibLoader::getSoTrackedItem()
{
#ifdef WIN32
    std::string func="?getNewInstance@SoTrackedItem@@SAPAV1@XZ";
#else
    std::string func="_ZN13SoTrackedItem14getNewInstanceEv";
#endif
    SoTrackedItemInterface* (*getTrItem)()=NULL;
    getTrItem=(SoTrackedItemInterface*  (*)())
	os_GetProcAddress(stbEventHandle,func.c_str());
    if(!getTrItem){
	printf("STB_ERROR: could not find SoTrackedItem::getNewInstance() \n");
	return NULL;
    }
    
    return (*getTrItem)(); 
}
