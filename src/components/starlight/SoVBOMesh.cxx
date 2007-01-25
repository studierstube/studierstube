#include <stb/components/starlight/SoVBOMesh.h>
#include <stb/components/starlight/InventorUtils.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/nodes/SoCoordinate3.h> 
#include <Inventor/elements/SoGLLazyElement.h>
#include <Inventor/elements/SoCacheElement.h>


#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <gl\gl.h>												// Header File For The OpenGL32 Library

SO_NODE_SOURCE(SoVBOMesh);

void 
SoVBOMesh::initClass()
{
   SO_NODE_INIT_CLASS(SoVBOMesh, SoIndexedFaceSet, "IndexedFaceSet");
  
}

SoVBOMesh::SoVBOMesh()
{   
    SO_NODE_CONSTRUCTOR(SoVBOMesh); 
    SO_NODE_ADD_FIELD(meshFile,(""));
    SO_NODE_ADD_FIELD(color,(0.0,0.0,0.0));
    SO_NODE_ADD_FIELD(opacity,(0.0));
    isInit=false;
}


SoVBOMesh::~SoVBOMesh()
{   
    delete[] vertexList;
    delete[] colorList;
    delete[] normalList;
}


void
SoVBOMesh::init()
{
    /////////////////////////////////////////////////////////////////////////    
    ////////////////// load mesh file ///////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////    
    vertexProperty.setValue(new SoVertexProperty());
    printf("ColorMesh:: reading meshFile %s\n",meshFile.getValue().getString());
    InventorUtils ivUtil;
    SoSeparator* fileRoot=ivUtil.loadFile(meshFile.getValue().getString());
    fileRoot->ref();
    //SoPathList pathsIdxFaceSet;
    //SoPathList pathsIdxVertexProp;
    //SoPathList pathsCoord3;
    SoIndexedFaceSet* idxFaceSet=(SoIndexedFaceSet*)ivUtil.find(SoIndexedFaceSet::getClassTypeId(),fileRoot);
    if(!idxFaceSet)
    {
        printf("SoVBOMesh:: couldn't find SoIndexFaceSet \n");
        return ;
    }
    printf("SoVBOMesh::found SoIndexFaceSet in file '%s'\n",meshFile.getValue().getString());
    this->coordIndex=idxFaceSet->coordIndex;
    if(idxFaceSet->vertexProperty.getValue())
    {
        printf("SoVBOMesh:: found SoVertexProperty \n");
        ((SoVertexProperty*)vertexProperty.getValue())->vertex=((SoVertexProperty*)idxFaceSet->vertexProperty.getValue())->vertex;
    }
    else 
    {    
        printf("SoVBOMesh:: couldn't find SoVertexProperty... searching for SoCoordinate3 in %s \n",meshFile.getValue().getString());
        SoCoordinate3* coord3=(SoCoordinate3*)ivUtil.find(SoCoordinate3::getClassTypeId(),fileRoot);
        if(coord3)
        {
            printf("SoVBOMesh:: found SoCoordinate3 \n");
            coord3->ref();
            ((SoVertexProperty*)vertexProperty.getValue())->vertex=coord3->point;
            coord3->unref();
        }
        else
        {
            printf("SoVBOMesh:: couldn't find SoCoordinate3 \n");
            return;
        }
    }
    //((SoVertexProperty*)vertexProperty.getValue())->normalBinding=SoVertexProperty::PER_VERTEX;
    fileRoot->unref();
    //////////////////////////////////////////////////////////
    // setup vertexList & colorList
    vertexCount   =((SoVertexProperty*)vertexProperty.getValue())->vertex.getNum();
    vertexList    = new Vertex[vertexCount];						// Allocate Vertex Data
    colorList     = new Color [vertexCount];
    normalList    = new Normal[vertexCount];

    printf("vertexCount %i \n",vertexCount);

    float r,g,b,a;
    color.getValue().getValue(r,g,b);
    a=opacity.getValue();
    for(int i=0;i<vertexCount;i++)
    {
        // fill vertList
        ((SoVertexProperty*)vertexProperty.getValue())->vertex[i].getValue(vertexList[i].x,
                                                                           vertexList[i].y,
                                                                           vertexList[i].z);
        colorList[i].r=r;
        colorList[i].g=g;
        colorList[i].b=b;
        colorList[i].a=a;
    }
    //////////////////////////////////////////////////////////////////////////    
    buildVBO();

    isInit=true;
}

