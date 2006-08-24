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
/** The header file for the SoImageCapture class.
 *
 * @author Bernhard Reitinger
 *
 * $Id: SoImageCapture.cxx 25 2005-11-28 16:11:59Z breiting $
 * @file                                                                   */
/* ======================================================================= */

#include <stb/components/viewer/SoImageCapture.h>
#include <stb/components/viewer/SoDisplay.h>
#include <stb/components/viewer/Viewer.h>
#include <stb/components/viewer/SoStudierstubeViewer.h>
#include <stb/kernel/Kernel.h>
#include <stb/kernel/ComponentManager.h>
#include <stb/components/video/Video.h>
#include <stb/components/event/event.h>
#include <stb/base/OS.h>

#include <stb/kernel/SceneManager.h>

#ifdef HAVE_OPENVIDEO
#  define ENABLE_VIDEOSINK
#  include <openvideo/nodes/VideoSink.h>
#  include <openvideo/nodes/VideoSinkSubscriber.h>
#  include <openvideo/State.h>
#else
#  pragma message(">>> HAVE_OPENVIDEO not defined - video background will not be available!")
#endif


#ifdef STB_IS_WINDOWS
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>

#ifdef STB_IS_WINCE
// load intrinsics for ARM instructions
#include <Armintr.h>
#else
#define _PreLoad(x)
#endif

#include <iostream>

BEGIN_NAMESPACE_STB

SO_NODE_SOURCE(SoImageCapture);

void
SoImageCapture::initClass()
{ 
    SO_NODE_INIT_CLASS(SoImageCapture, SoNode, "Node");
}

SoImageCapture::SoImageCapture() : _pressed(false), _image(NULL), _nr(0)
{
    SO_NODE_CONSTRUCTOR(SoImageCapture);
    _initialized = false;

    SO_NODE_ADD_FIELD(capture, (false));
    SO_NODE_ADD_FIELD(writeLocal, (true));
    SO_NODE_ADD_FIELD(path, (""));
    SO_NODE_ADD_FIELD(prefix, ("capture"));
}

void 
SoImageCapture::GLRender(SoGLRenderAction*)
{
    if(!_initialized)
    {
        _initialized = init();
    }
}


bool
SoImageCapture::init()
{
    Event* event=(Event*)(Kernel::getInstance()->getComponentManager()->load("Event"));
    if (!event)
        return false;

    Video* video = (Video*)(Kernel::getInstance()->getComponentManager()->load("Video"));
    if (!video)
        return false;

    video->vp_registerVideoUser(this);

    return true;
}


void
SoImageCapture::vu_init(const openvideo::Buffer& frame)
{
    _image = s_image_create(frame.getWidth(), 
                            frame.getHeight(), 
                            4 /* components */, NULL);
    
    if (!_image)
        logPrintE("Cannot allocate simage memory ...\n");
    
}


void
SoImageCapture::vu_update(const openvideo::Buffer& frame)
{

#ifdef HAVE_OPENVIDEO

    if (_pressed != capture.getValue())
    {
        if (_pressed)
        {
            int w = frame.getWidth();
            int h = frame.getHeight();
            int sz = w*h*4;
            unsigned char* buf;
            
            // image come vertically flipped !
            // FIXME: should be configurable
            bool flipv = true;
            if (flipv)
            {
                buf = new unsigned char[sz];
                
                int j=sz-w*4;
                for (int i=0; i<sz-w*4; i+=w*4, j-=w*4)
                {
                    memcpy(buf+i, frame.getPixels()+j, w*4);
                }
                
            }
            else
            {
                buf = (unsigned char*)frame.getPixels();
            }
            
            if (_image)
                s_image_set(_image, w, h, 4,
                            buf, 1 /* copy data */);
            
            if (writeLocal.getValue())
            {
                char fname[256];
                sprintf(fname, "%s%s%s_%03d.jpg",path.getValue().getString(), OS_SEP, prefix.getValue().getString(), _nr++);
                s_image_save(fname, _image, NULL);
            }
        }
        _pressed = capture.getValue();
    }
    
#endif
}

END_NAMESPACE_STB

//----------------------------------------------------------------------
// End of SoImageCapture.h
//----------------------------------------------------------------------
// Local Variables:
// mode: c++
// c-basic-offset: 4
// eval: (c-set-offset 'substatement-open 0)
// eval: (c-set-offset 'case-label '+)
// eval: (c-set-offset 'statement 'c-lineup-runin-statements)
// eval: (setq indent-tabs-mode nil)
// End:
//----------------------------------------------------------------------
