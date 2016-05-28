#include "ToonMaterial.h"
#include "VGL.h"

//*****************************************************************************
// Constructor
//*****************************************************************************
ToonMaterial::ToonMaterial()
{
    m_nType = MATERIAL_TOON;

    m_arColor[0] = 1.0f;
    m_arColor[1] = 1.0f;
    m_arColor[2] = 1.0f;
    m_arColor[3] = 1.0f;

    m_nIntervals = 3;
}

//*****************************************************************************
// Destructor
//*****************************************************************************
ToonMaterial::~ToonMaterial()
{

}

//*****************************************************************************
// SetRenderState
//*****************************************************************************
void ToonMaterial::SetRenderState(void*        pScene,
                                  unsigned int hProg)
{
    int hColor = glGetUniformLocation(hProg, "uDiffuseColor");
    int hIntervals = glGetUniformLocation(hProg, "uLightIntervals");
    glUniform4fv(hColor, 1, m_arColor);
    glUniform1i(hIntervals, m_nIntervals);
}

//*****************************************************************************
// SetColor
//*****************************************************************************
void ToonMaterial::SetColor(float fRed,
                               float fGreen,
                               float fBlue,
                               float fAlpha)
{
    m_arColor[0] = fRed;
    m_arColor[1] = fGreen;
    m_arColor[2] = fBlue;
    m_arColor[3] = fAlpha;
}

//*****************************************************************************
// SetLightingIntervalCount
//*****************************************************************************
void ToonMaterial::SetLightingIntervalCount(int nIntervals)
{
    m_nIntervals = nIntervals;
}