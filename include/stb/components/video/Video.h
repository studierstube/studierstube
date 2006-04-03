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
/** The header file for the Video class.  
*  
* @author Denis Kalkofen  
*  
* $Id: Video.h 25 2005-11-28 16:11:59Z denis $  
* @file                                                                   */  
/* ======================================================================= */  

#ifndef _VIDEO_H_
#define _VIDEO_H_
#include <stb/base/macros.h>
#include <stb/kernel/ComponentThread.h>
#ifdef WIN32
#include <windows.h>
#endif

#ifdef LINUX
#include <GL/glx.h>
#endif

//#ifdef WIN32
//    #ifdef STBVIEWER_EXPORTS
//        #define STBVIEWER_API __declspec(dllexport)
//    #else
//        #define STBVIEWER_API __declspec(dllimport)
//    #endif
//#endif
//#ifdef LINUX
//    #define STBVIEWER_API
//#endif

namespace openvideo{
    class Manager;
    class GL_TEXTURE_2D_Sink;
}

BEGIN_NAMESPACE_STB
/**
*	
*/
class Video : public stb::ComponentThread
{
public:
    /**
    *     The Constructor	
    */
    Video();
    /**
    *     The destructor.
    */
    virtual ~Video();

	/// Called before the application is destructed.
	virtual bool init();
	/// Called before the application is destructed.
	virtual void shutDown();
    //
    virtual void setParameter(stb::string key, std::string value);

#ifdef WIN32
    virtual void setGLContext(HGLRC glContext,HDC dc);
#endif
#ifdef LINUX
    virtual void setGLContext(GLXDrawable drawable, GLXContext dc, Display* dsp);
#endif

    virtual void deleteGLContext();
    
    virtual void* getOpenVideoNode(const char *nodeName);

    virtual void aquire2DTextureSink(openvideo::GL_TEXTURE_2D_Sink* textureSink);
    virtual void release2DTextureSink(openvideo::GL_TEXTURE_2D_Sink* textureSink);
    virtual int getTextureID(openvideo::GL_TEXTURE_2D_Sink* textureSink);

protected:	
    stb::string configFile; 

    openvideo::Manager* ovManager;

    bool isGLContext;

    virtual void run();

private:
	
};// class 

END_NAMESPACE_STB


#endif//_VIDEO_H_
//========================================================================
// End of Video.h 
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
