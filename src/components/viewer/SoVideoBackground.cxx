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
  * $Id: SoVideoBackground.cxx 25 2005-11-28 16:11:59Z denis $
  * @file                                                                   */
 /* ======================================================================= */

#include <stb/components/viewer/SoVideoBackground.h>
#include <stb/components/viewer/SoDisplay.h>
#include <stb/components/viewer/Viewer.h>
#include <stb/components/viewer/SoStudierstubeViewer.h>
#include <stb/kernel/Kernel.h>
#include <stb/kernel/ComponentManager.h>
#include <stb/components/video/Video.h>

#ifdef HAVE_OPENVIDEO
    #define ENABLE_GL_TEXTURE_2D_SINK
    #include <openvideo/nodes/GL_TEXTURE_2D_Sink.h>
#endif

#ifdef WIN32
	#include <windows.h>
	#define WIN32_LEAN_AND_MEAN
#endif

#include <GL/gl.h>			
#include <GL/glu.h>	

BEGIN_NAMESPACE_STB


SO_NODE_SOURCE(SoVideoBackground);


void
SoVideoBackground::initClass()
{ 
   SO_NODE_INIT_CLASS(SoVideoBackground, SoNode, "Node");
}

SoVideoBackground::SoVideoBackground()
{
   SO_NODE_CONSTRUCTOR(SoVideoBackground);
   SO_NODE_ADD_FIELD(ovStbSink,("")); // default red
   isInitialized=false;
   unableToInitialize=false;
}

void 
SoVideoBackground::GLRender(SoGLRenderAction * /*action*/)
{
    
    if(!isInitialized)
    {
	    isInitialized=initVideoBackground();
    }
    else
    {
	    blitOverlay();
    }
}


bool
SoVideoBackground::initVideoBackground()
{
#ifdef HAVE_OPENVIDEO
    SoDisplay* display=Viewer::findSoDisplay(this);
    if(!display)
        return false;

    video=(Video*)Kernel::getInstance()->getComponentManager()->load("Video");
    if(!video)
        return false;       
    ovStbSinkNode=(openvideo::GL_TEXTURE_2D_Sink*)video->getOpenVideoNode(ovStbSink.getValue().getString());

    SoStudierstubeViewer* stbViewer=display->getViewer();
    if(!stbViewer)
        return false;

    if(stbViewer->isOVGLContext())
        return true;

    stbViewer->setOVGLContext(video);
#endif
    return false;
}

bool
SoVideoBackground::blitOverlay() 
{
#ifdef HAVE_OPENVIDEO
    if(!ovStbSinkNode ) 
        return false;

    video->aquire2DTextureSink(ovStbSinkNode);

    /////////preGLCalls()
    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    glDisable(GL_LIGHTING);
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
    ///////////////////////    

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glEnable(GL_TEXTURE_2D);



    // redraw texture
    glBindTexture(GL_TEXTURE_2D,  video->getTextureID(ovStbSinkNode));
    glBegin(GL_QUADS);
    // video backplate
    glTexCoord2f(ovStbSinkNode->t_u0,ovStbSinkNode->t_v0); glVertex3f(-1.0f, -1.0f,  0.0f);
    glTexCoord2f(ovStbSinkNode->t_u1,ovStbSinkNode->t_v0); glVertex3f( 1.0f, -1.0f,  0.0f);
    glTexCoord2f(ovStbSinkNode->t_u1,ovStbSinkNode->t_v1); glVertex3f( 1.0f,  1.0f,  0.0f);
    glTexCoord2f(ovStbSinkNode->t_u0,ovStbSinkNode->t_v1); glVertex3f(-1.0f,  1.0f,  0.0f);

    glEnd();

    glDisable(GL_TEXTURE_2D);

    ///postGLCalls()
    glPopAttrib();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    //////////////////////
    video->release2DTextureSink(ovStbSinkNode);
#endif
    return true;
}

END_NAMESPACE_STB

