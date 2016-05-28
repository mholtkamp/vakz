#include "DirectionalLight.h"
#include "VGL.h"

//*****************************************************************************
// Constructor
//*****************************************************************************
DirectionalLight::DirectionalLight()
{
    // Overwrite parent constructor with correct type.
    m_nType = LIGHT_DIRECTIONAL;

    // Set initial light direction to straight down
    m_arDirection[0] =  0.0f;
    m_arDirection[1] = -1.0f;
    m_arDirection[2] =  0.0f;

    m_fIntensity = 1.0f;
}

//*****************************************************************************
// Destructor
//*****************************************************************************
DirectionalLight::~DirectionalLight()
{

}

//*****************************************************************************
// SetRenderState
//*****************************************************************************
void DirectionalLight::SetRenderState(void*        pScene,
                                      unsigned int hProg)
{
    int hVector = glGetUniformLocation(hProg, "uDirLightVector");
    int hColor  = glGetUniformLocation(hProg, "uDirLightColor");
    int hIntensity = glGetUniformLocation(hProg, "uDirLightIntensity");

    glUniform3fv(hVector, 1, m_arDirection);
    glUniform3fv(hColor,  1, m_arColor);
    glUniform1f(hIntensity, m_fIntensity);
}

//*****************************************************************************
// SetDirectionVector
//*****************************************************************************
void DirectionalLight::SetDirectionVector(float fX,
                                          float fY,
                                          float fZ)
{
    m_arDirection[0] = fX;
    m_arDirection[1] = fY;
    m_arDirection[2] = fZ;
}

void DirectionalLight::SetIntensity(float fIntensity)
{
    m_fIntensity = fIntensity;
}
