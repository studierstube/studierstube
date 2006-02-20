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
/** The header file for the ComponentInfo class.  
*  
* @author Denis Kalkofen  
*  
* $Id: ComponentInfo.h 25 2005-11-28 16:11:59Z denis $  
* @file                                                                   */  
/* ======================================================================= */  
#ifndef _COMPONENTINFO_H_
#define _COMPONENTINFO_H_

#include <stb/base/OS.h>
#include <stb/base/string.h>
#include <stb/base/macros.h>
#include <map>

class TiXmlElement;
#include <stb/kernel/Component.h>

BEGIN_NAMESPACE_STB

/**
*	
*/
class ComponentInfo
{
public:
   friend class Component;
   /**
    *     The Constructor	
    */
   ComponentInfo();
   
   /**
    *     The destructor.
    */
   ~ComponentInfo();
   
   void parseConfiguration(TiXmlElement* element);
   
   stb::string getName(){return name;}
   
   stb::string getLibName(){return libName;}
   
   void setHModule(hModule aLibHandle);
   
   enum AVAILABILITY{
       ON_LOAD=0,
       ON_DEMAND=1
   };
   ComponentInfo::AVAILABILITY getAvailability();

   void setAvailability(AVAILABILITY avl);

   void setTypeID(stb::string type);
protected:	
   stb::string typeID;
   stb::string name;
   stb::string libName;
   hModule libHandle;
   AVAILABILITY availability;
   std::map<stb::string,stb::string> parameter;
private:
	
};// class 

END_NAMESPACE_STB



#endif//_COMPONENTINFO_H_

//========================================================================
// End of file
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
