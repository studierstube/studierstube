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
/** The header file for the ComponentManager class.  
*  
* @author Denis Kalkofen  
*  
* $Id: ComponentManager.h 25 2005-11-28 16:11:59Z denis $  
* @file                                                                   */  
/* ======================================================================= */  

#ifndef _COMPONENTMANAGER_H_
#define _COMPONENTMANAGER_H_

#include <vector>

#include <stb/base/macros.h>
#include <stb/kernel/Studierstube.h>

BEGIN_NAMESPACE_STB
class ComponentRetriever;
class ComponentInfo;
class Component;
class Application;
/**
*	
*/
class STB_API ComponentManager
{
public:
	/**
	*     The Constructor	
	*/
	ComponentManager();

	/**
	*     The destructor.
	*/
	~ComponentManager();

	void addComponent(ComponentInfo* compInfo);

    Component* load(std::string compName);

	///// Checks if new apps should be downloaded and started or if running apps should be stopped and destroyed.
	void update(); 
    
protected:
	std::vector<stb::Component*> compList;
	std::vector<stb::Application*> appList;
    std::vector<stb::ComponentInfo*> demandList;
    std::vector<stb::Component*> initList;
	ComponentRetriever *compRetriever;
	int appListSize;
    int initListSize;
    void initComponent(Component *comp);
    stb::Component* isLoaded(std::string compName);

private:
	
};// class 

END_NAMESPACE_STB



#endif//_COMPONENTMANAGER_H_
