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
/** The header file for the SoMagicScene.
*
* @author Erick Mendez
* @ingroup vidente
*
* $Id: SoMagicScene.h 2006-03-10 mendez $
* @file                                                                   */
/* ======================================================================= */

#ifndef _SoMagicScene_H_
#define _SoMagicScene_H_

/*
 * --------------------------------------------------------------------------------
 * Includes
 * --------------------------------------------------------------------------------
 */
#include <Inventor/nodes/SoSeparator.h>

#ifdef WIN32
#include <SoWinEnterScope.h>
#include <windows.h>
#endif

#include <Inventor/actions/SoGLRenderAction.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

#include "CgFboManager.h"
#include "starlight.h"

#ifndef SCENE_RENDER_PASSES
#define SCENE_RENDER_PASSES 3
#define BEHIND_LENS 0
#define INSIDE_LENS 1
#define INFRONT_LENS 2
#endif

/**
 * This class implements the Scene of a Magic Lens as defined by Ropinski's algorithm
 * user should avoid using it directly and instead use SoMagicSceneKit
 *
 * @author Erick Méndez
 * @ingroup vidente
 */
class STARLIGHT_API SoMagicScene: public SoSeparator
{
	SO_NODE_HEADER(SoMagicScene);

public:

	/// Flag for verification of Cg Programs Loading
	bool areCgProgramsLoaded;

	/// The constructor of the class, initializes whats necessary
	SoMagicScene();

	/// Destructor, deletes whatever is left
	~SoMagicScene();
	
	/// Initializes the node
    static void initClass();

	/// Sets the Fbo handle
	void setFboHandle(CgFboManager *newHandle);

	/// Disables or Enables Magic rendering
	void goMagic(bool flag);

protected:
	/// Controls the render passes
	int renderPass;

	/// Flag for turning on/off magic
	bool isMagic;

	/// We need 3 fragment programs, one for each render pass
	CGprogram fragmentPrograms[SCENE_RENDER_PASSES];

	/// Pointer to the Handle of the FBO
	CgFboManager *handleCgFbo;

	/// Sets the Cg parameters and renders down its tree
	virtual void GLRenderBelowPath(SoGLRenderAction * action);

	/// Initializes the Cg Programs
	void initCgPrograms();

	/// Enables the Specified Fragment Program
	void enableFragmentProgram(int which);

	/// Disables the Cg program
	void disableCgProgram();

	/// Sets whatever GL commands we need
	void prepareGL();
};

#endif
