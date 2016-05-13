#include "BoxCollider.h"
#include "MeshCollider.h"
#include "TriangleBoxCollision.h"
#include "Matter.h"


//*****************************************************************************
// Constructor
//*****************************************************************************
BoxCollider::BoxCollider()
{
    m_nType = COLLIDER_BOX;
    m_arHalfExtents[0] = 1.0f;
    m_arHalfExtents[1] = 1.0f;
    m_arHalfExtents[2] = 1.0f;

    m_nRender    = 0;
}

//*****************************************************************************
// Destructor
//*****************************************************************************
BoxCollider::~BoxCollider()
{

}

//*****************************************************************************
// SetHalfExtents
//*****************************************************************************
void BoxCollider::SetHalfExtents(float fHalfX,
                                 float fHalfY,
                                 float fHalfZ)
{
    m_arHalfExtents[0] = fHalfX;
    m_arHalfExtents[1] = fHalfY;
    m_arHalfExtents[2] = fHalfZ;
}

//*****************************************************************************
// GetHalfExtents
//*****************************************************************************
const float* BoxCollider::GetHalfExtents()
{
    return m_arHalfExtents;
}

//*****************************************************************************
// GetRelativePosition
//*****************************************************************************
const float* BoxCollider::GetRelativePosition()
{
    return m_arPosition;
}

//*****************************************************************************
// Render
//*****************************************************************************
void BoxCollider::Render(Matrix* pMVP)
{
    if (m_nRender != 0)
    {

    }
}


//*****************************************************************************
// Overlaps
//*****************************************************************************
OverlapResult BoxCollider::Overlaps(Collider* pOther,
                                    void* pOtherMatter,
                                    void* pThisMatter)
{
    OverlapResult orResult;
    orResult.m_nOverlapping = 0;

    int i                    = 0;
    int nVert                = 0;
    int nTriangles           = 0;
    BoxCollider*  pBox       = 0;
    MeshCollider* pMesh      = 0;
    Matrix*       pMatrixM   = 0;
    Matter*       pOMatter   = 0;
    Matter*       pTMatter   = 0;
    float arTriangle[3][3]   = {0.0f};
    float arTransTri[3][3]   = {0.0f};

    const float* pTPosition            = 0;
    const float* pTScale               = 0;
    const float* pOPosition            = 0;
    const float* pOScale               = 0;
    const float* pOColliderPosition    = 0;
    const float* pOColliderHalfExtents = 0;

    if (pOther->GetType() == COLLIDER_BOX)
    {
        pBox     = reinterpret_cast<BoxCollider*>(pOther);
        pOMatter = reinterpret_cast<Matter*>(pOtherMatter);
        pTMatter = reinterpret_cast<Matter*>(pThisMatter);

        pTPosition            = pTMatter->GetPosition();
        pTScale               = pTMatter->GetScale();
        pOPosition            = pOMatter->GetPosition();
        pOScale               = pOMatter->GetScale();
        pOColliderPosition    = pBox->GetRelativePosition();
        pOColliderHalfExtents = pBox->GetHalfExtents();

        if (pOPosition[0] + pOScale[0]*(pOColliderPosition[0] - pOColliderHalfExtents[0]) >
            pTPosition[0] + pTScale[0]*(m_arPosition[0] + m_arHalfExtents[0]))
        {
            return orResult;
        }
        //if (pBox->GetMinX() > this->GetMaxX())
        //    return 0;

        if (pOPosition[1] + pOScale[1]*(pOColliderPosition[1] - pOColliderHalfExtents[1]) >
            pTPosition[1] + pTScale[1]*(m_arPosition[1] + m_arHalfExtents[1]))
        {
            return orResult;
        }
        //if (pBox->GetMinY() > this->GetMaxY())
        //    return 0;

        if (pOPosition[2] + pOScale[2]*(pOColliderPosition[2] - pOColliderHalfExtents[2]) >
            pTPosition[2] + pTScale[2]*(m_arPosition[2] + m_arHalfExtents[2]))
        {
            return orResult;
        }
        //if (pBox->GetMinZ() > this->GetMaxZ())
        //    return 0;

        if (pOPosition[0] + pOScale[0]*(pOColliderPosition[0] + pOColliderHalfExtents[0]) <
            pTPosition[0] + pTScale[0]*(m_arPosition[0] - m_arHalfExtents[0]))
        {
            return orResult;
        }
        //if (pBox->GetMaxX() < this->GetMinX())
        //    return 0;

        if (pOPosition[1] + pOScale[1]*(pOColliderPosition[1] + pOColliderHalfExtents[1]) <
            pTPosition[1] + pTScale[1]*(m_arPosition[1] - m_arHalfExtents[1]))
        {
            return orResult;
        }
        //if (pBox->GetMaxY() < this->GetMinY())
        //    return 0;

        if (pOPosition[2] + pOScale[2]*(pOColliderPosition[2] + pOColliderHalfExtents[2]) <
            pTPosition[2] + pTScale[2]*(m_arPosition[2] - m_arHalfExtents[2]))
        {
            return orResult;
        }
        //if (pBox->GetMaxZ() < this->GetMinZ())
        //    return 0;

        orResult.m_nOverlapping = 1;
        return orResult;
    }
    else if (pOther->GetType() == COLLIDER_MESH)
    {
        // We need a pointer to the owner Matter so that we can 
        // get the Matter's model matrix;
        if (pOtherMatter == 0)
        {
            return orResult;
        }

        pMatrixM = reinterpret_cast<Matter*>(pOtherMatter)->GetModelMatrix();
        pMesh    = reinterpret_cast<MeshCollider*>(pOther);
        pTPosition            = pTMatter->GetPosition();
        pTScale               = pTMatter->GetScale();

        float arBoxCenter[3] = {m_arPosition[0] * pTScale[0] + pTPosition[0],
                                m_arPosition[1] * pTScale[1] + pTPosition[1],
                                m_arPosition[2] * pTScale[2] + pTPosition[2]};

        float arBoxHalfSize[3] = {m_arHalfExtents[0] * pTScale[0],
                                  m_arHalfExtents[1] * pTScale[1],
                                  m_arHalfExtents[2] * pTScale[2]};

        nTriangles = pMesh->GetVertexCount()/3;

        for (i = 0; i < nTriangles; i++)
        {
            pMesh->GetTriangle(i, arTriangle);

            // Transform the points of the triangle
            for (nVert = 0; nVert < 3; nVert++)
            {
                pMatrixM->MultiplyVec3(arTriangle[nVert],
                                       arTransTri[nVert]);
            }

            if (triBoxOverlap(arBoxCenter,
                              arBoxHalfSize,
                              arTransTri) == 1)
            {
                orResult.m_nOverlapping = 1;
                return orResult;
            }
        }
        return orResult;
    }

    return orResult;
}

////*****************************************************************************
//// GetExtents
////*****************************************************************************
//float BoxCollider::GetMinX()
//{
//    return m_fMinX * m_arScale[0] + m_arPosition[0];
//}
//float BoxCollider::GetMaxX()
//{
//    return m_fMaxX * m_arScale[0] + m_arPosition[0];
//}
//float BoxCollider::GetMinY()
//{
//    return m_fMinY * m_arScale[1] + m_arPosition[1];
//}
//float BoxCollider::GetMaxY()
//{
//    return m_fMaxY * m_arScale[1] + m_arPosition[1];
//}
//float BoxCollider::GetMinZ()
//{
//    return m_fMinZ * m_arScale[2] + m_arPosition[2];
//}
//float BoxCollider::GetMaxZ()
//{
//    return m_fMaxZ * m_arScale[2] + m_arPosition[2];
//}
