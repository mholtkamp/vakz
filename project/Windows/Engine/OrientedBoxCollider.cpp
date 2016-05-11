#include "OrientedBoxCollider.h"
#include "BoxCollider.h"
#include "MeshCollider.h"
#include "Matter.h"
#include "Log.h"

OrientedBoxCollider::OrientedBoxCollider()
{
    m_nType = COLLIDER_ORIENTED_BOX;
    m_arHalfExtents[0] = 1.0f;
    m_arHalfExtents[1] = 1.0f;
    m_arHalfExtents[2] = 1.0f;
    m_arRotation[0] = 0.0f;
    m_arRotation[1] = 0.0f;
    m_arRotation[2] = 0.0f;
}

OrientedBoxCollider::~OrientedBoxCollider()
{

}

void OrientedBoxCollider::Render(Matrix* pMVP)
{

}

int OrientedBoxCollider::Overlaps(Collider* pOther,
                                  void* pOtherMatter,
                                  void* pThisMatter)
{
    int i = 0;
    float arAVerts[VERTICES_PER_OBB * 3];
    float arBVerts[VERTICES_PER_OBB * 3];
    Matter* pTMatter = 0;
    Matter* pOMatter = 0;
    float arBoxVertices[VERTICES_PER_OBB * 3] = {0.0f};

    if (pOther->GetType() == COLLIDER_ORIENTED_BOX)
    {
        LogDebug("Performing SAT with two OBBs");
    }
    else
    {
        OrientedBoxCollider* pA = this;
        OrientedBoxCollider* pB = reinterpret_cast<OrientedBoxCollider*>(pOther);
        pTMatter = reinterpret_cast<Matter*>(pThisMatter);
        pOMatter = reinterpret_cast<Matter*>(pOtherMatter);

        // Find all the coordinates of box A (this box) in world space.
        GenerateLocalCoordinates(arBoxVertices);
        for(i = 0; i < VERTICES_PER_OBB; i++)
        {
            (*pTMatter->GetModelMatrix() * m_matRotation).MultiplyVec3(&arBoxVertices[i*3], arAVerts);
        }

        // Now find the second box's coordinates in world space.
        pB->GenerateLocalCoordinates(arBoxVertices);
        for(i = 0; i < VERTICES_PER_OBB; i++)
        {
            (*pOMatter->GetModelMatrix() * pB->m_matRotation).MultiplyVec3(&arBoxVertices[i*3], arBVerts);
        }
    }

    return 0;
}

void OrientedBoxCollider::SetHalfExtents(float fHalfX,
                                         float fHalfY,
                                         float fHalfZ)
{
    m_arHalfExtents[0] = fHalfX;
    m_arHalfExtents[1] = fHalfY;
    m_arHalfExtents[2] = fHalfZ;
}

void OrientedBoxCollider::SetRelativeRotation(float fRotX,
                                              float fRotY,
                                              float fRotZ)
{
    m_arRotation[0] = fRotX;
    m_arRotation[1] = fRotY;
    m_arRotation[2] = fRotZ;
    m_matRotation.Rotate(fRotX, 1.0f, 0.0f, 0.0f);
    m_matRotation.Rotate(fRotY, 0.0f, 1.0f, 0.0f);
    m_matRotation.Rotate(fRotZ, 0.0f, 0.0f, 1.0f);
}

void OrientedBoxCollider::GenerateLocalCoordinates(float* pRes)
{
    int i = 0;

    for (i = 0; i < VERTICES_PER_OBB; i++)
    {
        pRes[i*3 + 0] = m_arPosition[0] + (1 - (i & 1)*2)*m_arHalfExtents[0];
        pRes[i*3 + 1] = m_arPosition[1] + (1 - (i & 2)*2)*m_arHalfExtents[1];
        pRes[i*3 + 2] = m_arPosition[2] + (1 - (i & 4)*2)*m_arHalfExtents[2];
    }
}
