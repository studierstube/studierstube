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
/** The header file for the Viewer class.  
*  
* @author Denis Kalkofen  
*  
* $Id: Viewer.h 25 2005-11-28 16:11:59Z denis $  
* @file                                                                   */  
/* ======================================================================= */  

#ifndef _VIEWER_H_
#define _VIEWER_H_
#include <stb/base/macros.h>
#include <stb/kernel/Component.h>
#ifdef WIN32
    #ifdef STBVIEWER_EXPORTS
        #define STBVIEWER_API __declspec(dllexport)
    #else
        #define STBVIEWER_API __declspec(dllimport)
    #endif
#endif
#ifdef LINUX
    #define STBVIEWER_API
#endif

BEGIN_NAMESPACE_STB
/**
*	
*/
class Viewer : public stb::Component
{
public:
    /**
    *     The Constructor	
    */
    Viewer();
    /**
    *     The destructor.
    */
    ~Viewer();

	/// Called before the application is destructed.
	virtual bool init();
	/// Called before the application is destructed.
	virtual void shutDown();
    //
    virtual void setParameter(stb::string key, std::string value);

protected:	
    stb::string configFile; 
private:
	
};// class 
END_NAMESPACE_STB
#endif//_VIEWER_H_
//========================================================================
// End of Viewer.h 
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