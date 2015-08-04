#ifndef MESH_COLLIDER_H
#define MESH_COLLIDER_H

#include "Collider.h"
#include "StaticMesh.h"

class MeshCollider : public Collider
{

public:

    MeshCollider();

    ~MeshCollider();

    void AssignMesh(StaticMesh* pStaticMesh);

    void Render(Matrix* pMVP);

    int Overlaps(Collider* pOther);

    int GetVertexCount();

    float* GetPositionArray();

    void GetTriangle(int   nIndex,
                     float arTriangle[3][3]);

private:

    float* m_arPosition;

    int m_nVertices;
};

#endif
