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
/** The header file for the MagicLens class.  
*  
* @author Denis Kalkofen  
*  
* $Id: MagicLens.h 25 2005-11-28 16:11:59Z denis $  
* @file                                                                   */  
/* ======================================================================= */  

#ifndef _MagicLens_H_
#define _MagicLens_H_
#include <stb/base/macros.h>
#include <stb/kernel/Component.h>


BEGIN_NAMESPACE_STB
class Starlight;
/**
*	
*/
class MagicLens : public stb::Component
{
public:
    /**
    *     The Constructor	
    */
    MagicLens();
    /**
    *     The destructor.
    */
    ~MagicLens();

	/// Called before the application is destructed.
	virtual bool init();
	/// Called before the application is destructed.
	virtual void shutDown();
    //
    virtual void setParameter(stb::string key, std::string value);
    
    static Starlight* getStarlight();
protected:	
    stb::string configFile; 
    static Starlight* starlight;
private:
	
};// class 
END_NAMESPACE_STB
#endif//_MagicLens_H_
//========================================================================
// End of MagicLens.h 
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
