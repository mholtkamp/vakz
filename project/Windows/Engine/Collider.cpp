#include "Collider.h"

#define DEFAULT_COLLIDER_COLOR_R 0.2f
#define DEFAULT_COLLIDER_COLOR_G 0.8f
#define DEFAULT_COLLIDER_COLOR_B 0.25f
#define DEFAULT_COLLIDER_COLOR_A 0.3f

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

    m_arRenderColor[0] = DEFAULT_COLLIDER_COLOR_R;
    m_arRenderColor[1] = DEFAULT_COLLIDER_COLOR_G;
    m_arRenderColor[2] = DEFAULT_COLLIDER_COLOR_B;
    m_arRenderColor[3] = DEFAULT_COLLIDER_COLOR_A;
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

void Collider::SetRotation(float fRotX,
                           float fRotY,
                           float fRotZ)
{
    m_fRotX = fRotX;
    m_fRotY = fRotY;
    m_fRotZ = fRotZ;
}
