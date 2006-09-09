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
 * $Id: SoImageCapture.h 25 2005-11-28 16:11:59Z breiting $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _SOIMAGECAPTURE_H_
#define _SOIMAGECAPTURE_H_

#include <stb/components/viewer/ViewerDefines.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFString.h> 
#include <stb/base/macros.h>
#include <stb/kernel/VideoUser.h>
#ifndef __APPLE__
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif

#include <Inventor/fields/SoSFBool.h>

// Requires simage for storing jpg images
#include <simage.h>

#ifdef HAVE_OPENVIDEO

namespace openvideo{
    class VideoSink;
    class BufferSynchronizer;
}


BEGIN_NAMESPACE_STB

// Interface for capture callback
class VIEWER_API ImageCaptureListener
{
public:
    virtual ~ImageCaptureListener() {}
    virtual void newImage(const char *fname) = 0;
};


class Video;
class VideoBackgroundSinkSubscriber;
struct VideoBackgroundTexInfo;

/**
 * \brief This class takes the video background stream and captures
 * the current frame if capturing is triggered by "capture". The
 * easiest way is to conntect the field to a SoTrakEngine
 * (e.g. EventKeyboardSource).
 *
 * Currently, a jpg is written locally and subscribers get the path to
 * that image
 */

class VIEWER_API SoImageCapture : public SoNode, public stb::VideoUser
{
    friend class VideoBackgroundSinkSubscriber;

    SO_NODE_HEADER(SoImageCapture);

public:

    typedef std::vector<ImageCaptureListener*> ListenerVector;

    static void initClass();

    SoImageCapture();
    virtual ~SoImageCapture()
    {}

    bool init();

    // required by VideoUser
    virtual void vu_init(const openvideo::Buffer& frame);
    virtual void vu_update(const openvideo::Buffer& frame);
    virtual UPDATE_MODE vu_getUpdateMode() const  {  return VideoUser::UPDATE_BEFORE_RENDER;  }

    void GLRender(SoGLRenderAction*);

    /// methods for image listeners who can register for new image
    /// notification
    void registerListener(ImageCaptureListener *listener);
    void unregisterListener(ImageCaptureListener *listener);

    // public attributes
    SoSFBool capture;
    SoSFString path;
    SoSFString prefix;

protected:
    bool _initialized;
    bool _pressed;
    s_image *_image;
    int _nr;

    // vector storing listener
    ListenerVector _subscriber;
};

END_NAMESPACE_STB

#else
#ifdef WIN32
# pragma "Please set HAVE_OPENVIDEO if you include SoImageCapture.h"
#endif
#endif // HAVE_OPENVIDEO

#endif //_SoImageCapture_H_

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
