#include "Collider.h"

Collider::Collider()
{
    m_nType = COLLIDER_NO_TYPE;

    m_fX = 0.0f;
    m_fY = 0.0f;
    m_fZ = 0.0f;
    m_fScaleX = 0.0f;
    m_fScaleY = 0.0f;
    m_fScaleZ = 0.0f;
    m_fRotX = 0.0f;
    m_fRotY = 0.0f;
    m_fRotZ = 0.0f;
}

Collider::~Collider()
{

}

int Collider::GetType()
{
    return m_nType;
}

void Collider::SetPosition(float fX,
                           float fY,
                           float fZ)
{
    m_fX = fX;
    m_fY = fY;
    m_fZ = fZ;
}

void Collider::SetScale(float fScaleX,
                        float fScaleY,
                        float fScaleZ)
{
    m_fScaleX = fScaleX;
    m_fScaleY = fScaleY;
    m_fScaleZ = fScaleZ;
}

void Collider::SetPosition(float fRotX,
                           float fRotY,
                           float fRotZ)
{
    m_fRotX = fRotX;
    m_fRotY = fRotY;
    m_fRotZ = fRotZ;
}
