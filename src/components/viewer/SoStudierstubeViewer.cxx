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
/** The header file for the SoStudierstubeViewer class.
*
* @author Denis Kalkofen
*
* $Id: SoStudierstubeViewer.cxx 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#include <stb/components/viewer/SoStudierstubeViewer.h>
#include <stb/components/viewer/SoDisplay.h>
#include <stb/components/viewer/Viewer.h>
#include <stb/base/OS.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>

#include SOGUI_H
#include SOGUI_CURSOR_H
#ifdef USE_SOQT
#  include GuiWidget_H
#endif

#include <iostream>
#include <GL/gl.h>
#include <errno.h>


BEGIN_NAMESPACE_STB


//----------------------------------------------------------------------------
// The constructor of the viewer.
SoStudierstubeViewer::SoStudierstubeViewer(GuiWidget widget) :
	SoGuiExaminerViewer(widget, "Studierstube", TRUE, SoGuiFullViewer::BUILD_ALL)
{
    isWindowDecorationActive=TRUE;
    headlight=NULL;
    shareGLContextWithVideo=false;
    // SoGLRenderAction* renderAction = this->getGLRenderAction();
 //   renderAction->setSmoothing(TRUE);
    // in order to support COINs SORTED_LAYER_BLEND transparency Mode, 
    // the Alpha bits must be set
    this->setAlphaChannel(TRUE);
#ifdef STB_IS_WINDOWS
    curDC=NULL;
    curGLContext=NULL;
#elif defined(STB_IS_LINUX)
    //drawable=NULL;
    dsp=NULL;
    ovGLContext=NULL;
#endif
    isVideoGLContext=false;
    isGLContextShared=false;
    showTriangleCount=false;
    showFrameRate=false;
    showCursor=true;
}


//----------------------------------------------------------------------------
// Destructor, does some clean up work.
SoStudierstubeViewer::~SoStudierstubeViewer()
{
    if(isGLContextShared)
    {
        videoComponent->deleteGLContext();
    }
}

int 
SoStudierstubeViewer::countTriangles()
{
    SoGetPrimitiveCountAction countAction;
    countAction.apply(this->getSceneGraph());
    return countAction.getTriangleCount();
}

//----------------------------------------------------------------------------
// Overides the inherited setHeadlight method
void
SoStudierstubeViewer::setHeadlight(SbBool /*on*/)
{
 /* SoStereoCameraKit *camKit = getStereoCameraKit();
    assert(camKit);

    SoGuiExaminerViewer::setHeadlight(on);
    // If the SoStudierstubeViewer uses the examiner-camera to render the
    // scene, don't set the custom headlight
    if ((camKit->getExaminerCamera()->getName() != "defaultExaminerCamera") &&
        getCameraControlMode()->isViewing())  
        return;

    //  Disable inherited headlight
    if (SoGuiExaminerViewer::getHeadlight())
        SoGuiExaminerViewer::getHeadlight()->on = FALSE;
    // and set custom headlight
    if (on) {
        if (!headlight) {
            // create SoStudierstubeViewer's custom headlight
            SoTransform *camXf = new SoTransform;
            SoRotation  *lorient = new SoRotation;
            headlight = new SoDirectionalLight;
            headlight->direction.setValue(1, -1, -10);
 
            SoTransformSeparator *lightroot = 
                (SoTransformSeparator *) root->getChild(1);
            lightroot->addChild(camXf);
            lightroot->addChild(lorient);
            lightroot->addChild(headlight);
            camXf->translation.connectFrom(&camKit->getTransform()->translation);
            camXf->rotation.connectFrom(&camKit->getTransform()->rotation);
                        
                        SoCalculator *vDir = new SoCalculator;
                    vDir->A.connectFrom(&camKit->getCameraLeft()->eyepointPosition);
                        vDir->B.connectFrom(&camKit->getCameraLeft()->position);
                    vDir->expression.setValue("oA = vec3f(0,0,-1); oB = B-A");
                        SoRotateFromTo *rot = new SoRotateFromTo;
                        rot->vectorFrom.connectFrom(&vDir->oA);
                        rot->vectorTo.connectFrom(&vDir->oB);
            lorient->rotation.connectFrom(&rot->rotateFromTo);
        }
        headlight->on = TRUE;
    }
    else
        if (headlight)
            headlight->on = FALSE;      
	*/
}


//----------------------------------------------------------------------------
// Returns whether the headlight is turned on or off.
SbBool 
SoStudierstubeViewer::isHeadlight() const
{
    if (headlight)
        return headlight->on.getValue();
    else
        return FALSE;
}

