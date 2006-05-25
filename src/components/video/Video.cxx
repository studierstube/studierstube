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
* $Id: Video.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#include <stb/components/video/Video.h>
#include <stb/kernel/Kernel.h>
#include <stb/kernel/VideoUser.h>
#include <stb/base/OS.h>
#include <openvideo/Manager.h>


// this assumes that OpenVideo was compiled
// with these two options enabled...
//
#define ENABLE_GL_TEXTURE_2D_SINK
#define ENABLE_VIDEOSINK


#include <openvideo/nodes/VideoSinkSubscriber.h>
#include <openvideo/nodes/VideoSink.h>
#include <openvideo/Manager.h>
#include <openvideo/nodes/GL_TEXTURE_2D_Sink.h>


#include <assert.h>
#include <GL/gl.h>			
#include <GL/glu.h>	


CREATE_COMPONENT_FUNC(Video)


BEGIN_NAMESPACE_STB


// we have to do this in a separate class because both
// openvideo::Node and stb::Component defined 'bool init()'
// and the compiler will complain when deriving from both...
//
class Stb4VideoSinkSubscriber : public openvideo::VideoSinkSubscriber
{
public:
	Stb4VideoSinkSubscriber(Video* videoComp) : video(videoComp)
	{
		firstFrame = true;
	}

	void update(openvideo::State* curState)
	{
		if(firstFrame)
		{
			if(openvideo::Buffer* buffer = curState->getCurrentBuffer())
			{
				buffer->lock();

				frame.width = curState->width;
				frame.height = curState->height;
				frame.format = (PIXEL_FORMAT)curState->format;		// this works because Stb's pixel formats are compatible to OpenVideo's...
				frame.buffer = buffer->getPixels();

				video->setOVFormat(frame);
				firstFrame = false;

				buffer->unlock();
			}
		}
		else
		{
			if(openvideo::Buffer* buffer = curState->getCurrentBuffer())
			{
				buffer->lock();

				// video format should never change...
				//
				assert(frame.width == curState->width);
				assert(frame.height == curState->height);
				assert(frame.format == (PIXEL_FORMAT)curState->format);

				frame.buffer = buffer->getPixels();
				video->setNewFrame(frame);

				buffer->unlock();
			}
		}
	}


	void initPixelFormats()
	{
		pixelFormats.push_back(openvideo::FORMAT_R8G8B8);
		pixelFormats.push_back(openvideo::FORMAT_B8G8R8);
		pixelFormats.push_back(openvideo::FORMAT_R8G8B8X8);
		pixelFormats.push_back(openvideo::FORMAT_B8G8R8X8);
		pixelFormats.push_back(openvideo::FORMAT_L8);
	}

private:
	Video* video;
	bool firstFrame;
	VIDEO_FRAME frame;
};



Video::Video()
{
   configFile="";
   isGLContext=false;

   videoSinkSubscriber = NULL;
   video_format = NULL;

   ovManager=openvideo::Manager::getInstance(); 
}

Video::~Video()
{
   //nil
}

/// Called before the application is destructed.
bool 
Video::init()
{
    // FIXME: insert log message as soon as the logger is done
    //    stb::Kernel::getInstance()->logDebug("Stb:: init Video\n");
    if(isInit)
        return true;
    isInit=true;

    retrieveParameter();
    start();

    return isInit;
}

void 
Video::setParameter(stb::string key, std::string value)
{
    if(key=="configFile")
    {
        configFile = value;
    }
    else if(key=="ovSinkName")
    {
		ovSinkName = value;
    }
}

/// Called before the application is destructed.
void 
Video::shutDown()
{

}


void
Video::run()
{
    if(!ovManager->parseConfiguration(configFile.c_str())){
        return;
    }
    ovManager->initTraversal();

	videoSinkSubscriber = new Stb4VideoSinkSubscriber(this);
	if(openvideo::VideoSink* sink = reinterpret_cast<openvideo::VideoSink*>(ovManager->getNode(ovSinkName.c_str())))
		sink->subscribe(videoSinkSubscriber);

    ovManager->run();

    return;
}

 void 
 Video::deleteGLContext()
 {
     ovManager->deleteGLContext();
 }

#ifdef STB_IS_WINDOWS
void 
Video::setGLContext(HGLRC glContext,HDC dc)
{
     if(isGLContext)
        return;
    isGLContext=true;
    ovManager->setGLContext(glContext,dc);
}
#endif

#ifdef STB_IS_LINUX
void 
Video::setGLContext(GLXDrawable drawable, GLXContext dc, Display* dsp) {
    if(isGLContext)
        return;
    isGLContext=true;
    ovManager->setGLContext(drawable, dc, dsp);
}
#endif

void* 
Video::getOpenVideoNode(const char *nodeName)
{
    return ovManager->getNode(nodeName);
}

void 
Video::aquire2DTextureSink(openvideo::GL_TEXTURE_2D_Sink* textureSink)
{
    textureSink->acquire();
}

void 
Video::release2DTextureSink(openvideo::GL_TEXTURE_2D_Sink* textureSink)
{
    textureSink->release();
}

int
Video::getTextureID(openvideo::GL_TEXTURE_2D_Sink* textureSink)
{
       return textureSink->get_video_texture_id();
}


void
Video::vp_registerVideoUser(VideoUser* videouser)
{
	for(VideoUserVector::iterator it=videousers.begin(); it!=videousers.end(); it++)
		if(*it == videouser)
			return;

	videousers.push_back(videouser);

	// if the format is already known tell it right now
	// otherwise setOVFormat() will do this later on
	if(video_format)
		videouser->vu_init(*video_format);
}


void
Video::vp_unregisterVideoUser(VideoUser* videouser)
{
	for(VideoUserVector::iterator it=videousers.begin(); it!=videousers.end(); it++)
		if(*it == videouser)
		{
			videousers.erase(it);
			return;
		}
}


void
Video::setOVFormat(VIDEO_FRAME& format)
{
	video_format = new VIDEO_FRAME;
	*video_format = format;

	// tell all already registered video users about this format
	//
	for(VideoUserVector::iterator it=videousers.begin(); it!=videousers.end(); it++)
		(*it)->vu_init(*video_format);
}


void
Video::setNewFrame(VIDEO_FRAME& format)
{
	for(VideoUserVector::iterator it=videousers.begin(); it!=videousers.end(); it++)
		(*it)->vu_update(*video_format);
}



END_NAMESPACE_STB
//========================================================================
// End of Video.cxx
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
