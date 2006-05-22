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
#include <stb/base/OS.h>
#include <stb/base/datatypes.h>
#include <stb/kernel/ComponentThread.h>
#include <stb/kernel/VideoProvider.h>


#ifdef STB_IS_WINDOWS
#  include <windows.h>
#elif defined(STB_IS_LINUX)
#  include <GL/glx.h>
#endif


namespace openvideo{
    class Manager;
    class GL_TEXTURE_2D_Sink;
}

BEGIN_NAMESPACE_STB


class Stb4VideoSinkSubscriber;


/**
*	
*/
class Video : public stb::ComponentThread, public stb::VideoProvider
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

#ifdef STB_IS_WINDOWS
    virtual void setGLContext(HGLRC glContext,HDC dc);
#elif defined(STB_IS_LINUX)
    virtual void setGLContext(GLXDrawable drawable, GLXContext dc, Display* dsp);
#endif

    virtual void deleteGLContext();
    
    virtual void* getOpenVideoNode(const char *nodeName);

    virtual void aquire2DTextureSink(openvideo::GL_TEXTURE_2D_Sink* textureSink);
    virtual void release2DTextureSink(openvideo::GL_TEXTURE_2D_Sink* textureSink);
    virtual int getTextureID(openvideo::GL_TEXTURE_2D_Sink* textureSink);


	// Declare that this component is a video provider
	virtual VideoProvider* getVideoProviderInterface()  {  return this;  }


	/// Implement stb::VideoProvider interface
	virtual void vp_registerVideoUser(VideoUser* videouser);
	virtual void vp_unregisterVideoUser(VideoUser* videouser);


	void setOVFormat(VIDEO_FRAME& format);

	void setNewFrame(VIDEO_FRAME& format);

protected:	
    stb::string configFile,
				ovSinkName;

    openvideo::Manager* ovManager;

    bool isGLContext;

    virtual void run();

private:
	Stb4VideoSinkSubscriber*	videoSinkSubscriber;
	VideoUserVector	videousers;

	VIDEO_FRAME *video_format;
};


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
