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
/** The header file for the SoMagicLens
*
* @author Erick Mendez
* @ingroup vidente
*
* $Id: SoMagicLens.h 2006-03-10 mendez $
* @file                                                                   */
/* ======================================================================= */
#ifndef _SOMAGICLENS_H_
#define _SOMAGICLENS_H_

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

#ifndef LENS_RENDER_PASSES
#define LENS_RENDER_PASSES 2
#define BACK_FACE 0
#define FRONT_FACE 1
#endif

/**
 * This class implements a Magic Lens as defined by Ropinski's algorithm
 * user should avoid using it directly and instead use SoMagicLensKit
 *
 * @author Erick Méndez
 * @ingroup vidente
 */
class STARLIGHT_API SoMagicLens: public SoSeparator
{
	SO_NODE_HEADER(SoMagicLens);

public:

	/// Flag for verification of Cg Programs Loading
	bool areCgProgramsLoaded;

	/// The constructor of the class, initializes whats necessary
	SoMagicLens();

	/// Destructor, deletes whatever is left
	~SoMagicLens();
	
	/// Initializes the node
    static void initClass();

	/// Sets the Fbo handle and passes the same to its magic child
	void setFboHandle(CgFboManager *newHandle);

	/// Disables or Enables Magic rendering
	void goMagic(bool flag);

protected:
	/// Controls the render passes
	int renderPass;

	/// Flag for turning on/off magic
	bool isMagic;

	/// We need 2 fragment programs, one for each render pass
	CGprogram fragmentPrograms[LENS_RENDER_PASSES];

	/// Pointer to the Handle of the FBO
	CgFboManager *handleCgFbo;

	/// Sets the Cg parameters and renders down its tree
	virtual void GLRenderBelowPath(SoGLRenderAction * action);

	/// Loads the Cg Programs
	void loadCgPrograms();

	/// Enables the Specified Fragment Program
	void enableFragmentProgram(int which);

	/// Disables the current Cg Program
	void disableProgram();

	/// Sets whatever GL commands we need
	void prepareGL(int which);
};

#endif
