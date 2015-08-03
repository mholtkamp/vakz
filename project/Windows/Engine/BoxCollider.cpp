#include "BoxCollider.h"

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
    BoxCollider* pBox = 0;

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
