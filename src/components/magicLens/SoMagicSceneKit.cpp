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
/** The Source file for the SoMagicSceneKit.
*
* @author Erick Mendez
*
* $Id: SoMagicSceneKit.cpp 2006-03-10 mendez $
* @file                                                                   */
/* ======================================================================= */



/*
 * --------------------------------------------------------------------------------
 * Includes
 * --------------------------------------------------------------------------------
 */
#include <stb/components/starlight/SoContextSeparator.h>
#include <stb/components/starlight/SoNodeContextReport.h>
#include <Inventor/nodes/SoTransform.h>
#include <stb/components/magicLens/SoMagicSceneKit.h>
#include <stb/components/magicLens/MagicLens.h>
#include <stb/components/starlight/starlight.h>

using namespace std;

SO_KIT_SOURCE(SoMagicSceneKit);

/*
* --------------------------------------------------------------------------------
* Required by the node kit
* --------------------------------------------------------------------------------
*/
void SoMagicSceneKit::initClass(void)
{
	SO_KIT_INIT_CLASS(SoMagicSceneKit, SoBaseKit, "BaseKit");
}

/*
* --------------------------------------------------------------------------------
* Constructor and Destructor
* --------------------------------------------------------------------------------
*/
SoMagicSceneKit::SoMagicSceneKit()
{
    // SO_KIT_ADD_CATALOG_ENTRY(
    // name,className,nullByDefault,parentName,rightSiblingName,isPublic

	SO_KIT_CONSTRUCTOR(SoMagicSceneKit);
    //////////////////////// DENIS 
    //SO_KIT_ADD_CATALOG_ENTRY(sepTop,						SoContextSeparator, FALSE, this,	\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(sepTop,						SoSeparator, TRUE, this,	\x0, FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(sepBehind,						SoSeparator, TRUE, sepTop,	\x0, TRUE);
	//SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(behindRenderStyle,	SoNode, SoNodeContextReport, TRUE, sepBehind, "", TRUE);
    SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(behindRenderStyle,	SoNode, SoNode, TRUE, sepBehind, "", TRUE);
	SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(content,				SoNode, SoMagicScene, TRUE, sepBehind, "", TRUE);
	//SO_KIT_ADD_CATALOG_ENTRY(content,		SoSeparator, TRUE, sepBehind,	\x0, TRUE);

	SO_KIT_ADD_CATALOG_ENTRY(sepInside,						SoSeparator, TRUE, sepTop,	\x0, TRUE);
	//SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(insideRenderStyle,	SoNode, SoNodeContextReport, TRUE, sepInside, "", TRUE);
    SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(insideRenderStyle,	SoNode, SoNode, TRUE, sepInside, "", TRUE);
	SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(contentInside,		SoNode, SoMagicScene, TRUE, sepInside, "", TRUE);

	SO_KIT_ADD_CATALOG_ENTRY(sepInfront,					SoSeparator, TRUE, sepTop,	\x0, TRUE);
   // SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(infrontRenderStyle,	SoNode, SoNodeContextReport, TRUE, sepInfront, "", TRUE);
	SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(infrontRenderStyle,	SoNode, SoNode, TRUE, sepInfront, "", TRUE);
	SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(contentInfront,		SoNode, SoMagicScene, TRUE, sepInfront, "", TRUE);

	SO_KIT_ADD_FIELD(styleName,		(NULL));

	SO_KIT_INIT_INSTANCE();

	// Create the Sensors
	styleNameSensor=new SoFieldSensor(SoMagicSceneKit::styleNameCB, this);

    stb::Starlight* starlight=(stb::Starlight*)(stb::MagicLens::getStarlight());
    sepTop.setValue((SoContextSeparator*)(starlight->createSoContextSeparator()));
    behindRenderStyle.setValue(((SoNodeContextReport*)(starlight->createSoNodeContextReport())));
    insideRenderStyle.setValue(((SoNodeContextReport*)(starlight->createSoNodeContextReport())));
    infrontRenderStyle.setValue(((SoNodeContextReport*)(starlight->createSoNodeContextReport())));

	this->setUpConnections(TRUE, TRUE);
}

SoMagicSceneKit::~SoMagicSceneKit()
{
}

SbBool SoMagicSceneKit::setUpConnections(SbBool onoff, SbBool doitalways)
{
	if ((doitalways == FALSE) && (connectionsSetUp == onoff)) return onoff;
	if (onoff) 
	{
		// We connect AFTER base class.
		SoBaseKit::setUpConnections(onoff, doitalways);

		styleNameSensor->attach(&this->styleName);
		setStyle((char *)styleName.getValue().getString());
		propagateContent();
	}
	else 
	{
		// We disconnect BEFORE base class.
		styleNameSensor->detach();

		SoBaseKit::setUpConnections(onoff, doitalways);
	}
	return !(connectionsSetUp = onoff);
}

void SoMagicSceneKit::styleNameCB(void* data, SoSensor* sensor)
{
	SoMagicSceneKit *mskTmp= (SoMagicSceneKit *)data;
	mskTmp->setStyle((char *)mskTmp->styleName.getValue().getString());
}

void SoMagicSceneKit::goMagic(bool flag)
{
	SO_GET_ANY_PART(this, "content", SoMagicScene)->goMagic(flag);
}

void SoMagicSceneKit::setStyle(char *strStyleName)
{
	char *strOutsideStyleName=new char[255];
	char *strInsideStyleName=new char[255];
	strcpy(strOutsideStyleName,strStyleName);
	strcat(strOutsideStyleName,"OutsideLens");
	strcpy(strInsideStyleName,strStyleName);
	strcat(strInsideStyleName,"InsideLens");

	SO_GET_ANY_PART(this, "behindRenderStyle", SoNodeContextReport)->index.setValue(strOutsideStyleName);
	SO_GET_ANY_PART(this, "insideRenderStyle", SoNodeContextReport)->index.setValue(strInsideStyleName);
	SO_GET_ANY_PART(this, "infrontRenderStyle", SoNodeContextReport)->index.setValue(strOutsideStyleName);
}

void SoMagicSceneKit::setContent(SoNode *newContent)
{
	if(newContent)
	{
		setAnyPart("content",newContent);
	}
}

void SoMagicSceneKit::propagateContent()
{
	setAnyPart("contentInside",getAnyPart("content",FALSE));
	setAnyPart("contentInfront",getAnyPart("content",FALSE));
}

void SoMagicSceneKit::GLRender(SoGLRenderAction * action)
{
	// Use instead a Field sensor!!!!
	propagateContent();
	SoBaseKit::GLRender(action);
}

