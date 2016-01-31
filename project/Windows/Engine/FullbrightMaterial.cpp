#include "FullbrightMaterial.h"
#include "VGL.h"

//*****************************************************************************
// Constructor
//*****************************************************************************
FullbrightMaterial::FullbrightMaterial()
{
    m_nType = MATERIAL_FULLBRIGHT;

    m_arColor[0] = 1.0f;
    m_arColor[1] = 1.0f;
    m_arColor[2] = 1.0f;
    m_arColor[3] = 1.0f;
}

//*****************************************************************************
// Destructor
//*****************************************************************************
FullbrightMaterial::~FullbrightMaterial()
{
    
}

//*****************************************************************************
// SetRenderState
//*****************************************************************************
void FullbrightMaterial::SetRenderState(void        *pScene,
                                        unsigned int hProg)
{
    int hColor = glGetUniformLocation(hProg, "uColor");
    glUniform4fv(hColor, 1, m_arColor);
}

//*****************************************************************************
// SetColor
//*****************************************************************************
void FullbrightMaterial::SetColor(float fRed,
                                  float fGreen,
                                  float fBlue,
                                  float fAlpha)
{
    m_arColor[0] = fRed;
    m_arColor[1] = fGreen;
    m_arColor[2] = fBlue;
    m_arColor[3] = fAlpha;
}
