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
/** The header file for the SoVideoBackground class.
  *
  * @author Denis Kalkofen
  *
  * $Id: SoVideoBackground.h 25 2005-11-28 16:11:59Z denis $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SOVIDEOBACKGROUND_H_
#define _SOVIDEOBACKGROUND_H_

#include <stb/components/viewer/guiDefines.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFString.h> 
#include <stb/base/macros.h>
#include <stb/kernel/VideoUser.h>


namespace openvideo{
  class VideoSink;
}


BEGIN_NAMESPACE_STB


class Video;
class VideoBackgroundSinkSubscriber;
struct VideoBackgroundTexInfo;


class  SoVideoBackground : public SoNode, public stb::VideoUser
{
friend class VideoBackgroundSinkSubscriber;

   SO_NODE_HEADER(SoVideoBackground);

  public:
   static void initClass();

   SoVideoBackground();

   virtual ~SoVideoBackground(){}

   bool init();

   /**
    * blits the actual video image into the frame buffer. This needs to be
    * implemented to draw the actual video background. It is called for each
    * frame and each viewer.
    */
   bool blitOverlay();


   virtual void vu_init(const openvideo::Buffer& frame);
   virtual void vu_update(const openvideo::Buffer& frame);
   virtual UPDATE_MODE vu_getUpdateMode() const  {  return VideoUser::UPDATE_BEFORE_RENDER;  }


   SoSFString ovStbSink;

protected:
   virtual void GLRender(SoGLRenderAction *action);
   //bool createTexture();
   bool createTexture(const openvideo::Buffer& buffer);
   //void updateTexture();
   void updateTexture(const openvideo::Buffer& buffer);
   void drawTexture();
   void blitIntoVideoMemory();
   void drawVideoBackground();

   //openvideo::VideoSink* videoSink;
   //VideoBackgroundSinkSubscriber*	videoSinkSubscriber;
   VideoBackgroundTexInfo*	texInfo;
   bool						initialized;
};

END_NAMESPACE_STB

#endif //_SoVideoBackground_H_

