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
/** The Source file for the SoXRayKit
*
* @author Erick Mendez
*
* $Id: SoXRayKit.cpp 2006-03-10 mendez $
* @file                                                                   */
/* ======================================================================= */


/*
 * --------------------------------------------------------------------------------
 * Includes
 * --------------------------------------------------------------------------------
 */
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/nodes/SoCamera.h>
#include <stb/components/magicLens/SoXRayKit.h>

using namespace std;

SO_KIT_SOURCE(SoXRayKit);

void SoXRayKit::initClass(void)
{
    SO_KIT_INIT_CLASS(SoXRayKit, SoBaseKit, "BaseKit");
}

SoXRayKit::SoXRayKit()
{

	SO_KIT_CONSTRUCTOR(SoXRayKit);

	// This is for the parts of the catalog
	SO_KIT_ADD_CATALOG_ENTRY(lensKit,	SoMagicLensKit, TRUE,	this,	\x0, TRUE);

	// This is for the Fields
	SO_KIT_ADD_FIELD(ratio,		(0));
	SO_KIT_ADD_FIELD(camera,	(NULL));

	SO_KIT_INIT_INSTANCE();

	// Create the Sensors
	ratioSensor=new SoFieldSensor(SoXRayKit::ratioCB, this);

	coords=new SoCoordinate3;
	faces=new SoIndexedFaceSet;
	shapeHints=new SoShapeHints;

	// This has to be done only once, we know all the vertices are in
	// counterclockwise order and that they could be concave
	shapeHints->vertexOrdering=SoShapeHints::COUNTERCLOCKWISE;
	shapeHints->faceType=SoShapeHints::UNKNOWN_FACE_TYPE;

	// The Viewing tunnel always has the same shape
	int indices[30]={ 
					0, 1, 2, 3, -1, 
					5, 4, 7, 6, -1,
					1, 5, 6, 2, -1,
					2, 6, 7, 3, -1,
					4, 0, 3, 7, -1,
					0, 4, 5, 1, -1 };
	faces->coordIndex.setValues(0,30, indices);

	// The lens is created only once
	SoMagicLens *myLens=new SoMagicLens;
	myLens->addChild(shapeHints);
	myLens->addChild(coords);
	myLens->addChild(faces);
	SoMagicLensKit *myLensKit=(SoMagicLensKit *)(this->getPart("lensKit", TRUE));
	myLensKit->setContent(myLens);

	//myLensKit->goMagic(FALSE);

	this->setUpConnections(TRUE, TRUE);
}

SoXRayKit::~SoXRayKit()
{
}

SbBool SoXRayKit::setUpConnections(SbBool onoff, SbBool doitalways)
{
	if ((doitalways == FALSE) && (connectionsSetUp == onoff)) return onoff;
	if (onoff) 
	{
		// We connect AFTER base class.
		SoBaseKit::setUpConnections(onoff, doitalways);
		ratioSensor->attach(&this->ratio);
		refresh();
	}
	else 
	{
		// We disconnect BEFORE base class.
		ratioSensor->detach();
		SoBaseKit::setUpConnections(onoff, doitalways);
	}
	return !(connectionsSetUp = onoff);
}

void SoXRayKit::ratioCB(void* data, SoSensor* sensor)
{
	SoXRayKit *tmp=(SoXRayKit *)data;
	tmp->refresh();
}

void SoXRayKit::refresh()
{
	SbVec3f punto;
	static SbVec3f lastPosition=SbVec3f(0,0,0);
	static SbRotation lastOrientation=SbRotation(0,0,1,0);

	// We retrieve the camera
	SoCamera *myCam=(SoCamera *)camera.getValue();
	if (myCam!=NULL)
	{
		// Check if the position or orientation has changed
		if ((myCam->position.getValue()!=lastPosition)||(myCam->orientation.getValue()!=lastOrientation))
		{
			lastPosition=myCam->position.getValue();
			lastOrientation=myCam->orientation.getValue();

			// Get it's vieweing volume
			SbViewVolume myVol=myCam->getViewVolume();

			// Calculate the 2D points we will try to get from the planes
			SbVec2f coords2d;
			coords2d.setValue((1-ratio.getValue())/2,((1-ratio.getValue())/2)+ratio.getValue());
			printf("[%f,%f]\n",coords2d[0],coords2d[1]);

			// Get the 2D points from the near plane
			punto=myVol.getPlanePoint(myCam->nearDistance.getValue(),SbVec2f(coords2d[0],coords2d[0]));
			coords->point.set1Value(0,punto);
			punto=myVol.getPlanePoint(myCam->nearDistance.getValue(),SbVec2f(coords2d[1],coords2d[0]));
			coords->point.set1Value(1,punto);
			punto=myVol.getPlanePoint(myCam->nearDistance.getValue(),SbVec2f(coords2d[1],coords2d[1]));
			coords->point.set1Value(2,punto);
			punto=myVol.getPlanePoint(myCam->nearDistance.getValue(),SbVec2f(coords2d[0],coords2d[1]));
			coords->point.set1Value(3,punto);

			// Get the 2D points from the far plane
			punto=myVol.getPlanePoint(myCam->farDistance.getValue(),SbVec2f(coords2d[0],coords2d[0]));
			coords->point.set1Value(4,punto);
			punto=myVol.getPlanePoint(myCam->farDistance.getValue(),SbVec2f(coords2d[1],coords2d[0]));
			coords->point.set1Value(5,punto);
			punto=myVol.getPlanePoint(myCam->farDistance.getValue(),SbVec2f(coords2d[1],coords2d[1]));
			coords->point.set1Value(6,punto);
			punto=myVol.getPlanePoint(myCam->farDistance.getValue(),SbVec2f(coords2d[0],coords2d[1]));
			coords->point.set1Value(7,punto);
		}
	}
}

void SoXRayKit::GLRender(SoGLRenderAction * action)
{
	refresh();
	SoBaseKit::GLRender(action);
}
