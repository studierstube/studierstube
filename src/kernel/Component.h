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
/** The header file for the Component class.  
*  
* @author Denis Kalkofen  
*  
* $Id: Component.h 25 2005-11-28 16:11:59Z denis $  
* @file                                                                   */  
/* ======================================================================= */  

#ifndef _COMPONENT_H_
#define _COMPONENT_H_
#include "Studierstube.h"
#include "common/macros.h"
#include "common/string.h"

BEGIN_NAMESPACE_STB

// class Kernel;

/**
*	
*/
class ComponentInfo;

class STB_API Component
{
public:
	/**
	*     The destructor.
	*/
	virtual ~Component();

	/// Called before the application is destructed.
	virtual bool init() = 0;

	/// Called before the application is destructed.
	virtual void shutDown() = 0;

    virtual void setParameter(stb::string key, std::string value);

    virtual void setComponentInfo(ComponentInfo* info);

    virtual void retrieveParameter();
protected:	
    /**
    *     The Constructor	
    */
    Component();

	Kernel		*kernel_;
	ComponentInfo	*compInfo_;

private:
	
};// class 

END_NAMESPACE_STB


#define CREATE_COMPONENT_FUNC(CLASSNAME) \
extern "C" {																						\
\
__declspec(dllexport) stb::CLASSNAME* createComponent()\
{																									\
    return new stb::CLASSNAME;																	\
}																									\
\
}

#endif//_COMPONENT_H_
