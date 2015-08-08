#include "BoxCollider.h"
#include "MeshCollider.h"
#include "TriangleBoxCollision.h"
#include "Matter.h"

BoxCollider::BoxCollider()
{
    m_nType = COLLIDER_BOX;
    m_fMinX = -1.0f;
    m_fMaxX =  1.0f;
    m_fMinY = -1.0f;
    m_fMaxY =  1.0f;
    m_fMinZ = -1.0f;
    m_fMaxZ =  1.0f;

    m_nRender    = 0;
    m_arPosition = 0;
}

BoxCollider::~BoxCollider()
{
    if (m_arPosition != 0)
    {
        delete [] m_arPosition;
        m_arPosition = 0;
    }
}

void BoxCollider::SetExtents(float fMinX,
                             float fMaxX,
                             float fMinY,
                             float fMaxY,
                             float fMinZ,
                             float fMaxZ)
{
    m_fMinX = fMinX;
    m_fMaxX = fMaxX;
    m_fMinY = fMinY;
    m_fMaxY = fMaxY;
    m_fMinZ = fMinZ;
    m_fMaxZ = fMaxZ;
}

void BoxCollider::Render(Matrix* pMVP)
{
    if (m_nRender != 0)
    {
        if (m_arPosition == 0)
        {

        } 
    }
}

int BoxCollider::Overlaps(Collider* pOther)
{
    int i                  = 0;
    int nVert              = 0;
    int nTriangles         = 0;
    BoxCollider*  pBox     = 0;
    MeshCollider* pMesh    = 0;
    Matrix*       pMatrixM = 0;
    float arTriangle[3][3] = {0.0f};
    float arTransTri[3][3] = {0.0f};

    if (pOther->GetType() == COLLIDER_BOX)
    {
        pBox = reinterpret_cast<BoxCollider*>(pOther);
        
        if (pBox->GetMinX() > this->GetMaxX())
            return 0;
        if (pBox->GetMinY() > this->GetMaxY())
            return 0;
        if (pBox->GetMinZ() > this->GetMaxZ())
            return 0;
        if (pBox->GetMaxX() < this->GetMinX())
            return 0;
        if (pBox->GetMaxY() < this->GetMinY())
            return 0;
        if (pBox->GetMaxZ() < this->GetMinZ())
            return 0;

        return 1;
    }
    else if (pOther->GetType() == COLLIDER_MESH)
    {
        // We need a pointer to the owner Matter so that we can 
        // get the Matter's model matrix;
        if (pOther->GetMatter() == 0)
        {
            return 0;
        }

        pMatrixM = reinterpret_cast<Matter*>(pOther->GetMatter())->GetModelMatrix();

        pMesh = reinterpret_cast<MeshCollider*>(pOther);

        float arBoxCenter[3] = {(GetMinX() + GetMaxX())/2.0f,
                                (GetMinY() + GetMaxY())/2.0f,
                                (GetMinZ() + GetMaxZ())/2.0f};

        float arBoxHalfSize[3] = {(GetMaxX() - GetMinX())/2.0f,
                                  (GetMaxY() - GetMinY())/2.0f,
                                  (GetMaxZ() - GetMinZ())/2.0f};

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
                return 1;
            }
        }
        return 0;
    }

    return 0;
}

float BoxCollider::GetMinX()
{
    return m_fMinX * m_fScaleX + m_fX;
}

float BoxCollider::GetMaxX()
{
    return m_fMaxX * m_fScaleX + m_fX;
}

float BoxCollider::GetMinY()
{
    return m_fMinY * m_fScaleY + m_fY;
}

float BoxCollider::GetMaxY()
{
    return m_fMaxY * m_fScaleY + m_fY;
}

float BoxCollider::GetMinZ()
{
    return m_fMinZ * m_fScaleZ + m_fZ;
}

float BoxCollider::GetMaxZ()
{
    return m_fMaxZ * m_fScaleZ + m_fZ;
}
