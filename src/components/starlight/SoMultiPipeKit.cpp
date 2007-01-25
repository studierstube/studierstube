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
/** The Source file for the SoMultiPipeKit.
*
* @author Erick Mendez
*
* $Id: SoPipeKit.cpp 2006-03-10 mendez $
* @file                                                                   */
/* ======================================================================= */



/*
* --------------------------------------------------------------------------------
* Includes
* --------------------------------------------------------------------------------
*/
#include <stb/components/starlight/SoMultiPipeKit.h>
#include <stb/components/starlight/SoPipeKit.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <iostream>

using namespace std;

SO_KIT_SOURCE(SoMultiPipeKit);

/*
* --------------------------------------------------------------------------------
* Required by the node kit
* --------------------------------------------------------------------------------
*/
void SoMultiPipeKit::initClass(void)
{
	SO_KIT_INIT_CLASS(SoMultiPipeKit, SoBaseKit, "BaseKit");
}

/*
* --------------------------------------------------------------------------------
* Constructor and Destructor
* --------------------------------------------------------------------------------
*/
SoMultiPipeKit::SoMultiPipeKit()
{
	SO_KIT_CONSTRUCTOR(SoMultiPipeKit);

	// This is for the parts of the catalog
	SO_KIT_ADD_CATALOG_ENTRY(topSeparator,		    SoSeparator,		FALSE,	this,	\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(shapeHintsInternal,	SoShapeHints,		FALSE,	topSeparator,	\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(coordsInternal,		SoCoordinate3,		FALSE,	topSeparator,	\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(facesInternal,			SoIndexedFaceSet,	FALSE,	topSeparator,	\x0, TRUE);

	// This is for the Fields
	SO_KIT_ADD_FIELD(coords,	    (0,0,0));
    SO_KIT_ADD_FIELD(radius,	    (0));
    SO_KIT_ADD_FIELD(numFaces,	    (3));
    SO_KIT_ADD_FIELD(lineIndices,	(0));
    SO_KIT_ADD_FIELD(caps,		    (FALSE));
    SO_KIT_ADD_FIELD(doubleSided,	(FALSE));

    // The enumerations
    SO_KIT_DEFINE_ENUM_VALUE(Part, SIDES);
    SO_KIT_DEFINE_ENUM_VALUE(Part, TOP);
    SO_KIT_DEFINE_ENUM_VALUE(Part, BOTTOM);
    SO_KIT_DEFINE_ENUM_VALUE(Part, ALL);
    SO_KIT_SET_SF_ENUM_TYPE(parts, Part);

	SO_KIT_INIT_INSTANCE();

	// Create the Sensors
	coordsSensor=new SoFieldSensor(SoMultiPipeKit::refreshCB, this);
    radiusSensor=new SoFieldSensor(SoMultiPipeKit::refreshCB, this);
    doubleSidedSensor=new SoFieldSensor(SoMultiPipeKit::refreshCB, this);

    numOfInternalCoords=0;
    numOfInternalFaces=0;

	this->setUpConnections(TRUE, TRUE);
}

SoMultiPipeKit::~SoMultiPipeKit()
{
	delete coordsSensor;
    delete radiusSensor;
    delete doubleSidedSensor;
}

/*
* --------------------------------------------------------------------------------
* Protected functions including callbacks, and those requested by the nodekit
* --------------------------------------------------------------------------------
*/
SbBool SoMultiPipeKit::setUpConnections(SbBool onoff, SbBool doitalways)
{
	if ((doitalways == FALSE) && (connectionsSetUp == onoff)) return onoff;
	if (onoff) 
	{
		// We connect AFTER base class.
		inherited::setUpConnections(onoff, doitalways);

		coordsSensor->attach(&this->coords);
        radiusSensor->attach(&this->radius);
        doubleSidedSensor->attach(&this->doubleSided);

		refresh();
	}
	else 
	{
		// We disconnect BEFORE base class.
		coordsSensor->detach();
        radiusSensor->detach();
        doubleSidedSensor->detach();

		inherited::setUpConnections(onoff, doitalways);
	}
	return !(connectionsSetUp = onoff);
}

void SoMultiPipeKit::refreshCB(void *data, SoSensor * /*sensor*/)
{
	SoMultiPipeKit *tmp= (SoMultiPipeKit *)data;
	tmp->refresh();
}

void SoMultiPipeKit::refresh()
{
    SoCoordinate3 *coords=(SoCoordinate3 *)(this->getPart("coordsInternal", TRUE));
    SoIndexedFaceSet *faces=(SoIndexedFaceSet *)(this->getPart("facesInternal", TRUE));

    SoShapeHints *shapeHints=(SoShapeHints *)(this->getPart("shapeHintsInternal", TRUE));
    shapeHints->vertexOrdering=SoShapeHints::CLOCKWISE;
    shapeHints->faceType=SoShapeHints::CONVEX;

    if (doubleSided.getValue())
        shapeHints->shapeType=SoShapeHints::UNKNOWN_SHAPE_TYPE;
    else
        shapeHints->shapeType=SoShapeHints::SOLID;


    unsigned int k, startIndex, endIndex;
    k=0; startIndex=0;

    coords->point.deleteValues(0);
    faces->coordIndex.deleteValues(0);
    numOfInternalCoords=0;
    numOfInternalFaces=0;

    // There must be at least three faces
    if (numFaces.getValue()<3) numFaces.setValue(3);

    // Loop all the indices of line strings
    for (k=0;k<lineIndices.getNum();k++)
    {
        // Loop through all the vectors on this line string
        endIndex=startIndex+lineIndices[k];

        // Skip if only has one element or less
        if (lineIndices[k]>=2)
        {
            createOneLineString(startIndex,endIndex);
        }

        startIndex=endIndex;
    }
}

void SoMultiPipeKit::createOneLineString(int startIndex, int endIndex)
{
    int i;

    for (i=startIndex;i<endIndex-1;i++)
    {
        createOneSegment(coords[i],coords[i+1]);
    }
}


void SoMultiPipeKit::createOneSegment(SbVec3f start, SbVec3f end)
{
    SoCoordinate3 *coords=(SoCoordinate3 *)(this->getPart("coordsInternal", TRUE));
    SoIndexedFaceSet *faces=(SoIndexedFaceSet *)(this->getPart("facesInternal", TRUE));

    unsigned int nNextIndex, nNumberOfVertices, i;
    float magnitude, angle, theta, angleSlope, x, y, z;
    SbVec3f tmpVec, diffVec, axisVec, nextVertexBottom, nextVertexTop;

    /// ------------------------- Prepare everything

    nNumberOfVertices=numFaces.getValue();

    // Get the magnitude of the difference vector
    diffVec=end-start;
    magnitude=diffVec.length();

    // Get a rotation vector and its angle
    tmpVec.setValue(0, 0, 1);
    axisVec=diffVec.cross(tmpVec);
    axisVec.normalize();
    theta=getAngle(diffVec, tmpVec);

    // Prepare the angle and its slope
    angle=3.141592654/2;
    angleSlope=(2*3.141592654)/nNumberOfVertices;

    /// ------------------------- Add the vertices

    SbVec3f firstTerm, secondTerm, thirdTerm;
    // Add all the vertices
    for (i=0;i<nNumberOfVertices;i++)
    {

        x=radius.getValue()*(sin(angle));
        y=radius.getValue()*(cos(angle));
        z=0;

        // Calculate bottom vertex (without rotation)
        nextVertexBottom.setValue(x,y,z);

        // Rotate bottom about angle/axis
            // xcos(theta)
            firstTerm=nextVertexBottom;
            firstTerm*=cos(theta);

            // n(n.x)(1-cos(theta))
            secondTerm=axisVec;
            secondTerm*=axisVec.dot(nextVertexBottom);
            secondTerm*=(1-cos(theta));

            // (xXn)sin(theta)
            tmpVec=nextVertexBottom;
            thirdTerm=tmpVec.cross(axisVec);
            thirdTerm*=sin(theta);

            nextVertexBottom=firstTerm+secondTerm+thirdTerm;

        // Translate bottom vertex to start position
        nextVertexBottom=nextVertexBottom+start;

        // Add bottom vertex
        coords->point.set1Value(numOfInternalCoords+i,nextVertexBottom);

        // Calculate top vertex (without rotation)
        z=magnitude;
        nextVertexTop.setValue(x,y,z);


        // Rotate top about angle/axis
            // xcos(theta)
            firstTerm=nextVertexTop;
            firstTerm*=cos(theta);

            // n(n.x)(1-cos(theta))
            secondTerm=axisVec;
            secondTerm*=axisVec.dot(nextVertexTop);
            secondTerm*=(1-cos(theta));

            // (xXn)sin(theta)
            tmpVec=nextVertexTop;
            thirdTerm=tmpVec.cross(axisVec);
            thirdTerm*=sin(theta);

            nextVertexTop=firstTerm+secondTerm+thirdTerm;

        // Translate top vertex to start position
        nextVertexTop=nextVertexTop+start;

        // Add top vertex
        coords->point.set1Value(numOfInternalCoords+i+nNumberOfVertices,nextVertexTop);

        // Change the angle for the next vertex
        angle-=angleSlope;
    }

    // ------------------------- Add the faces

    int old=numOfInternalFaces;

    // Create Bottom Face
    if (parts.getValue()&SoMultiPipeKit::BOTTOM)
    {
        // Loop the number of all the new bottom vertices
        // Then add them at the appropriate GLOBAL positions of faces
        for (i=0;i<nNumberOfVertices;i++)
            faces->coordIndex.set1Value(numOfInternalFaces+i,numOfInternalCoords+i);

        // Finish this face
        faces->coordIndex.set1Value(numOfInternalFaces+nNumberOfVertices,-1);

        // Update the number of internal faces
        numOfInternalFaces=numOfInternalFaces+nNumberOfVertices;
    }

    // Create Top Face
    if (parts.getValue()&SoMultiPipeKit::TOP)
    {
        // Loop the number of all the new bottom vertices
        // Then add them at the appropriate GLOBAL positions of faces
        for (i=0;i<nNumberOfVertices;i++)
            faces->coordIndex.set1Value(numOfInternalFaces+i,numOfInternalCoords+nNumberOfVertices+i);

        // Finish this face
        faces->coordIndex.set1Value(numOfInternalFaces+nNumberOfVertices,-1);

        // Update the number of internal faces
        numOfInternalFaces=numOfInternalFaces+nNumberOfVertices;
    }

    // Create the rest of the cylinder faces except for the last
    for (i=0;i<nNumberOfVertices-1;i++)
    {
        nNextIndex=numOfInternalCoords+i;
        faces->coordIndex.set1Value(numOfInternalFaces,		nNextIndex);
        faces->coordIndex.set1Value(numOfInternalFaces+1,	nNextIndex+nNumberOfVertices);
        faces->coordIndex.set1Value(numOfInternalFaces+2,	nNextIndex+nNumberOfVertices+1);
        faces->coordIndex.set1Value(numOfInternalFaces+3,	nNextIndex+1);
        faces->coordIndex.set1Value(numOfInternalFaces+4,	-1);

        // Update the number of internal faces
        numOfInternalFaces=numOfInternalFaces+5;
    }

    // Create the last of the cylinder faces
    faces->coordIndex.set1Value(numOfInternalFaces,		numOfInternalCoords+(nNumberOfVertices-1));
    faces->coordIndex.set1Value(numOfInternalFaces+1,	numOfInternalCoords+(nNumberOfVertices*2)-1);
    faces->coordIndex.set1Value(numOfInternalFaces+2,	numOfInternalCoords+nNumberOfVertices);
    faces->coordIndex.set1Value(numOfInternalFaces+3,	numOfInternalCoords);
    faces->coordIndex.set1Value(numOfInternalFaces+4,	-1);

    // Update the number of internal faces
    numOfInternalFaces=numOfInternalFaces+5;

    // Update the number of internal coordinates
    numOfInternalCoords=numOfInternalCoords+(nNumberOfVertices*2);
}