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


#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFString.h> 

namespace openvideo{
class    GL_TEXTURE_2D_Sink;
}

namespace stb{
class Video;
}

class  SoVideoBackground : public SoNode
{
   SO_NODE_HEADER(SoVideoBackground);

  public:
   static void initClass();

   SoVideoBackground();

   virtual ~SoVideoBackground(){}

   /** 
    * initializes the video overlay to a given window size.
    * @param width width of the window
    * @param height height of the window
    * @return TRUE if video background could be created.
    */
   bool initVideoBackground();

   /**
    * blits the actual video image into the frame buffer. This needs to be
    * implemented to draw the actual video background. It is called for each
    * frame and each viewer.
    */
   bool blitOverlay();

   SoSFString ovStbSink;

protected:
   virtual void GLRender(SoGLRenderAction *action);

   bool isInitialized;
   bool unableToInitialize;
   openvideo::GL_TEXTURE_2D_Sink* ovStbSinkNode;
   stb::Video*  video;
  
};


#endif //_SoVideoBackground_H_

