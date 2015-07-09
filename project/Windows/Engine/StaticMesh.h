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
    //## Set*Array
    //##
    //## Sets a client-side vertex array.
    //## **********************************************************************
    void SetPositionArray(float* pPosition);
    void SetTexCoordArray(float* pTexCoord);
    void SetNormalArray(float* pNormal);

    //## **********************************************************************
    //## Get*Array
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
    //##   pScene - pointer to Scene that is currently rendering.
    //##   hProg  - handle to shader program.
    //## **********************************************************************
    void SetRenderState(void* pScene,
                        unsigned int hProg);

private:

    //## Handle to vertex buffer object holding vertex data.
    unsigned int m_unVBO;

    //## Client arrays
    float* m_pClientPosition;
    float* m_pClientTexCoord;
    float* m_pClientNormal;
};

#endif