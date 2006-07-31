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
#include <stb/kernel/Profiler.h>
#include <stb/base/OS.h>
#include <openvideo/Manager.h>


// this assumes that OpenVideo was compiled
// with the videosink option enabled...
//
#define ENABLE_VIDEOSINK


#include <openvideo/nodes/VideoSinkSubscriber.h>
#include <openvideo/nodes/VideoSink.h>
#include <openvideo/Manager.h>
#include <openvideo/nodes/GL_TEXTURE_2D_Sink.h>


#include <assert.h>
#ifndef __APPLE__
#include <GL/gl.h>			
#include <GL/glu.h>	
#else
#include <OpenGL/gl.h>			
#include <OpenGL/glu.h>	
#endif

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
		bufferSynchronizer.assign(curState->getCurrentBuffer());

		if(firstFrame)
		{
			if(openvideo::Buffer* buffer = bufferSynchronizer.getLocked())
			{
				video->setVideoFormat(*buffer);
				buffer->unlock();
				firstFrame = false;
			}
		}
		else
		{
			if(openvideo::Buffer* buffer = bufferSynchronizer.getLocked())
			{
				video->setNewVideoFrame(*buffer);
				buffer->unlock();
			}
		}
	}


	openvideo::Buffer* getCurrentFrameLocked()  {  return bufferSynchronizer.getLocked();  }

	bool gotFirstFrame() const  {  return !firstFrame;  }

	void initPixelFormats()
	{
		pixelFormats.push_back(openvideo::FORMAT_R8G8B8);
		pixelFormats.push_back(openvideo::FORMAT_B8G8R8);
		pixelFormats.push_back(openvideo::FORMAT_R8G8B8X8);
		pixelFormats.push_back(openvideo::FORMAT_B8G8R8X8);
		pixelFormats.push_back(openvideo::FORMAT_L8);
		pixelFormats.push_back(openvideo::FORMAT_R5G6B5);
	}

private:
	Video* video;
	bool firstFrame;
	openvideo::BufferSynchronizer bufferSynchronizer;
};



Video::Video()
{
   configFile="";
   isGLContext=false;
   runSingleThreaded = false;
   ovInitialized = false;

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

	// register so we get the kes_beforeRender() event
	//
	stb::Kernel::getInstance()->registerForKernelEvents(this);

	if(runSingleThreaded)
		initOpenVideo();
	else
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
	else if(key=="single-threaded")
	{
		if(value=="true" || value=="TRUE")
			runSingleThreaded = true;
	}
}

/// Called before the application is destructed.
void 
Video::shutDown()
{

}


bool
Video::initOpenVideo()
{
	if(!ovManager->parseConfiguration(stb::Kernel::getInstance()->getConfig(configFile).c_str()))
		return false;

	ovManager->initTraversal();

	// register with openvideo as the one and only video input in Studierstube
	//
	videoSinkSubscriber = new Stb4VideoSinkSubscriber(this);
	if(openvideo::VideoSink* sink = reinterpret_cast<openvideo::VideoSink*>(ovManager->getNode(ovSinkName.c_str())))
		sink->subscribe(videoSinkSubscriber);
	else
		return false;

	ovInitialized = true;
	return true;
}


void
Video::run()
{
    if(!initOpenVideo())
		return;

    ovManager->run();
}


void
Video::vp_registerVideoUser(VideoUser* videouser)
{
	switch(videouser->vu_getUpdateMode())
	{
	case VideoUser::UPDATE_IMMEDIATELY:
		for(VideoUserVector::iterator it=videoUsersImmediate.begin(); it!=videoUsersImmediate.end(); it++)
			if(*it == videouser)
				return;
		videoUsersImmediate.push_back(videouser);
		break;


	case VideoUser::UPDATE_BEFORE_RENDER:
		for(VideoUserVector::iterator it=videoUsersBeforeRender.begin(); it!=videoUsersBeforeRender.end(); it++)
			if(*it == videouser)
				return;
		videoUsersBeforeRender.push_back(videouser);
		break;
	}


	// if the format is already known tell it right now
	// otherwise setOVFormat() will do this later on
	if(video_format)
		videouser->vu_init(*video_format);
}


void
Video::vp_unregisterVideoUser(VideoUser* videouser)
{
	for(VideoUserVector::iterator it=videoUsersImmediate.begin(); it!=videoUsersImmediate.end(); it++)
		if(*it == videouser)
		{
			videoUsersImmediate.erase(it);
			return;
		}

	for(VideoUserVector::iterator it=videoUsersBeforeRender.begin(); it!=videoUsersBeforeRender.end(); it++)
		if(*it == videouser)
		{
			videoUsersBeforeRender.erase(it);
			return;
		}
}


void
Video::setVideoFormat(const openvideo::Buffer& format)
{
	video_format = new openvideo::Buffer;
	*video_format = format;

	// tell all already registered video users about this format
	//
	for(size_t i=0; i<videoUsersImmediate.size(); i++)
		videoUsersImmediate[i]->vu_init(*video_format);

	for(size_t i=0; i<videoUsersBeforeRender.size(); i++)
		videoUsersBeforeRender[i]->vu_init(*video_format);
}


void
Video::kes_beforeRender()
{
	if(!ovInitialized)
		return;

	if(runSingleThreaded)
	{
		STB_PROFILER_AUTOMEASURE(OPEN_VIDEO);
		assert(ovManager);
		ovManager->updateSingleThreaded();
	}

	if(!videoSinkSubscriber || !videoSinkSubscriber->gotFirstFrame())
		return;

	if(openvideo::Buffer* buffer = videoSinkSubscriber->getCurrentFrameLocked())
	{
		notifyVideoUsers(videoUsersBeforeRender, *buffer);
		buffer->unlock();
	}
}


void
Video::setNewVideoFrame(const openvideo::Buffer& frame)
{
	notifyVideoUsers(videoUsersImmediate, frame);
}


void
Video::notifyVideoUsers(VideoUserVector& videoUsers, const openvideo::Buffer& frame)
{
	for(size_t i=0; i<videoUsers.size(); i++)
		videoUsers[i]->vu_update(frame);
}


const openvideo::Buffer*
Video::getCurrentFrameLocked()
{
	return videoSinkSubscriber->getCurrentFrameLocked();
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
