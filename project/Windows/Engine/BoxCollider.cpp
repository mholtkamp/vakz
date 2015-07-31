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
}

BoxCollider::~BoxCollider()
{

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

void BoxCollider::Render()
{

}

int BoxCollider::Overlaps(Collider* pOther)
{
    if (pOther->GetType() == COLLIDER_BOX)
    {
        return 0;
    }
    else if (pOther->GetType() == COLLIDER_MESH)
    {
        return 0;
    }
}

float BoxCollider::GetMinX()
{
    return m_fMinX + m_fX;
}

float BoxCollider::GetMaxX()
{
    return m_fMaxX + m_fX;
}

float BoxCollider::GetMinY()
{
    return m_fMinY + m_fY;
}

float BoxCollider::GetMaxY()
{
    return m_fMaxY + m_fY;
}

float BoxCollider::GetMinZ()
{
    return m_fMinZ + m_fZ;
}

float BoxCollider::GetMaxZ()
{
    return m_fMaxZ + m_fZ;
}
