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
/** Source File for the SoLineStringKit
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
#include <stb/components/starlight/SoLineStringKit.h>
#include <iostream>

using namespace std;

SO_KIT_SOURCE(SoLineStringKit);

/*
 * --------------------------------------------------------------------------------
 * Required by the node kit
 * --------------------------------------------------------------------------------
 */
void SoLineStringKit::initClass(void)
{
    SO_KIT_INIT_CLASS(SoLineStringKit, SoBaseKit, "BaseKit");
}

/*
 * --------------------------------------------------------------------------------
 * Constructor and Destructor
 * --------------------------------------------------------------------------------
 */
SoLineStringKit::SoLineStringKit()
{
	SO_KIT_CONSTRUCTOR(SoLineStringKit);

	// This is for the parts of the catalog
	SO_KIT_ADD_CATALOG_ENTRY(topSeparator,	SoSeparator,		FALSE,	this,			\x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(shapeHints,	SoShapeHints,		FALSE,	topSeparator,	\x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(coords,		SoCoordinate3,		FALSE,	topSeparator,	\x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(faces,			SoIndexedFaceSet,	FALSE,	topSeparator,	\x0, TRUE);

	// This is for the Fields
	SO_KIT_ADD_FIELD(vertices,		(0,0));
	SO_KIT_ADD_FIELD(width,			(0));

    SO_KIT_INIT_INSTANCE();

	// Create the Sensors
	verticesSensor=new SoFieldSensor(SoLineStringKit::verticesCB, this);
	widthSensor=new SoFieldSensor(SoLineStringKit::widthCB, this);

	// This has to be done only once, we know all the vertices are in
	// counterclockwise order and that they could be concave
	SoShapeHints *shapeHints=(SoShapeHints *)(this->getPart("shapeHints", TRUE));
	shapeHints->vertexOrdering=SoShapeHints::COUNTERCLOCKWISE;
	shapeHints->faceType=SoShapeHints::UNKNOWN_FACE_TYPE;

	this->setUpConnections(TRUE, TRUE);
}

SoLineStringKit::~SoLineStringKit()
{
	delete verticesSensor;
	delete widthSensor;
}

/*
 * --------------------------------------------------------------------------------
 * Protected functions including callbacks, and those requested by the nodekit
 * --------------------------------------------------------------------------------
 */
SbBool SoLineStringKit::setUpConnections(SbBool onoff, SbBool doitalways)
{
    if ((doitalways == FALSE) && (connectionsSetUp == onoff)) return onoff;
    if (onoff) 
    {
        // We connect AFTER base class.
        inherited::setUpConnections(onoff, doitalways);

		// Attach the field sensors, always make sure to change first the 
		// extrusion vector
		verticesSensor->attach(&this->vertices);
		widthSensor->attach(&this->width);

		refresh();
    }
    else 
    {
        // We disconnect BEFORE base class.
		verticesSensor->detach();
		widthSensor->detach();

        inherited::setUpConnections(onoff, doitalways);
    }
    return !(connectionsSetUp = onoff);
}

void SoLineStringKit::verticesCB(void *data, SoSensor *sensor)
{
    SoLineStringKit *exkTmp= (SoLineStringKit *)data;
	exkTmp->refresh();
}

void SoLineStringKit::widthCB(void *data, SoSensor *sensor)
{
    SoLineStringKit *exkTmp= (SoLineStringKit *)data;
	exkTmp->refresh();
}

void SoLineStringKit::refresh()
{
	unsigned int i, nNumberOfVertices;

	nNumberOfVertices=vertices.getNum();
	if (nNumberOfVertices<2) return;

	SoCoordinate3 *coords=(SoCoordinate3 *)(this->getPart("coords", TRUE));
	SoIndexedFaceSet *faces=(SoIndexedFaceSet *)(this->getPart("faces", TRUE));

	// These will be used as normals to the current line
	SbVec3f normalPrevious, normalNext, tmpVertex;

	// Remember, the points are the center line, we use the normal to make the "width"
	// of the line to follow the normal to itself

	// The first point
	normalNext[0]=-(vertices[1][1]-vertices[0][1]);
	normalNext[1]=vertices[1][0]-vertices[0][0];
	normalNext[2]=0;
	normalNext.normalize();
	normalNext*=width.getValue();
	tmpVertex[0]=vertices[0][0];
	tmpVertex[1]=vertices[0][1];
	tmpVertex[2]=0;
	coords->point.set1Value(0,tmpVertex+normalNext);
	coords->point.set1Value((nNumberOfVertices*2)-1,tmpVertex-normalNext);

	// The second to the second to last
	for (i=1;i<nNumberOfVertices-1;i++)
	{
		normalPrevious[0]=-(vertices[i][1]-vertices[i-1][1]);
		normalPrevious[1]=vertices[i][0]-vertices[i-1][0];
		normalPrevious[2]=0;

		normalNext[0]=-(vertices[i+1][1]-vertices[i][1]);
		normalNext[1]=vertices[i+1][0]-vertices[i][0];
		normalNext[2]=0;

		normalPrevious+=normalNext;
		normalPrevious.normalize();
		normalPrevious*=width.getValue();

		tmpVertex[0]=vertices[i][0];
		tmpVertex[1]=vertices[i][1];
		coords->point.set1Value(i,tmpVertex+normalPrevious);
		coords->point.set1Value((nNumberOfVertices*2)-i-1,tmpVertex-normalPrevious);
	}

	// The last point
	normalPrevious[0]=-(vertices[nNumberOfVertices-1][1]-vertices[nNumberOfVertices-2][1]);
	normalPrevious[1]=vertices[nNumberOfVertices-1][0]-vertices[nNumberOfVertices-2][0];
	normalPrevious[2]=0;
	normalPrevious.normalize();
	normalPrevious*=width.getValue();
	tmpVertex[0]=vertices[nNumberOfVertices-1][0];
	tmpVertex[1]=vertices[nNumberOfVertices-1][1];
	coords->point.set1Value(nNumberOfVertices-1,tmpVertex+normalPrevious);
	coords->point.set1Value(nNumberOfVertices,tmpVertex-normalPrevious);

	// Create footprint face
	for (i=0;i<nNumberOfVertices*2;i++)
		faces->coordIndex.set1Value(i,i);
	faces->coordIndex.set1Value(nNumberOfVertices*2,-1);
}