//----------------------------------------------------------------------------
// Returns the SoDirectionalLight-instance used as headlight.
SoDirectionalLight *
SoStudierstubeViewer::getHeadlight() const
{
    return headlight;
}


//----------------------------------------------------------------------------
// This method sets a window on top of all other windows and will keep it there
void
SoStudierstubeViewer::setWindowOnTop(SbBool on)
{
    if (on) {
#ifdef USE_SOQT
        this->getParentWidget()->reparent(this->getParentWidget()->parentWidget(), 
                                          Qt::WStyle_Customize| Qt::WStyle_StaysOnTop,
                                          this->getParentWidget()->pos(), TRUE);
#endif
    }
}


//----------------------------------------------------------------------------
// This method turns the decorations of a window on/off.
void
SoStudierstubeViewer::setWindowDecoration(SbBool on)
{
    // _attention_: extremely toolkit dependent stuff following...

    // checks if the decorations aren't already switched on or off
    if (this->isWindowDecoration() != on) {
        // switch window decorations on
        if (on) {
            
#ifdef USE_SOQT
        this->getParentWidget()->reparent(this->getParentWidget()->parentWidget(), 
                                Qt::WStyle_Customize|Qt::WStyle_NormalBorder,
                                this->getParentWidget()->pos(), TRUE);
#elif USE_SOWIN
            SetActiveWindow(this->getParentWidget());
            SetWindowLong(this->getParentWidget(), GWL_STYLE,
                          WS_POPUP|WS_MAXIMIZE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VISIBLE);
#endif
            
            this->isWindowDecorationActive = TRUE;
        }
        else {

#ifdef USE_SOQT
            this->getParentWidget()->reparent(this->getParentWidget()->parentWidget(), 
                                              Qt::WStyle_Customize|Qt::WStyle_NoBorder,
                                              this->getParentWidget()->pos());
#elif USE_SOWIN
            SetActiveWindow(this->getParentWidget());
            SetWindowLong(this->getParentWidget(), GWL_STYLE,
                          WS_POPUP|WS_MAXIMIZE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VISIBLE);
#endif
            
            this->isWindowDecorationActive = FALSE;
        }
    }
}

//----------------------------------------------------------------------------
// This method sets the position and the size of a window on the desktop.
void
SoStudierstubeViewer::setWindowPosSize(int x, int y, int width, int height)
{
    char *ep = '\0';
    errno = 0;

    // string to int conversion the very safe OpenBSD way. --tamer
    char *envX = getenv("SOGUI_DEFAULT_X_SIZE");
    long sizeX = 0;

    if (envX) {
        sizeX = strtol(envX, &ep, 10);
        if (envX[0] == '\0' || *ep != '\0')
			std::cerr << "** SOGUI_DEFAULT_X_SIZE=" << envX << " is not a number, therefore ignored!" << std::endl;
        else if (errno == ERANGE && (sizeX == LONG_MAX || sizeX == LONG_MIN) || (sizeX > INT_MAX || sizeX < INT_MIN))
            std::cerr << "** SOGUI_DEFAULT_X_SIZE=" << envX << " is out of range, therefore ignored!" << std::endl;
        else {
            std::cout << "INFO: viewer widget environmental width override, width=(" << sizeX << ")" << std::endl;
            width = sizeX;
        }
    }

    char *envY = getenv("SOGUI_DEFAULT_Y_SIZE");
    long sizeY = 0;

    if (envY) {
        sizeY = strtol(envY, &ep, 10);
        if (envY[0] == '\0' || *ep != '\0')
            std::cerr << "** SOGUI_DEFAULT_Y_SIZE=" << envY << " is not a number, therefore ignored!" << std::endl;
        else if (errno == ERANGE && (sizeY == LONG_MAX || sizeY == LONG_MIN) || (sizeY > INT_MAX || sizeY < INT_MIN))
            std::cerr << "** SOGUI_DEFAULT_Y_SIZE=" << envY << " is out of range, therefore ignored!" << std::endl;
        else {
            std::cout << "INFO: viewer widget environmental height override, height=(" << sizeY << ")" << std::endl;
            height = sizeY;
        }
    }

#ifdef USE_SOQT
    (this->getParentWidget())->move(x, y);

#elif USE_SOWIN
    SetWindowPos(this->getParentWidget(), 
                 HWND_TOP,
                 x, y, width, height,
                 SWP_SHOWWINDOW);
#endif
    
    // the viewer instantiated with SoGui::init needs to be treated differently.
    // getShellWidget returns NULL in this case.
    if (!this->getShellWidget())
        SoGui::setWidgetSize(SoGui::getShellWidget(this->getWidget()), SbVec2s(width, height));
    else
        this->setSize(SbVec2s(width, height));
}


