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
/** The cxx file for the ComponentManager class.
*
* @author Denis Kalkofen
*
* $Id: ComponentManager.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */
#include <stb/kernel/ComponentManager.h>
#include <stb/kernel/Component.h>
#include <stb/kernel/Application.h>
#include <stb/kernel/ComponentInfo.h>
#include <stb/kernel/ComponentRetriever.h>
#include <stb/kernel/Kernel.h>

#include <iostream>

BEGIN_NAMESPACE_STB

ComponentManager::ComponentManager()
{
	compRetriever=new stb::ComponentRetriever();

#pragma message(">>> daniel2denis: do we really need this 'appListSize' ?")
	appListSize=0;

#pragma message(">>> daniel2denis: do we really need this 'initListSize' ?")
    initListSize=0;
}

ComponentManager::~ComponentManager()
{
   delete compRetriever;
}

void 
ComponentManager::update()
{
    // init all new components
    if(initListSize>0)
    {
        for(int i=0;i<initListSize;i++)
        {
            Component* newComp=initList[i];
            if(!initComponent(newComp))
                initList[i]=NULL;
        }
        initList.clear();
        initListSize=0;
    }

    // update application list
    for(int i=0;i<appListSize;i++)
    {
        appList[i]->update();
    }
}

bool
ComponentManager::initComponent(Component *comp)
{
    if(comp->init())
    {
        // add to applist or comp.list
        stb::string id=comp->getTypeID();
        if(id==Application::getBaseTypeID())
        {
            appList.push_back((Application*)comp);
            appListSize++;
        }
        else if(id==Component::getBaseTypeID())
            compList.push_back(comp);
        return true;
    }
    
    return false;
}

void 
ComponentManager::addComponent(ComponentInfo* compInfo)
{
    switch(compInfo->getAvailability())
    {
        case ComponentInfo::ON_DEMAND:
            demandList.push_back(compInfo);
            break;
        case ComponentInfo::ON_LOAD:
            Component* newComp=NULL;
            newComp=(Component*)compRetriever->getComponent(compInfo);
            if(!newComp){
                return;
            }
            initList.push_back(newComp);
            initListSize++;
            break;
    }
}

stb::Component* 
ComponentManager::isLoaded(std::string compName)
{
    using namespace std;
//     cerr << "Check if component is loaded: " <<  compName << endl;
    for(int i=0;i<(int)compList.size();i++)
    {
// 	cerr << "Check if component is loaded: " << compList[i]->getInfo()->getName()  << endl;
        if(compList[i]->getInfo()->getName()==compName)
            return compList[i];
    }
    for(int i=0;i<(int)appList.size();i++)
    {
        if(appList[i]->getInfo()->getName()==compName)
            return appList[i];
    }

    return NULL;
}

stb::Component* 
ComponentManager::load(std::string compName)
{
    stb::Component* retComp=isLoaded(compName);
    if(retComp)
        return retComp;
    //search demandList
    for(int i=0;i<(int)demandList.size();i++)
    {
        if(demandList[i]->getName()==compName)
        {
            Component* newComp=NULL;
            newComp=(Component*)compRetriever->getComponent(demandList[i]);
            if(!newComp){
                return NULL;
            }
            initComponent(newComp);
            return newComp;
        }

    }
    //search initList 
    for(int i=0;i<(int)initList.size();i++)
    {
        if(initList[i]->getInfo()->getName()==compName)
        {
            Component* newComp=initList[i];
            initComponent(newComp);
            return newComp;
        }
    }

    logPrintE("Error: unable to find " + compName + "\n");
    return NULL;
}

END_NAMESPACE_STB
