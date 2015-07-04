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
                                      unsigned int hProg,
                                      int          nLightIndex)
{
    int hVector = glGetUniformLocation(hProg, "uDirLightVector");
    int hColor  = glGetUniformLocation(hProg, "uDirLightColor");

    glUniform3fv(hVector, 1, m_arDirection);
    glUniform4fv(hColor,  1, m_arColor);
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
