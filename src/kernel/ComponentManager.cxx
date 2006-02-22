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

BEGIN_NAMESPACE_STB

ComponentManager::ComponentManager()
{
	compRetriever=new stb::ComponentRetriever();
	appListSize=0;
    initListSize=0;
}

ComponentManager::~ComponentManager()
{
   delete compRetriever;
}

void 
ComponentManager::update()
{
    printf("ComponentManager::update()\n");
    // init all new components
    if(initListSize>0)
    {
        for(int i=0;i<initListSize;i++)
        {
            initComponent(initList[i]);
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


void
ComponentManager::initComponent(Component *comp)
{
    if(comp->init())
    {
        // add to applist or comp.list
        stb::string id=comp->getBaseTypeID();
        if(id==Application::getBaseTypeID())
        {
            appList.push_back((Application*)comp);
            appListSize++;
        }
        else if(id==Component::getBaseTypeID())
            compList.push_back(comp);

    }
    else
        delete comp;
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

bool 
ComponentManager::isLoaded(std::string compName)
{
    for(int i=0;i<(int)compList.size();i++)
    {
        if(compList[i]->getInfo()->getName()==compName)
            return true;
    }
    for(int i=0;i<(int)appList.size();i++)
    {
        if(appList[i]->getInfo()->getName()==compName)
            return true;
    }


    return false;

}

bool 
ComponentManager::load(std::string compName)
{
    if(isLoaded(compName))
        return true;
    //search demandList
    for(int i=0;i<(int)demandList.size();i++)
    {
        if(demandList[i]->getName()==compName)
        {
            Component* newComp=NULL;
            newComp=(Component*)compRetriever->getComponent(demandList[i]);
            if(!newComp){
                return false;
            }
            initComponent(newComp);
            return true;
        }
    }
    //search initList 
    for(int i=0;i<(int)initList.size();i++)
    {
        if(initList[i]->getInfo()->getName()==compName)
        {
            initComponent(initList[i]);
            return true;
        }
    }

    stb::Kernel::getInstance()->log("Error: unable to find " + compName + "\n");
    return false;
}




END_NAMESPACE_STB