/*
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
/** The header file for the SoMagicLensKit
*
* @author Erick Mendez
* @ingroup vidente
*
* $Id: SoMagicLensKit.h 2006-03-10 mendez $
* @file                                                                   */
/* ======================================================================= */

#ifndef _SOMAGICLENSKIT_H_
#define _SOMAGICLENSKIT_H_



/*
 * --------------------------------------------------------------------------------
 * Includes
 * --------------------------------------------------------------------------------
 */
#ifdef WIN32
#include <SoWinEnterScope.h>
#include <windows.h>
#endif

#include <Inventor/actions/SoGLRenderAction.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include "SoMagicLens.h"
#include "SoMagicStylesKit.h"
#include <stb/components/magicLens/SoMagicLens.h>
#include <stb/components/magicLens/SoMagicStylesKit.h>

#include <stb/components/starlight/starlight.h>


/**
 * This class is the kit the user will be using for defining a lens.
 *
 * @author Erick Méndez
 * @ingroup vidente
 */
class SoMagicLensKit: public SoBaseKit
{
protected:
	/// Parts of the catalog
	SO_KIT_HEADER(SoMagicLensKit);
	SO_KIT_CATALOG_ENTRY_HEADER(sepTop);
	SO_KIT_CATALOG_ENTRY_HEADER(sepLeft);
	SO_KIT_CATALOG_ENTRY_HEADER(content); //Assumed to be left
	SO_KIT_CATALOG_ENTRY_HEADER(sepRight);
	SO_KIT_CATALOG_ENTRY_HEADER(contentright);

public:

	/// The constructor of the class, initializes the catalog
    SoMagicLensKit();

	/// Destructor, deletes the sensors
    ~SoMagicLensKit();

	/// Initializes the node kit
    static void initClass();

	/// Sets the content 
	void setContent(SoNode *newContent);

	/// Propagates the content 
	void propagateContent();

	/// Disables or Enables Magic rendering
	void goMagic(bool flag);

protected:

	/// Sets the Cg parameters and renders down its tree
	virtual void GLRender(SoGLRenderAction * action);

	/// Attaches and detaches the sensors and does a couple of one time operations
    virtual SbBool setUpConnections(SbBool onoff, SbBool doitalways);

};


#endif