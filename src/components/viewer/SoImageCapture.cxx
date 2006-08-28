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
#define ENABLE_VIDEOSINK
#include <openvideo/nodes/VideoSink.h>
#include <openvideo/nodes/VideoSinkSubscriber.h>
#include <openvideo/State.h>

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


void bgr2rgb(unsigned char* out, const unsigned char* in, unsigned int n);

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
                            openvideo::PixelFormat::getBitsPerPixel(frame.getFormat())/8 /* components */, NULL);
    
    if (!_image)
        logPrintE("Cannot allocate simage memory ...\n");
    
}


void
SoImageCapture::vu_update(const openvideo::Buffer& frame)
{
    int components = openvideo::PixelFormat::getBitsPerPixel(frame.getFormat())/8;
    if (_pressed != capture.getValue())
    {
        if (_pressed)
        {
            logPrintI("Capture ...\n");
            int w = frame.getWidth();
            int h = frame.getHeight();
            int sz = w*h*components;
            unsigned char* buf;
            
            // image comes flipped, and must therefore be flipped
            bool flipv = true;
            if (flipv)
            {
                buf = new unsigned char[sz];
                
                int j=sz-w*components;
                for (int i=0; i<sz-w*components; i+=w*components, j-=w*components)
                {
                    memcpy(buf+i, frame.getPixels()+j, w*components);
                }
                
            }
            else
            {
                buf = (unsigned char*)frame.getPixels();
            }

			//s_image_set_component_order(_image, SIMAGE_ORDER_RGB);

            
            if (_image)
                s_image_set(_image, w, h, components,
                            buf, 1 /* copy data */);
            
            char fname[256];
            sprintf(fname, "%s%s%s_%03d.jpg",path.getValue().getString(), OS_SEP, prefix.getValue().getString(), _nr++);
            s_image_save(fname, _image, NULL);
            
            // notify subscribers
            for (ListenerVector::iterator it=_subscriber.begin(); it!=_subscriber.end(); it++)
            {
                (*it)->newImage(fname);
            }
        }
        _pressed = capture.getValue();
    }
}

void 
SoImageCapture::registerListener(ImageCaptureListener *listener)
{

    for (ListenerVector::iterator it=_subscriber.begin(); it != _subscriber.end(); it++)
    {
        if (*it == listener)
            return;
    }
    _subscriber.push_back(listener);
}

void 
SoImageCapture::unregisterListener(ImageCaptureListener *listener)
{
    for (ListenerVector::iterator it=_subscriber.begin(); it != _subscriber.end(); it++)
    {
        if (*it == listener)
        {
            _subscriber.erase(it);
            return;
        }
    }
}

END_NAMESPACE_STB

#endif  // HAVE_OPENVIDEO

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
