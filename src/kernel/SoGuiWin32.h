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
/** The header file for the SoGuiWin32 class.  
*  
* @author Denis Kalkofen  
*  
* $Id: SoGuiWin32.h 25 2005-11-28 16:11:59Z denis $  
* @file                                                                   */  
/* ======================================================================= */  

#ifndef _SOGUIWIN32_H_
#define _SOGUIWIN32_H_
#include "common/macros.h"
#include "SoGuiBase.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

BEGIN_NAMESPACE_STB
/**
*	
*/
class SoGuiWin32 : public SoGuiBase
{
public:
	/**
	*     The Constructor	
	*/
	SoGuiWin32();

	/**
	*     The destructor.
	*/
	~SoGuiWin32();

	virtual void parseConfiguration(TiXmlAttribute* attribute);

	virtual void setDefaultBinding();
	
	virtual void init();

	virtual void mainLoop();
	
protected:	
	virtual enum GUI_BINDING {
		SOQT=0,
		SOWIN=1
	};

	GUI_BINDING curGuiBinding;

	HMODULE libHandle;
	
	
	void loadSoQt();
	void loadSoWin();
	void mainLoopSoWin();
	void mainLoopSoQt();

private:
	
};// class 

END_NAMESPACE_STB



#endif//_SOGUIWIN32_H_
