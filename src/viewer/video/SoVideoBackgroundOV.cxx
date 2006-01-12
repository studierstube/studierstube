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
/** The header file for the SoVideoBackgroundOV class.
  *
  * @author Denis Kalkofen
  *
  * $Id: SoVideoBackgroundOV.cxx 25 2005-11-28 16:11:59Z denis $
  * @file                                                                   */
 /* ======================================================================= */

#include "SoVideoBackgroundOV.h"
#include <StbKernel/openvideo/OpenVideoContext.h>

#ifdef USE_OPENVIDEO
	//openvideo node
#include <nodes/GL_TEXTURE_2D_Sink/GL_TEXTURE_2D_Sink.h>
#ifdef _DEBUG
#pragma comment(lib,"openVideod.lib")
#else
#pragma comment(lib,"openVideo.lib")
#endif
#endif //USE_OPENVIDEO

#ifdef WIN32
	#include <windows.h>
	#define WIN32_LEAN_AND_MEAN
#endif

#include <GL/gl.h>			
#include <GL/glu.h>	

SO_NODE_SOURCE(SoVideoBackgroundOV);


void
SoVideoBackgroundOV::initClass()
{ 
   SO_NODE_INIT_CLASS(SoVideoBackgroundOV, SoNode, "Node");
}

SoVideoBackgroundOV::SoVideoBackgroundOV()
{
   SO_NODE_CONSTRUCTOR(SoVideoBackgroundOV);
   SO_NODE_ADD_FIELD(ovStbSink,("")); // default red
   isInitialized=false;
   unableToInitialize=false;
}

void 
SoVideoBackgroundOV::GLRender(SoGLRenderAction *action)
{
    //printf("SoVideoBackgroundOV::GLRender(SoGLRenderAction *action)\n");
    if(!isInitialized)
    {
	isInitialized=initVideoBackground();
    }
    else
    {
	blitOverlay();
    }
}

#ifndef USE_OPENVIDEO
bool
SoVideoBackgroundOV::initVideoBackground()
{
	return true;
}

bool
SoVideoBackgroundOV::blitOverlay() 
{
   return true;
}

#else /////////// USE_OPENVIDEO  ///////////////////////

bool
SoVideoBackgroundOV::initVideoBackground()
{
    //setup displays timer to update the video
    if(unableToInitialize) 
	return false;
    
    
    
    if(!this->ovStbSinkNode){
	OpenVideoContext* ovContext=OpenVideoContext::getInstance();
	if(!ovContext->isOpenVideoStarted()){
	    return false;
	}
	
	ovStbSinkNode=(openvideo::GL_TEXTURE_2D_Sink*)ovContext->getOpenVideoNode((ovStbSink.getValue()).getString());
	if(!ovStbSinkNode){
	    printf("STB_ERROR: unable to initialize SoOpenVideoBackground\n");
	    unableToInitialize=true;
	    return false;
	}
    }


	if(!ovStbSinkNode->isStarted){
		return false;
		}
	
	
	/*for(int i=0;i<(int)(SoDisplay::).size();i++)
	{
		//search for the display this videobackground node exists in 
		if(SoDisplay::displays[i]->find(this)){
			SoDisplay::displays[i]->setTimer();
			SoDisplay::displays[i]->viewer->setClearBeforeRender(FALSE);
			break;
		}
		}*/

	return true;
}

bool
SoVideoBackgroundOV::blitOverlay() 
{
	
	if(!ovStbSinkNode ) 
		return false;

	ovStbSinkNode->acquire();

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
	glBindTexture(GL_TEXTURE_2D, this->ovStbSinkNode->get_video_texture_id());
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
	ovStbSinkNode->release();
   return true;
}

#endif //#ifndef USE_OPENVIDEO
