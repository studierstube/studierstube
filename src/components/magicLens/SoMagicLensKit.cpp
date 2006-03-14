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
/** The Source file for the SoMagicLensKit.
*
* @author Erick Mendez
*
* $Id: SoMagicLensKit.cpp 2006-03-10 mendez $
* @file                                                                   */
/* ======================================================================= */



/*
 * --------------------------------------------------------------------------------
 * Includes
 * --------------------------------------------------------------------------------
 */
#include <stb/components/starlight/SoContextSeparator.h>
#include <stb/components/magicLens/SoMagicLensKit.h>
#include <stb/components/starlight/starlight.h>
#include <stb/components/magicLens/MagicLens.h>

using namespace std;

SO_KIT_SOURCE(SoMagicLensKit);

/*
 * --------------------------------------------------------------------------------
 * Required by the node kit
 * --------------------------------------------------------------------------------
 */
void SoMagicLensKit::initClass(void)
{
    SO_KIT_INIT_CLASS(SoMagicLensKit, SoBaseKit, "BaseKit");
}

/*
 * --------------------------------------------------------------------------------
 * Constructor and Destructor
 * --------------------------------------------------------------------------------
 */
SoMagicLensKit::SoMagicLensKit()
{

	SO_KIT_CONSTRUCTOR(SoMagicLensKit);

	// This is for the parts of the catalog
    //////// denis 
	//SO_KIT_ADD_CATALOG_ENTRY(sepTop,				SoContextSeparator,	FALSE,	this,	\x0, TRUE);
    //#define SO_KIT_ADD_CATALOG_ENTRY(_part_, _partclass_, _isdefnull_ , _parent_, _sibling_, _ispublic_) 
    SO_KIT_ADD_CATALOG_ENTRY(sepTop,SoSeparator,TRUE,	this,	\x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(sepLeft,				SoSeparator, TRUE, sepTop,	\x0, TRUE);
	SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(content,		SoNode, SoMagicLens, TRUE, sepLeft, "", TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(sepRight,				SoSeparator, TRUE, sepTop,	\x0, TRUE);
	SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(contentright,	SoNode, SoMagicLens, TRUE, sepRight, "", TRUE);

	SO_KIT_INIT_INSTANCE();

	SO_GET_ANY_PART(this, "content", SoMagicLens)->setFboHandle(SoMagicStylesKit::handleCgFbo);

    stb::Starlight* starlight=(stb::Starlight*)(stb::MagicLens::getStarlight());
    sepTop.setValue((SoContextSeparator*)(starlight->createSoContextSeparator()));

	this->setUpConnections(TRUE, TRUE);
}

SoMagicLensKit::~SoMagicLensKit()
{
}

SbBool SoMagicLensKit::setUpConnections(SbBool onoff, SbBool doitalways)
{
	if ((doitalways == FALSE) && (connectionsSetUp == onoff)) return onoff;
	if (onoff) 
	{
		// We connect AFTER base class.
		SoBaseKit::setUpConnections(onoff, doitalways);

		propagateContent();
	}
	else 
	{
		// We disconnect BEFORE base class.
		SoBaseKit::setUpConnections(onoff, doitalways);
	}
	return !(connectionsSetUp = onoff);
}

void SoMagicLensKit::setContent(SoNode *newContent)
{
	if(newContent)
	{
		setAnyPart("content",newContent);
	}
}

void SoMagicLensKit::propagateContent()
{
	setAnyPart("contentright",getAnyPart("content",FALSE));
}

void SoMagicLensKit::goMagic(bool flag)
{
	SO_GET_ANY_PART(this, "content", SoMagicLens)->goMagic(flag);
	SO_GET_ANY_PART(this, "contentright", SoMagicLens)->goMagic(flag);
}

void SoMagicLensKit::GLRender(SoGLRenderAction * action)
{
	// Use instead a Field sensor!!!!
	propagateContent();
	SO_GET_ANY_PART(this, "content", SoMagicLens)->setFboHandle(SoMagicStylesKit::handleCgFbo);
	SO_GET_ANY_PART(this, "contentright", SoMagicLens)->setFboHandle(SoMagicStylesKit::handleCgFbo);
	SoBaseKit::GLRender(action);
}


