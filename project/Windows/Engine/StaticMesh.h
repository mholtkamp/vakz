#ifndef STATIC_MESH_H
#define STATIC_MESH_H

#include "Mesh.h"

class StaticMesh : public Mesh
{

public:

    //## **********************************************************************
    //## Constructor
    //## **********************************************************************
    StaticMesh();

    //## **********************************************************************
    //## Destructor
    //## **********************************************************************
    ~StaticMesh();

    //## **********************************************************************
    //## Load
    //##
    //## Loads a static mesh from an OBJ file. This function creates a vertex
    //## buffer object that holds the position, texcoord, and normal arrays.
    //## 
    //## Input:
    //##   pFile - filename string
    //##
    //## Returns:
    //##   int - '1' if mesh was successfully loaded.
    //##       - '0' otherwise.
    //## **********************************************************************
    int Load(const char* pFile);

    //## **********************************************************************
    //## SetVertexCount
    //##
    //## Sets the number of vertices that the static mesh contains.
    //## 
    //## Input:
    //##   nCount - the number of vertices to draw.
    //## **********************************************************************
    void SetVertexCount(int nCount);

    //## **********************************************************************
    //## Set*Array
    //##
    //## Sets a client-side vertex array.
    //## **********************************************************************
    void SetPositionArray(float* pPosition);
    void SetTexCoordArray(float* pTexCoord);
    void SetNormalArray(float* pNormal);

    //## **********************************************************************
    //## GetVertexCount
    //##
    //## Returns number of vertices to be rendered.
    //## 
    //## Returns:
    //##   int - number of vertices in static mesh.
    //## **********************************************************************
    int GetVertexCount();

    //## **********************************************************************
    //## get*Array
    //##
    //## Returns pointer to specified vertex array.
    //## **********************************************************************
    float* GetPositionArray();
    float* GetTexCoordArray();
    float* GetNormalArray();

    //## **********************************************************************
    //## SetRenderState
    //##
    //## Sets up the OpenGL state to properly draw the static mesh by binding
    //## vertex buffer object(s) and specifying vertex array properties.
    //##
    //## Input:
    //##   hProg - handle to shader program.
    //## **********************************************************************
    void SetRenderState(unsigned int hProg);

private:
    
    //## Number of vertices to render
    int m_nVertexCount;

    //## Handle to vertex buffer object holding vertex data.
    unsigned int m_unVBO;

    //## Client arrays
    float* m_pClientPosition;
    float* m_pClientTexCoord;
    float* m_pClientNormal;
};

#endif