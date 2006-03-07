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
/** The header file for the Kernel class.
*
* @author Denis Kalkofen
*
* $Id: Kernel.h 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#ifndef _KERNEL_H_
#define _KERNEL_H_

#include <stb/base/macros.h>
#include <stb/base/string.h>
#include <stb/kernel/Studierstube.h>

class TiXmlElement;
class SoSensor;

#include <stb/kernel/Scheduler.h>



/**
*
*/
BEGIN_NAMESPACE_STB
class Config;
class ComponentManager;
class SceneManager;
class ComponentInfo;
class SoTrakEngineInterface;

class STB_API Kernel
{
public:
    /**
    *	Returns the Kernel. it creates a new kernel, if no one exists.
    */
    static Kernel* getInstance();

    /**
    *	The destructor.
    */
    ~Kernel();


    /**
    *	Creates and starts a new kernel. 
    *	The function also initializes ACE and Inventor.
    */
    void start(int argc,char* argv[]);

    /**
    *	Stop the kernel. --> Notice: stopKernel will not delete the Kernel!
    */
    void stop();

    /*
    *	                                                                   
    */
    void logDebug(stb::string  nStr);
    void log(stb::string nStr);
    void logEx(const char* nStr, ...);

    /*
    *	                                                                   
    */
    void parseConfiguration(TiXmlElement* root);

    static void update( void * data, SoSensor * sensor);

    void addComponent(ComponentInfo* compInfo);

    stb::SceneManager* getSceneManager();

    stb::ComponentManager* getComponentManager(){return componentManager;}

public:
    // define here search paths for config files, public available
    stb::string stb_config_path1;  //"/usr/share/stb/"
    stb::string stb_config_path2;  // "/usr/local/share/stb/"
    stb::string stb_home;          // ".stb/"
    stb::string kernel_config_file;// "kernel.xml"

protected:	
    ///////////////////// 
    /**
    *	The constructor.
    */
    Kernel();

    /**
    *	THE KERNEL
    */
    static Kernel *instance;

    /*
    *		                                                                   
    */
    stb::Config* config;	
    stb::Scheduler *scheduler;
    stb::ComponentManager *componentManager;
    stb::SceneManager *sceneManager;
    /*
    *                                                                    
    */
    enum LOG_MODE {
	    OFF=0,
	    FILE=1,
	    CONSOLE=2
    };	
    LOG_MODE logMode;

    /*
    *                                                                    
    */
    stb::string logFile;

    stb::string getKernelConfig(int argc, char* argv[]);
    /*
    *                                                                    
    */

};// class 

END_NAMESPACE_STB

#endif //_STBKERNEL_H_
