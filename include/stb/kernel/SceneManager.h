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
/** The header file for the SceneManager class.  
*  
* @author Denis Kalkofen  
*  
* $Id: SceneManager.h 25 2005-11-28 16:11:59Z denis $  
* @file                                                                   */  
/* ======================================================================= */  

#ifndef _SCENEMANAGER_H_
#define _SCENEMANAGER_H_
#include <stb/base/macros.h>
#include <stb/kernel/Studierstube.h>


class SoSeparator;
class SoGroup;
class SbName;
class SoNode;


BEGIN_NAMESPACE_STB
class STB_API SceneManager
{
public:
	/**
	*     The Constructor	
	*/
	SceneManager();

	/**
	*     The destructor.
	*/
	~SceneManager();

	void registerApp(SoSeparator *appRoot);
	
	void update();

	enum TOUCHROOT{
		ROOT=0,
		SCENE=1,
		DISPLAY=2
	};

	void setTouchRoot(TOUCHROOT touchNode);

	SoSeparator* getDisplayRoot();
    
    void setDisplay(SoGroup *display);

    void setTrackerSource(SoNode *otSource);

    SoSeparator* getSceneRoot();

protected:	
	SoSeparator* root;
	SoNode* touchRoot;
	SoSeparator* trackRoot;
	SoSeparator* sceneRoot;
	SoSeparator* displayRoot;
private:
	
};// class 
END_NAMESPACE_STB



#endif//_SCENEMANAGER_H_
