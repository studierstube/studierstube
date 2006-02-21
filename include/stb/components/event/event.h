/* ========================================================================
 * Copyright (C) 2000-2005  Vienna University of Technology
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
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/** 
  *
  * @author Denis Kalkofen
  *
  * $Id: event.h 44 2005-07-18 15:03:44Z bara $
  * @file                                                                   */
 /* ======================================================================= */

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the EVENT_DLL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// EVENT_DLL_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.


#ifndef _EVENT_H_
#define _EVENT_H_
#include <stb/base/macros.h>
#include <stb/kernel/Component.h>

#ifdef WIN32
    #pragma warning(disable:4251)
    //#pragma warning(disable:4275)
    //#pragma warning(disable:4305)
    #pragma warning(disable:4786)
    #ifdef EVENT_EXPORTS
        #define EVENT_API __declspec(dllexport)
        #define EVENT_EXTERN
    #else
        #define EVENT_API __declspec(dllimport)
        #define EVENT_EXTERN extern
    #endif
#endif

#ifdef LINUX
    #define EVENT_API
#endif


BEGIN_NAMESPACE_STB
/**
*	
*/
class Event : public stb::Component
{
public:
    /**
    *     The Constructor	
    */
    Event();
    /**
    *     The destructor.
    */
    ~Event();

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
#endif//_EVENT_H_
//========================================================================
// End of Event.h 
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