void 
SoVBOMesh::buildVBO()
{
    //if (!CgGlManager::getInstance()->isExtensionSupported("GL_ARB_vertex_buffer_object"))
    //{
    //    printf("SoVBOMesh: GL_ARB_vertex_buffer_object not available.\n");
    //    assert(0);
    //}
    //else
    //    printf("SoVBOMesh: GL_ARB_vertex_buffer_object available.\n");

    // Generate And Bind The Vertex Buffer
    // vertices
    glEnableClientState(GL_VERTEX_ARRAY);	
        glGenBuffersARB(1, &vboVertices);							 // Get A Valid Name
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboVertices);			// Bind The Buffer   
        glBufferDataARB(GL_ARRAY_BUFFER_ARB, vertexCount*3*sizeof(float), vertexList, GL_STATIC_DRAW_ARB);// Load Vertex Data Into The Graphics Card Memory
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);			// Bind The Buffer   
    glDisableClientState(GL_VERTEX_ARRAY);	

    //color
    glEnableClientState(GL_COLOR_ARRAY);
        glGenBuffersARB(1, &vboColors);
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboColors);
        glBufferDataARB(GL_ARRAY_BUFFER_ARB,vertexCount*4*sizeof(float),  colorList, GL_DYNAMIC_DRAW);
    glDisableClientState(GL_COLOR_ARRAY);

    ////set up polygon index lists
    //printf("coordIndex.getNum()= %i",coordIndex.getNum());   
    struct IndexedFace{std::vector<int> indices;};
    std::vector<IndexedFace*> faceList;
    IndexedFace* curFace=new IndexedFace;
    for(int i=coordIndex.getNum()-1;i>=0;i--)
    {
        if(coordIndex[i]==-1)
        {//push face            
            faceList.push_back(curFace);
            curFace=new IndexedFace;
        }
        else{
            curFace->indices.push_back(coordIndex[i]);
        }
    }
    delete curFace;
    polygonCount=(int)faceList.size();    
    printf("SoVBOMesh::polygonCount=%i \n",polygonCount);
    polygonSizeList   =new int[polygonCount];//polygonSizeList[i]= number of vertices of polygon i
    indexedPolygonList=new int*[polygonCount];//indexedPolygonList[k][i] = index of vertex i of polygon k    
    for(int i=polygonCount-1;i>=0;i--)
    {
        int numOfVert=(int)(faceList[i]->indices.size());//number of vertices of face i
        polygonSizeList[i]=numOfVert;
        indexedPolygonList[i]=new int[numOfVert];
        int k=0;
        for(int j=(int)(faceList[i]->indices.size())-1;j>=0;j--,k++)
        {
            indexedPolygonList[i][k]=faceList[i]->indices[j];
        }
        
    }

 
    //set up color buffer 
    //color
    glEnableClientState(GL_COLOR_ARRAY);
        glGenBuffersARB(1, &vboColors);
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboColors);
        glBufferDataARB(GL_ARRAY_BUFFER_ARB,vertexCount*4*sizeof(float),  colorList, GL_DYNAMIC_DRAW);
    glDisableClientState(GL_COLOR_ARRAY);

    //compute normals per vertex
    for(int i=0;i<vertexCount;i++)
    {
        normalList[i].nx=1.0;
        normalList[i].ny=0.0;
        normalList[i].nz=0.0;           
    }


    for(int i=(int)faceList.size()-1;i>=0;i--)
    {//per face i
        int numOfVertPerFace=(int)(faceList[i]->indices.size());//number of vertices of face i
        if(numOfVertPerFace<3)
        {
            printf("less than 3 vertices per face --> can't compute a normal for this face -- set normal to vn(1,0,0)\n");
        }
        else{
            int vertexIdx0=faceList[i]->indices[0];
            int vertexIdx1=faceList[i]->indices[1];
            int vertexIdx2=faceList[i]->indices[2];

            SbVec3f a=SbVec3f(vertexList[vertexIdx1].x,vertexList[vertexIdx1].y,vertexList[vertexIdx1].z)
                     -SbVec3f(vertexList[vertexIdx0].x,vertexList[vertexIdx0].y,vertexList[vertexIdx0].z);
            SbVec3f b=SbVec3f(vertexList[vertexIdx2].x,vertexList[vertexIdx2].y,vertexList[vertexIdx2].z)
                     -SbVec3f(vertexList[vertexIdx0].x,vertexList[vertexIdx0].y,vertexList[vertexIdx0].z);
            SbVec3f n=b.cross(a);
            n.normalize();
            for (int j=0;j<numOfVertPerFace;j++)
            {
                n.getValue( normalList[faceList[i]->indices[j]].nx,
                            normalList[faceList[i]->indices[j]].ny,
                            normalList[faceList[i]->indices[j]].nz
                          );
            }
        }
        
        delete faceList[i];
    }
    ////delete faces 
    faceList.empty();

    glEnableClientState(GL_NORMAL_ARRAY);
    glGenBuffersARB(1, &vboNormals);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB,vboNormals);
    glBufferDataARB(GL_ARRAY_BUFFER_ARB,vertexCount*3*sizeof(float),  normalList,  GL_STATIC_DRAW_ARB);
    glDisableClientState(GL_NORMAL_ARRAY);
}

