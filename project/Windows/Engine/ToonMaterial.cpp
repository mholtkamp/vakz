#include "ToonMaterial.h"
#include "VGL.h"

//*****************************************************************************
// Constructor
//*****************************************************************************
ToonMaterial::ToonMaterial()
{
    m_nType = MATERIAL_TOON;

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
// SetLightingIntervalCount
//*****************************************************************************
void ToonMaterial::SetLightingIntervalCount(int nIntervals)
{
    m_nIntervals = nIntervals;
}

int ToonMaterial::GetLightingIntervalCount()
{
    return m_nIntervals;
}
