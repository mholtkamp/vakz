#ifndef MESH_COLLIDER_H
#define MESH_COLLIDER_H

#include "Collider.h"
#include "StaticMesh.h"

class MeshCollider : public Collider
{

public:

    //## **********************************************************************
    //## Constructor
    //## **********************************************************************
    MeshCollider();

    //## **********************************************************************
    //## Destructor
    //## **********************************************************************
    ~MeshCollider();

    //## **********************************************************************
    //## AssignMesh
    //##
    //## Assign a static mesh geometry to this collider. The collider will
    //## keep a pointer that points to the StaticMesh's position array.
    //##
    //## Input:
    //##   pStaticMesh - pointer to the static mesh object.
    //## **********************************************************************
    void AssignMesh(StaticMesh* pStaticMesh);

    //## **********************************************************************
    //## Render
    //##
    //## Renders the collider for debugging purposes. The MVP matrix provided
    //## should be the same as the matrix used to render the parent Matter. 
    //##
    //## Input:
    //##   pMVP - model-view-projection matrix
    //## **********************************************************************
    void Render(Matrix* pMVP);

    //## **********************************************************************
    //## Overlaps
    //## 
    //## Checks whether two colliders overlap.
    //##
    //## Input:
    //##   pOther - other collider to test.
    //##
    //## Returns:
    //##   int - '1' if the colliders overlap,
    //##         '0' otherwise.
    //## **********************************************************************
    int Overlaps(Collider* pOther,
                 void* pOtherMatter,
                 void* pThisMatter);

    //## **********************************************************************
    //## GetVertexCount
    //##
    //## Returns:
    //##    int - number of vertices in the collision mesh.
    //## **********************************************************************
    int GetVertexCount();

    //## **********************************************************************
    //## GetPositionArray
    //## 
    //## Returns:
    //##   float* - the pointer to the collision mesh's position array that 
    //##            belongs to a StaticMesh object.
    //## **********************************************************************
    float* GetPositionArray();

    //## **********************************************************************
    //## GetTriangle
    //##
    //## Retrieves a triangle at the index specified.
    //##
    //## Input:
    //##   nIndex - the index of the triangle.
    //##
    //## Output:
    //##   arTriangle - The triangle at index nIndex in the collision mesh.
    //## **********************************************************************
    void GetTriangle(int   nIndex,
                     float arTriangle[3][3]);

private:

    //## Pointer to triangle array
    float* m_arPosition;

    //## Number of vertices in the collision mesh
    int m_nVertices;
};

#endif