void
SoVBOMesh::GLRender(SoGLRenderAction *action)
{
    SoState* state=action->getState();
    if(!isInit){
        init();           
    }

    SoIndexedFaceSet::GLRender(action);

    ////printf("--> in glrender -- normal count %i \n",((SoVertexProperty*)vertexProperty.getValue())->normal.getNum());
    ////////////////////////////////////////////////////////////////////////////
    ////vertices 
    glEnableClientState( GL_VERTEX_ARRAY );            
    glBindBufferARB( GL_ARRAY_BUFFER_ARB, vboVertices);    
    glVertexPointer( 3, GL_FLOAT, 0, (char*)NULL);
    
    //////////////////////////////////////////////////////////////////////////    
    //color 
    glEnableClientState(GL_COLOR_ARRAY);
    glBindBufferARB( GL_ARRAY_BUFFER_ARB, vboColors );        
    //update colors
    glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, vertexCount*4*sizeof(float),colorList);        
    glColorPointer( 4,GL_FLOAT, 0, (char*) NULL );

    /// material
    //const float adiff[4] = { 1.0, 1.0, 1.0, 1.0 };
    //const float spec[4] = { 0.2,  0.2, 0.2, 1.0 };
    //const float shin = 64.0;
    //glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, adiff);
    //glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
    //glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shin);
    //glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    //glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
    //glEnable(GL_COLOR_MATERIAL);         



    //////////////////////////////////////////////////////////////////////////    
    //normals 
    glEnableClientState( GL_NORMAL_ARRAY );						// Enable Vertex Arrays
    glBindBufferARB( GL_ARRAY_BUFFER_ARB, vboNormals );
    glNormalPointer( GL_FLOAT, 0, (char*)NULL  );


    //glEnable(GL_BLEND);
    //glDisable(GL_CULL_FACE);
    //render 
    //indexedPolygonList[k][i] = index of vertex i of polygon k
    //polygonSizeList[i]= number of vertices of polygon i
    glMultiDrawElements(    GL_POLYGON,		    //GLenum mode
                            (GLsizei*)polygonSizeList,    //const GLsizei *count 
                            GL_UNSIGNED_INT,      //GLenum type
                            (const GLvoid **)indexedPolygonList,  //const GLvoid* *indices
                            (GLsizei)polygonCount);           //GLsizei primcount
            
    //Disable vbo
    glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
    glDisableClientState(GL_VERTEX_ARRAY );
    glDisableClientState(GL_COLOR_ARRAY  );	
    glDisableClientState(GL_NORMAL_ARRAY  );	

}


//----------------------------------------------------------------------
// End of SoColorMesh.cxx
//----------------------------------------------------------------------
// Local Variables:
// mode: c++
// c-basic-offset: 3
// End:
//----------------------------------------------------------------------
