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
/** The Source file for the SoMagicStylesKit.
*
* @author Erick Mendez
*
* $Id: SoMagicStylesKit.cpp 2006-03-10 mendez $
* @file                                                                   */
/* ======================================================================= */



/*
 * --------------------------------------------------------------------------------
 * Includes
 * --------------------------------------------------------------------------------
 */
#include <Inventor/nodes/SoGroup.h>
#include <stb/components/starlight/SoNodeContext.h>
#include <iostream>
#include <stb/components/magicLens/SoMagicStylesKit.h>
#include <stb/components/magicLens/MagicLens.h>
#include <stb/components/starlight/starlight.h>

using namespace std;

SO_KIT_SOURCE(SoMagicStylesKit);

SoMagicStylesKit *SoMagicStylesKit::pinstance=0;
CgFboManager *SoMagicStylesKit::handleCgFbo=NULL;

SoMagicStylesKit *SoMagicStylesKit::getInstance()
{
	if (pinstance==0)
	{
		pinstance=new SoMagicStylesKit;
	}
	return pinstance;
}

/*
 * --------------------------------------------------------------------------------
 * Required by the node kit
 * --------------------------------------------------------------------------------
 */
void SoMagicStylesKit::initClass(void)
{
    SO_KIT_INIT_CLASS(SoMagicStylesKit, SoBaseKit, "BaseKit");
}

/*
 * --------------------------------------------------------------------------------
 * Constructor and Destructor
 * --------------------------------------------------------------------------------
 */
SoMagicStylesKit::SoMagicStylesKit()
{
	SO_KIT_CONSTRUCTOR(SoMagicStylesKit);

	// Create the nodekit
	SO_KIT_ADD_CATALOG_ENTRY(sepTop,			SoGroup,		FALSE,	this,	\x0, TRUE);
    //SO_KIT_ADD_CATALOG_ENTRY(cxnRenderStyles,	SoNodeContext,	FALSE,	sepTop,	\x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(cxnRenderStyles,	SoNode,	FALSE,	sepTop,	\x0, TRUE);
	SO_KIT_ADD_FIELD(styleNames,	(0));
	SO_KIT_ADD_FIELD(stylesInside,	(0));
	SO_KIT_ADD_FIELD(stylesOutside,	(0));
    SO_KIT_INIT_INSTANCE();

	// Reset so fields are empty
	styleNames.deleteValues(0);
	stylesInside.deleteValues(0);
	stylesOutside.deleteValues(0);

	// Create the sensors
	styleNamesSensor=new SoFieldSensor(SoMagicStylesKit::stylesCB, this);
	stylesInsideSensor=new SoFieldSensor(SoMagicStylesKit::stylesCB, this);
	stylesOutsideSensor=new SoFieldSensor(SoMagicStylesKit::stylesCB, this);

	// A little setting up
	isLoaded=false;
	nNumberOfStyles=-1;
	handleCgFbo=new CgFboManager;

    stb::Starlight* starlight=(stb::Starlight*)(stb::MagicLens::getStarlight());
    cxnRenderStyles.setValue((SoNodeContext*)(starlight->createSoNodeContext()));

	this->setUpConnections(true, true);
}

SoMagicStylesKit::~SoMagicStylesKit()
{
}

SbBool SoMagicStylesKit::setUpConnections(SbBool onoff, SbBool doitalways)
{
	if ((doitalways == FALSE) && (connectionsSetUp == onoff)) return onoff;
	if (onoff) 
	{
		// We connect AFTER base class.
		SoBaseKit::setUpConnections(onoff, doitalways);

		// Attach the field sensors, always make sure to change first the 
		// extrusion vector
		styleNamesSensor->attach(&this->styleNames);
		stylesInsideSensor->attach(&this->stylesInside);
		stylesOutsideSensor->attach(&this->stylesOutside);

		refresh();
	}
	else 
	{
		// We disconnect BEFORE base class.
		styleNamesSensor->detach();
		stylesInsideSensor->detach();
		stylesOutsideSensor->detach();

		SoBaseKit::setUpConnections(onoff, doitalways);
	}
	return !(connectionsSetUp = onoff);
}

void SoMagicStylesKit::stylesCB(void *data, SoSensor *sensor)
{
	SoMagicStylesKit *mskTmp= (SoMagicStylesKit *)data;
	mskTmp->refresh();
}

void SoMagicStylesKit::GLRender(SoGLRenderAction * action)
{
	if (!isLoaded)
	{
		isLoaded=true;
		// Initialize Cg and the FBO
		handleCgFbo->init();
	}

	SoBaseKit::GLRender(action);

#if (defined(PRINT_FRAMERATE) && defined(WIN32))
	DWORD thisTime = GetTickCount();
	diffTime=thisTime-lastTime;
	if (diffTime>0) framerate=1000*(1/diffTime);
	lastTime=thisTime;
	printf("%c[Render = %3.1f]                                   ",13,framerate);
	touch();
#else
	//printf("[Render]\n");
#endif

}

void SoMagicStylesKit::setStyle(char *strStyleName, SoNode *innerStyle, SoNode *outerStyle)
{
	// Find strStyleName
	int nIndex=styleNames.find(strStyleName);

	// if found update stylesInside of the index returned
	if (nIndex>=0)
	{
		stylesInside.set1Value(nIndex, innerStyle);
		stylesOutside.set1Value(nIndex, outerStyle);
	}
	// if not add both
	else
	{
		nIndex=styleNames.getNum();
		styleNames.set1Value(nIndex,strStyleName);
		stylesInside.set1Value(nIndex, innerStyle);
		stylesOutside.set1Value(nIndex, outerStyle);
	}
}

void SoMagicStylesKit::refresh()
{
	int i;
	char *strTmp=new char[255];
	for (i=0;i<styleNames.getNum();i++)
	{
		if (i<stylesInside.getNum())
		{
			strcpy(strTmp, (char *) styleNames[i].getString());
			setStyle(strcat(strTmp,"InsideLens"), stylesInside[i]);
		}
		if (i<stylesOutside.getNum())
		{
			strcpy(strTmp, (char *) styleNames[i].getString());
			setStyle(strcat(strTmp,"OutsideLens"), stylesOutside[i]);
		}
	}
}

void SoMagicStylesKit::setStyle(char *strStyleName, SoNode *style)
{
	int nIndex=SO_GET_PART(this, "cxnRenderStyles", SoNodeContext)->index.find(strStyleName);

	if (nIndex>=0)
		SO_GET_PART(this, "cxnRenderStyles", SoNodeContext)->value.set1Value(nIndex, style);
	else 
	{
		nNumberOfStyles++;
		SO_GET_PART(this, "cxnRenderStyles", SoNodeContext)->index.set1Value(nNumberOfStyles, strStyleName);
		SO_GET_PART(this, "cxnRenderStyles", SoNodeContext)->value.set1Value(nNumberOfStyles, style);
	}
}

