/* ========================================================================
* Copyright (c) 2006,
* Institute for Computer Graphics and Vision
* Graz University of Technology
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are
* met:
*
* Redistributions of source code must retain the above copyright notice,
* this list of conditions and the following disclaimer.
*
* Redistributions in binary form must reproduce the above copyright
* notice, this list of conditions and the following disclaimer in the
* documentation and/or other materials provided with the distribution.
*
* Neither the name of the Graz University of Technology nor the names of
* its contributors may be used to endorse or promote products derived from
* this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
* IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
* TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
* OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
* PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
* ========================================================================
* PROJECT: OpenTracker
* ======================================================================== */
/** Source File for the SoExtrusionKit
*
*
* @author Erick Mendez
*
* $Id: $
* @file                                                                   */
/* ======================================================================= */

/*
 * --------------------------------------------------------------------------------
 * Includes
 * --------------------------------------------------------------------------------
 */
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <stb/components/starlight/SoExtrusionKit.h>
#include <iostream>

using namespace std;

SO_KIT_SOURCE(SoExtrusionKit);

/*
 * --------------------------------------------------------------------------------
 * Required by the node kit
 * --------------------------------------------------------------------------------
 */
void SoExtrusionKit::initClass(void)
{
    SO_KIT_INIT_CLASS(SoExtrusionKit, SoBaseKit, "BaseKit");
}

/*
 * --------------------------------------------------------------------------------
 * Constructor and Destructor
 * --------------------------------------------------------------------------------
 */
SoExtrusionKit::SoExtrusionKit()
{
	SO_KIT_CONSTRUCTOR(SoExtrusionKit);

	// This is for the parts of the catalog
	SO_KIT_ADD_CATALOG_ENTRY(topSeparator,	SoSeparator,		FALSE,	this,			\x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(shapeHints,	SoShapeHints,		FALSE,	topSeparator,	\x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(coords,		SoCoordinate3,		FALSE,	topSeparator,	\x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(faces,			SoIndexedFaceSet,	FALSE,	topSeparator,	\x0, TRUE);

	// This is for the Fields
	SO_KIT_ADD_FIELD(vertices,			(0,0,0));
	SO_KIT_ADD_FIELD(extrusionVector,	(0,0,0));

    SO_KIT_INIT_INSTANCE();

	// Create the Sensors
	verticesSensor=new SoFieldSensor(SoExtrusionKit::verticesCB, this);
	extrusionVectorSensor=new SoFieldSensor(SoExtrusionKit::extrusionVectorCB, this);

	// This has to be done only once, we know all the vertices are in
	// counterclockwise order and that they could be concave
	SoShapeHints *shapeHints=(SoShapeHints *)(this->getPart("shapeHints", TRUE));
	shapeHints->vertexOrdering=SoShapeHints::COUNTERCLOCKWISE;
	shapeHints->faceType=SoShapeHints::UNKNOWN_FACE_TYPE;

	this->setUpConnections(TRUE, TRUE);
}

SoExtrusionKit::~SoExtrusionKit()
{
	delete verticesSensor;
	delete extrusionVectorSensor;
}

/*
 * --------------------------------------------------------------------------------
 * Protected functions including callbacks, and those requested by the nodekit
 * --------------------------------------------------------------------------------
 */
SbBool SoExtrusionKit::setUpConnections(SbBool onoff, SbBool doitalways)
{
    if ((doitalways == FALSE) && (connectionsSetUp == onoff)) return onoff;
    if (onoff) 
    {
        // We connect AFTER base class.
        inherited::setUpConnections(onoff, doitalways);

		// Attach the field sensors, always make sure to change first the 
		// extrusion vector
		verticesSensor->attach(&this->vertices);
		extrusionVectorSensor->attach(&this->extrusionVector);

		refresh();
    }
    else 
    {
        // We disconnect BEFORE base class.
		verticesSensor->detach();
		extrusionVectorSensor->detach();

        inherited::setUpConnections(onoff, doitalways);
    }
    return !(connectionsSetUp = onoff);
}

void SoExtrusionKit::verticesCB(void *data, SoSensor *sensor)
{
    SoExtrusionKit *exkTmp= (SoExtrusionKit *)data;
	exkTmp->refresh();
}

void SoExtrusionKit::extrusionVectorCB(void *data, SoSensor *sensor)
{
    SoExtrusionKit *exkTmp= (SoExtrusionKit *)data;
	exkTmp->refresh();
}

void SoExtrusionKit::refresh()
{
	unsigned int i, nNumberOfVertices, nNumberOfCoordinates, nNextLimit, nNextIndex;

	nNumberOfVertices=vertices.getNum();
	SoCoordinate3 *coords=(SoCoordinate3 *)(this->getPart("coords", TRUE));
	SoIndexedFaceSet *faces=(SoIndexedFaceSet *)(this->getPart("faces", TRUE));

	// Attach footprint coordinates
	for (i=0;i<nNumberOfVertices;i++)
		coords->point.set1Value(i,vertices[i]);

	// Attach extruded footprint coordinates
	for (i=0;i<nNumberOfVertices;i++)
		coords->point.set1Value(i+nNumberOfVertices,vertices[i].getValue()+extrusionVector.getValue());

	// This is the total of coordinates in 'coords'
	nNumberOfCoordinates=nNumberOfVertices*2;
	
	// Create footprint face
	nNextIndex=0;
	nNextLimit=nNumberOfVertices;
	for (i=nNextIndex;i<nNextLimit;i++)
		faces->coordIndex.set1Value(i,i);
	faces->coordIndex.set1Value(nNextLimit,-1);

	// Create footprint extruded face
	nNextIndex=nNextLimit+1;
	nNextLimit=nNextIndex+nNumberOfVertices;
	for (i=0;i<nNumberOfVertices;i++)
		faces->coordIndex.set1Value(i+nNextIndex,nNextLimit-i-2);
	faces->coordIndex.set1Value(nNextLimit,-1);

	// Create the rest of the cylinder faces except for the last
	nNextIndex=nNextLimit+1;
	for (i=0;i<nNumberOfVertices-1;i++)
	{
		nNextLimit=(i*5+nNextIndex);
		faces->coordIndex.set1Value(nNextLimit,		i);
		faces->coordIndex.set1Value(nNextLimit+1,	i+nNumberOfVertices);
		faces->coordIndex.set1Value(nNextLimit+2,	i+nNumberOfVertices+1);
		faces->coordIndex.set1Value(nNextLimit+3,	i+1);
		faces->coordIndex.set1Value(nNextLimit+4,	-1);
	}

	// Create the last of the cylinder faces
	i=nNumberOfVertices-1;
	nNextLimit=(i*5+nNextIndex);
	faces->coordIndex.set1Value(nNextLimit,		nNumberOfVertices-1);
	faces->coordIndex.set1Value(nNextLimit+1,	nNumberOfVertices*2-1);
	faces->coordIndex.set1Value(nNextLimit+2,	nNumberOfVertices);
	faces->coordIndex.set1Value(nNextLimit+3,	0);
	faces->coordIndex.set1Value(nNextLimit+4,	-1);
}
