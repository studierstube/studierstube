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

#include "ComponentManager.h"
#include "Component.h"
#include "Application.h"
#include "ComponentInfo.h"
#include "ComponentRetriever.h"

BEGIN_NAMESPACE_STB

ComponentManager::ComponentManager()
{
	compRetriever=new stb::ComponentRetriever();
	appListSize=0;
}

ComponentManager::~ComponentManager()
{
   delete compRetriever;
}

void 
ComponentManager::addApplication(ComponentInfo* compInfo)
{
	Application* newApp=(Application*)compRetriever->getComponent(compInfo);
	if(!newApp)
		return;

    //add parameter
	newApp->init();
    newApp->registerScene();

	appList.push_back(newApp);
	appListSize++;
}

void 
ComponentManager::addComponent(ComponentInfo* compInfo)
{
	Component* newComp=(Component*)compRetriever->getComponent(compInfo);
	if(!newComp)
		return;

    //add parameter
	newComp->init();
    
	compList.push_back(newComp);
}

void 
ComponentManager::update()
{
    printf("ComponentManager::update()\n");
	for(int i=0;i<appListSize;i++)
	{
		appList[i]->update();
	}
}

END_NAMESPACE_STB
