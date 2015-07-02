#include "Light.h"

Light::Light()
{
    m_nEnable = 1;
    m_fIntensity = 1.0f;
    m_arColor[0] = 1.0f;
    m_arColor[1] = 1.0f;
    m_arColor[2] = 1.0f;
    m_arColor[3] = 1.0f;
}

Light::~Light()
{

}

void Light::SetEnable(int nEnable)
{
    m_nEnable = nEnable;
}

int Light::IsEnabled()
{
    return m_nEnable;
}

float Light::GetIntensity()
{
    return m_fIntensity;
}

void Light::SetIntensity(float fIntensity)
{
    m_fIntensity = fIntensity;
}

void Light::SetColor(float fRed,
                     float fGreen,
                     float fBlue,
                     float fAlpha)
{
    m_arColor[0] = fRed;
    m_arColor[1] = fGreen;
    m_arColor[2] = fBlue;
    m_arColor[3] = fAlpha;
}
