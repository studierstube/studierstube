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
/** The Source file for the CgFboManager
*
* @author Erick Mendez
* @author Markus Grabner
*
* $Id: CgFboManager.h 2006-03-10 mendez $
* @file                                                                   */
/* ======================================================================= */
#include <stb/components/magicLens/CgFboManager.h>
#include <iostream>
#include <string.h>
#include <assert.h>

/*
 * --------------------------------------------------------------------------------
 * Cg Profiles for Vertex and Fragment Programs
 * --------------------------------------------------------------------------------
 */
CGprofile CgFboManager::fragmentProfile;
CGcontext CgFboManager::context;

bool CgFboManager::isExtensionSupported(const char *extension)
{
	const GLubyte *extensions = NULL;
	const GLubyte *start;
	GLubyte *where, *terminator;

	where=(GLubyte *) strchr(extension, ' ');
	if (where || *extension == '\0') return false;
	extensions=glGetString(GL_EXTENSIONS);
	start=extensions;
	for (;;) 
	{
		where=(GLubyte *) strstr((const char *) start, extension);
		if (!where) break;
		terminator=where+strlen(extension);
		if (where==start || *(where - 1)==' ')
			if (*terminator==' ' || *terminator=='\0')
				return true;
		start=terminator;
	}
	return false;
}

CgFboManager::CgFboManager()
{
	isLoaded=false;
}

CgFboManager::~CgFboManager()
{
	// Clean up
	if (glIsTexture(handleDBTexture))
		glDeleteTextures(1, &handleDBTexture);

	cgDestroyContext(context);
}

void CgFboManager::init()
{
	// Do this only once
	if (isLoaded) return;
	isLoaded=true;

	// Calls the initProfiles function
	initProfiles();

	// Create Context
	context=cgCreateContext();

	if (context==NULL)
	{
		printf("CgFboManager: Context Could not be created.\nAborting.\n\n");
		assert(0);
	}

	// Register Error Callback
	cgSetErrorCallback(cgErrorCallback);

	// Check first for the existence of the extensions
	if (!isExtensionSupported("GL_EXT_framebuffer_object"))
	{
		printf("CgFboManager: Framebuffer Objects not available. \nAborting.\n\n");
		assert(0);
	}
	if (!isExtensionSupported("GL_NV_texture_rectangle"))
	{
		printf("CgFboManager: NV textures not available. \nAborting.\n\n");
		assert(0);
	}
	if (!(isFloatTextureSupported=isExtensionSupported("GL_NV_float_buffer")))
		printf("CgFboManager: Floating point textures not available, falling back to byte.\n");

	//isFloatTextureSupported=false;

	// Create the FBO handle
	glGenFramebuffersEXT(1, &handleFrameBufferObject);

	// Create the texture we will be using for exchange
	glGenTextures(1, &handleDBTexture);

	// Set its parameters
	glBindTexture(GL_TEXTURE_RECTANGLE_NV, handleDBTexture);
	if (isFloatTextureSupported) 
		glTexImage2D(GL_TEXTURE_RECTANGLE_NV, 0, GL_FLOAT_RGB16_NV, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	else 
		glTexImage2D(GL_TEXTURE_RECTANGLE_NV, 0, GL_RGB, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_RECTANGLE_NV, 0);
}

void CgFboManager::targetRenderToFBO()
{
	// Bind the FBO and attach the texture
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, handleFrameBufferObject);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_RECTANGLE_NV, handleDBTexture, 0);
}

void CgFboManager::releaseRenderToFBO()
{
	// Release the FBO and release the texture
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_RECTANGLE_NV, 0, 0);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void CgFboManager::prepareReadFromFBO()
{
	// Bind the texture so that next Cg programs can use it
	glBindTexture(GL_TEXTURE_RECTANGLE_NV, handleDBTexture);
}

void CgFboManager::releaseReadFromFBO()
{
	// Unbind the texture
	glBindTexture(GL_TEXTURE_RECTANGLE_NV, 0);
}

void CgFboManager::cgErrorCallback()
{
	CGerror err=cgGetError();

	if(!err) return;

	// If error found, print it out and exit
	printf("CgFboManager: %s \n %s. \nAborting.\n\n", cgGetErrorString(err), cgGetLastListing(context));
	assert(0);
}

void CgFboManager::initProfiles()
{
	static bool isLoaded=false;
	if (isLoaded) return;

	GLenum error=glewInit();
	if (error!=GLEW_OK)
	{
		printf("CgFboManager: GLEW could not be initialized [ %s ]. \nAborting.\n\n", glewGetErrorString(error));
		assert(0);
	}

	// Find a profile for the Fragment Program
	if (glewIsSupported("GL_NV_fragment_program2"))
		fragmentProfile=CG_PROFILE_FP40;
	else if (glewIsSupported("GL_NV_fragment_program"))
		fragmentProfile=CG_PROFILE_FP30;
	else 
	{
		printf("CgFboManager: No suitable fragment profile found. \nAborting.\n\n");
		assert(0);
	}

	isLoaded=true;
}


