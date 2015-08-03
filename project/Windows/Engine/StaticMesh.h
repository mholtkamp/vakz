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

    void LoadGeometry(const char* pFile);

    //## **********************************************************************
    //## Set*Array
    //##
    //## Sets a client-side vertex array.
    //## **********************************************************************
    void SetPositionArray(float* pPosition);
    void SetTexCoordArray(float* pTexCoord);
    void SetNormalArray(float* pNormal);

    //## **********************************************************************
    //## GetPositionArray
    //##
    //## Returns pointer to specified main memory position array for
    //## non-primitives.
    //## **********************************************************************
    float* GetPositionArray();

    //## **********************************************************************
    //## SetRenderState
    //##
    //## Sets up the OpenGL state to properly draw the static mesh by binding
    //## vertex buffer object(s) and specifying vertex array properties.
    //##
    //## Input:
    //##   pScene - pointer to Scene that is currently rendering.
    //##   hProg  - handle to shader program.
    //##   nAnimationIndex - not used.
    //##   fCurrentFrame   - not used.
    //## **********************************************************************
    void SetRenderState(void*        pScene,
                        unsigned int hProg,
                        int          nAnimationIndex,
                        float        fCurrentFrame);

private:

    //## Handle to vertex buffer object holding vertex data.
    unsigned int m_unVBO;

    //## Client arrays
    float* m_pClientPosition;
    float* m_pClientTexCoord;
    float* m_pClientNormal;

    //## Position array stored in main memory.
    float* m_arPosition;
};

#endif