void 
SoStudierstubeViewer::setOVGLContext(Video* video)
{
    videoComponent=video;
    shareGLContextWithVideo=true;
}

void 
SoStudierstubeViewer::redraw ()
{
    if(!isVideoGLContext)
    {
        this->glLockNormal(); // this makes the GL context "current"
#ifdef STB_IS_WINDOWS
        curDC=wglGetCurrentDC();
        if(!curDC){
            printf("StbError: failed to get current dc \n ");
        }

        HGLRC curStbContext=wglGetCurrentContext();
        if(!curStbContext){
            printf("StbError: failed to get current context \n");
        }


        if(curDC && curStbContext)
        {
            PIXELFORMATDESCRIPTOR pfd;

            DescribePixelFormat(curDC, 1, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
            //ChoosePixelFormat(curDC, &pfd );

            curGLContext=wglCreateContext(curDC);	
            if(!curGLContext)
            {
                printf("failed to create new opengl context \n");
                printf("%s\n",GetLastError());
            }
            else{
                if(!wglShareLists(curGLContext, curStbContext))
                {
                    printf("OV: failed to share opengl context \n");
                    printf("%i\n",::GetLastError());
                }
            }
        }   
#elif defined(STB_IS_LINUX)
        GLXContext curContext= glXGetCurrentContext();	
        dsp=glXGetCurrentDisplay();
        drawable=glXGetCurrentDrawable();

        int major;
        int minor;
        glXQueryVersion(dsp,
            &major,
            &minor);
        if(major<1 || minor <3 )
        {
            printf("StbError: need at least glx 1.3 to run openvideo");
            printf(" - you're running glx version %i.%i\n",major,minor);
        }
        ovGLContext=NULL;
        XVisualInfo* vis=NULL;

        int configID;
        glXQueryContext(dsp,
            curContext,
            GLX_FBCONFIG_ID,
            &configID);

        /////query screen /////////////////
        int screen;
        glXQueryContext(dsp,
            curContext,
            GLX_SCREEN,
            &screen);

        /////get FBConfig
        int nelements;
        GLXFBConfig  *fbConfigs=glXGetFBConfigs(dsp,
            screen,
            &nelements);
        int value;
        GLXFBConfig fbConfig;
        for(int i=0;i<nelements;i++)
        {
            glXGetFBConfigAttrib(dsp,
                fbConfigs[i],
                GLX_FBCONFIG_ID,
                &value);
            if(value==configID){
                fbConfig=fbConfigs[i];
                break;
            }
        }
        vis=glXGetVisualFromFBConfig(dsp,
            fbConfig);
        if(curContext
            && dsp
            && vis)
        {
            ovGLContext= glXCreateContext(dsp, 
                vis,
                curContext,
                true);
            if(!ovGLContext)
                printf("failed to create openvideo's context\n");
        }
#endif
	    this->glUnlockNormal();// this releases the GL contex	
        isVideoGLContext=true;
    }
    if(!isGLContextShared && shareGLContextWithVideo)
    {
        isGLContextShared=true;
        this->glLockNormal(); // this makes the GL context "current"
#ifdef STB_IS_WINDOWS
        videoComponent->setGLContext(curGLContext,curDC);
#elif defined(STB_IS_LINUX)
        videoComponent->setGLContext(drawable, ovGLContext, dsp);
#endif
        this->glUnlockNormal();// this releases the GL contex	
        
    }


    if(showFrameRate){
#if defined(STB_IS_WINDOWS)
        DWORD thisTime = GetTickCount();
        diffTime = (float)(thisTime-lastTime);
        if (diffTime>0) framerate=1000*(1/diffTime);
        lastTime=thisTime;
        printf("[Render = %3.1f] \n",framerate);
       
#endif
     }
    if(showTriangleCount)
        printf("[triangleCount = %i] \n",this->countTriangles());
    if(showCursor==FALSE){
        setComponentCursor(SoGuiCursor::getBlankCursor());
    }
    SoGuiExaminerViewer::redraw();
}

void 
SoStudierstubeViewer::showMouse(bool mouseOn)
{
    showCursor=mouseOn;
}

void 
SoStudierstubeViewer::printTriangles(bool onOff)
{
    this->showTriangleCount=onOff;
}

void 
SoStudierstubeViewer::printFrameRate(bool onOff)
{
    this->showFrameRate=onOff;
}   

END_NAMESPACE_STB
