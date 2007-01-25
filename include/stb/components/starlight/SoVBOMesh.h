#ifndef  _SO_VBOMESH_
#define  _SO_VBOMESH_

#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <vector>
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif
#include <gl/GL.h>
//


class SoVBOMesh : public SoIndexedFaceSet
{
   SO_NODE_HEADER(SoVBOMesh);
public:
    SoVBOMesh();
    ~SoVBOMesh();

   static void initClass();

   SoSFString meshFile;
   SoSFVec3f  color;
   SoSFFloat  opacity;
protected:
   virtual void GLRender(SoGLRenderAction * action);

   bool isInit;
   virtual void init();
   virtual void buildVBO();

   GLuint vboVertices;
   GLuint vboColors;
   GLuint vboNormals;

   struct Vertex {float x,y,z;};
   int      vertexCount;
   Vertex*  vertexList;

   struct Color{float r,g,b,a;};
   Color* colorList; 

   struct Normal{float nx,ny,nz;};
   Normal* normalList; 

   long  polygonCount;
   int** indexedPolygonList;//indexedPolygonList[k][i] = index of vertex i of polygon k
   int*  polygonSizeList;   //polygonSizeList[i]= number of vertices of polygon i

};

#endif
