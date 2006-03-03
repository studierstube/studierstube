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
#include <openvideo/Manager.h>
#define ENABLE_GL_TEXTURE_2D_SINK
#include <openvideo/nodes/GL_TEXTURE_2D_Sink.h>
#include <GL/gl.h>			
#include <GL/glu.h>	

CREATE_COMPONENT_FUNC(Video)

BEGIN_NAMESPACE_STB

Video::Video()
{
   configFile="";
   isGLContext=false;
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
    printf("Video::init()\n");
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
        configFile=value;
    }
    //else if()
    //{
    //}
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
    ovManager->initTraverasal();
    ovManager->run();

    return;
}

 void 
 Video::deleteGLContext(HGLRC glContext)
 {
     printf("delete context\n");
 }

void 
Video::setGLContext(HGLRC glContext,HDC dc)
{
     if(isGLContext)
        return;
    isGLContext=true;
    ovManager->setGLContext(glContext,dc);
}

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
