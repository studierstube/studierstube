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
/** The Source file for the SoMagicLens.
*
* @author Erick Mendez
*
* $Id: SoMagicLens.cpp 2006-03-10 mendez $
* @file                                                                   */
/* ======================================================================= */

#ifdef USE_VIDENTE

#include <Inventor/elements/SoGLLazyElement.h>
#include <Inventor/elements/SoCacheElement.h>
#include <stb/components/starlight/SoMagicLens.h>

// We load the Cg programs into strings
const char *strCgLensBackFace=
#include "cg/LensBackFace.cg"
;
const char *strCgLensFrontFace=
#include "cg/LensFrontFace.cg"
;

using namespace std;

SO_NODE_SOURCE(SoMagicLens);

void SoMagicLens::initClass(void)
{
	SO_NODE_INIT_CLASS(SoMagicLens, SoSeparator, "Separator");
}

SoMagicLens::SoMagicLens(void)
{
    SO_NODE_CONSTRUCTOR(SoMagicLens);
	isBuiltIn = TRUE;

	renderPass=0;
	areCgProgramsLoaded=false;
	isMagic=true;
}

SoMagicLens::~SoMagicLens()
{
	// Clean up
	if (areCgProgramsLoaded)
	{
		cgDestroyProgram(fragmentPrograms[BACK_FACE]);
		cgDestroyProgram(fragmentPrograms[FRONT_FACE]);
	}
}

void SoMagicLens::loadCgPrograms()
{
	if (areCgProgramsLoaded) return;

	fragmentPrograms[BACK_FACE]=cgCreateProgram(handleCgFbo->context, CG_SOURCE, strCgLensBackFace, handleCgFbo->fragmentProfile, 0, 0);
	fragmentPrograms[FRONT_FACE]=cgCreateProgram(handleCgFbo->context, CG_SOURCE, strCgLensFrontFace, handleCgFbo->fragmentProfile, 0, 0);
	cgGLLoadProgram(fragmentPrograms[BACK_FACE]);
	cgGLLoadProgram(fragmentPrograms[FRONT_FACE]);

	areCgProgramsLoaded=true;
}

void SoMagicLens::enableFragmentProgram(int which)
{
	// Bind Back Face Program and Enable Profile
	cgGLEnableProfile(handleCgFbo->fragmentProfile);
	cgGLBindProgram(fragmentPrograms[which]);
}

void SoMagicLens::disableProgram()
{
	// Disable Profile
	cgGLDisableProfile(handleCgFbo->fragmentProfile);
}

void SoMagicLens::prepareGL(int which)
{
	switch(which) {
	case BACK_FACE:
		glClear(GL_COLOR_BUFFER_BIT);
		glCullFace(GL_FRONT);
		glBlendFunc(GL_ONE,GL_ZERO);
		break;
	case FRONT_FACE:
		glCullFace(GL_BACK);
		glBlendFunc(GL_ONE,GL_ONE);
		break;
	}

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void SoMagicLens::GLRenderBelowPath(SoGLRenderAction * action)
{
	// Necessary
	SoGLLazyElement::sendAllMaterial(action->getState());
	SoCacheElement::invalidate(action->getState());

	//SbVec2s size=action->getViewportRegion().getWindowSize();

	glPushAttrib(GL_ENABLE_BIT | GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	loadCgPrograms();

	if (areCgProgramsLoaded&&isMagic)
	{
		// Enable the appropriate Fragment Program (two passes)
		enableFragmentProgram(renderPass);
		// Prepare to output to the FBO
		handleCgFbo->targetRenderToFBO();
		// Set everything in OpenGL for the current pass
		prepareGL(renderPass);

		// Render
		SoSeparator::GLRenderBelowPath(action);

		// Let go
		disableProgram();
		handleCgFbo->releaseRenderToFBO();
	}
	else
	{
		// Render
		SoSeparator::GLRenderBelowPath(action);
	}

	// We allow two render passes
	renderPass=(renderPass+1)%LENS_RENDER_PASSES;
	glPopAttrib();
}

void SoMagicLens::setFboHandle(CgFboManager *newHandle)
{
	handleCgFbo=newHandle;
}

void SoMagicLens::goMagic(bool flag)
{
	isMagic=flag;
}

#endif //USE_VIDENTE
