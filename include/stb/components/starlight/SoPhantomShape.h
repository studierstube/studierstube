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
/** The header file for the SoPhantomShape.
*
* @author Erick Mendez
* @ingroup vidente
*
* $Id: SoPhantomShape.h 2006-03-10 mendez $
* @file                                                                   */
/* ======================================================================= */

#ifndef _SOPHANTOMSHAPE_H_
#define _SOPHANTOMSHAPE_H_

/*
 * --------------------------------------------------------------------------------
 * Includes
 * --------------------------------------------------------------------------------
 */
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/nodes/SoSeparator.h>
#include "starlight.h"

/** 
* Header for an Open Inventor Class that implements a Phantom Shape 
*
* @author Erick Mendez
* @ingroup vidente
*/

class STARLIGHT_API SoPhantomShape: public SoSeparator
{
    SO_NODE_HEADER(SoPhantomShape);
public:
	/// Initializes the node kit
    static void initClass();

	/// The constructor of the class, initializes the catalog
    SoPhantomShape();

	/// Destructor
    ~SoPhantomShape();
	
	/// Enabling flag
	SoSFBool enable;

protected:
	virtual void GLRenderBelowPath(SoGLRenderAction * action);

};

#endif
