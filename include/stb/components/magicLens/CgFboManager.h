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
/** The header file for the CgFboManager
*
* @author Erick Mendez
* @author Markus Grabner
* @ingroup vidente
*
* $Id: CgFboManager.cpp 2006-03-10 mendez $
* @file                                                                   */
/* ======================================================================= */


#ifndef _CGFBOMANAGER_H_
#define _CGFBOMANAGER_H_


/*
 * --------------------------------------------------------------------------------
 * Includes
 * --------------------------------------------------------------------------------
 */
#include <iostream>

#include <GL/glew.h>
#include <Cg/cg.h>
#include <Cg/cgGL.h>
#include <stb/components/starlight/starlight.h>

#pragma comment(lib, "cg")
#pragma comment(lib, "cgGL")
#pragma comment(lib, "glew32")

/**
 * This class is responsible of initializing and managing whatever is necessary for 
 * FBO usage. It provides a handler for one Frame Buffer Object and one Texture.
 * It also provides functions for targeting and releasing rendering to a FBO.
 * This class is intended for hiding the FBO settings.
 *
 * @author Erick Méndez
 * @author Markus Grabner
 * @ingroup vidente
 */


class CgFboManager
{
public:

	/// The Cg Context
	static CGcontext context;

	/// The fragment profile
	static CGprofile fragmentProfile;

	/// The constructor of the class, initializes whats necessary
	CgFboManager();

	/// Destructor, deletes whatever is left
	~CgFboManager();

	/// Initializes the Texture and the FBO
	void init();

	/// Prints out the received error
	static void cgErrorCallback();

	/// Sets things so that the next render will go to the FBO
	void targetRenderToFBO();

	/// Releases the Rendering from the FBO
	void releaseRenderToFBO();

	/// Prepares to provide the FBO Texture to the next render pass
	void prepareReadFromFBO();

	/// Releases the texture, so that is not available in the next pass
	void releaseReadFromFBO();

	/// Finds a suitable profile and initializes it
	void initProfiles();

private:
	/// The FBO handle
	GLuint handleFrameBufferObject;

	/// The handle of the texture to which the FBO was targeted
	GLuint handleDBTexture;

	/// Flag for initialization
	bool isLoaded;

	/// Flag for verifying support of Floating point textures
	bool isFloatTextureSupported;

	/// Maximum texture dimensions
	enum {TEXTURE_WIDTH = 1280, TEXTURE_HEIGHT = 1024};

	/// Checks for extensions support, provided by openGL.org
	bool isExtensionSupported(const char *extension);
};

#endif