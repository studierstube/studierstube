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
/** The Source file for the SoMagicScene.
*
* @author Erick Mendez
*
* $Id: SoMagicScene.cpp 2006-03-10 mendez $
* @file                                                                   */
/* ======================================================================= */


/*
 * --------------------------------------------------------------------------------
 * Includes
 * --------------------------------------------------------------------------------
 */
#include <Inventor/elements/SoGLLazyElement.h>
#include <Inventor/elements/SoCacheElement.h>
#include <stb/components/magicLens/SoMagicScene.h>

// We load the Cg programs into strings
const char *strCgBehindLens=
#include "cg/BehindLens.cg"
;
const char *strCgInsideLens=
#include "cg/InsideLens.cg"
;
const char *strCgInfrontLens=
#include "cg/InfrontLens.cg"
;

using namespace std;

SO_NODE_SOURCE(SoMagicScene);

void SoMagicScene::initClass(void)
{
	SO_NODE_INIT_CLASS(SoMagicScene, SoSeparator, "Separator");
}

SoMagicScene::SoMagicScene(void)
{
    SO_NODE_CONSTRUCTOR(SoMagicScene);
	isBuiltIn = TRUE;    

	renderPass=0;
	areCgProgramsLoaded=false;
	isMagic=true;
}

SoMagicScene::~SoMagicScene()
{
	// Clean up
	if (areCgProgramsLoaded)
	{
		cgDestroyProgram(fragmentPrograms[BEHIND_LENS]);
		cgDestroyProgram(fragmentPrograms[INSIDE_LENS]);
		cgDestroyProgram(fragmentPrograms[INFRONT_LENS]);
	}

}

void SoMagicScene::initCgPrograms()
{
	if (areCgProgramsLoaded) return;

	fragmentPrograms[BEHIND_LENS]=cgCreateProgram(CgFboManager::getInstance()->context, CG_SOURCE, strCgBehindLens, CgFboManager::getInstance()->fragmentProfile, 0, 0);
	fragmentPrograms[INSIDE_LENS]=cgCreateProgram(CgFboManager::getInstance()->context, CG_SOURCE, strCgInsideLens, CgFboManager::getInstance()->fragmentProfile, 0, 0);
	fragmentPrograms[INFRONT_LENS]=cgCreateProgram(CgFboManager::getInstance()->context, CG_SOURCE, strCgInfrontLens, CgFboManager::getInstance()->fragmentProfile, 0, 0);

	cgGLLoadProgram(fragmentPrograms[BEHIND_LENS]);
	cgGLLoadProgram(fragmentPrograms[INSIDE_LENS]);
	cgGLLoadProgram(fragmentPrograms[INFRONT_LENS]);

	areCgProgramsLoaded=true;
}

void SoMagicScene::enableFragmentProgram(int which)
{
	cgGLEnableProfile(CgFboManager::getInstance()->fragmentProfile);
	cgGLBindProgram(fragmentPrograms[which]);
}

void SoMagicScene::disableCgProgram()
{
	cgGLDisableProfile(CgFboManager::getInstance()->fragmentProfile);
}

void SoMagicScene::prepareGL()
{
	glDisable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
}

void SoMagicScene::GLRenderBelowPath(SoGLRenderAction * action)
{
	// Necessary
	SoGLLazyElement::sendAllMaterial(action->getState());
	SoCacheElement::invalidate(action->getState());
	
	glPushAttrib(GL_ENABLE_BIT | GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	initCgPrograms();

	// If we have a program loaded
	if (areCgProgramsLoaded&&isMagic)
	{
		// Enable the appropriate Fragment Program (three passes)
		enableFragmentProgram(renderPass);
		// Prepare input from the FBO texture
		CgFboManager::getInstance()->prepareReadFromFBO();
		// Set everything in OpenGL
		prepareGL();

		// Render
		SoSeparator::GLRenderBelowPath(action);

		// Let go
		CgFboManager::getInstance()->releaseReadFromFBO();
		disableCgProgram();
	}
	else
	{
		// Render
		SoSeparator::GLRenderBelowPath(action);
	}

	renderPass=(renderPass+1)%SCENE_RENDER_PASSES;
	glPopAttrib();
}

void SoMagicScene::goMagic(bool flag)
{
	isMagic=flag;
}

