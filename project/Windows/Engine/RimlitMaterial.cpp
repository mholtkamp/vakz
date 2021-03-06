#include "RimlitMaterial.h"
#include "VGL.h"
#include "Scene.h"

//*****************************************************************************
// Constructor
//*****************************************************************************
RimlitMaterial::RimlitMaterial()
{
    m_nType = MATERIAL_RIMLIT;

    m_arRimColor[0] = 1.0f;
    m_arRimColor[1] = 0.0f;
    m_arRimColor[2] = 0.0f;
    m_arRimColor[3] = 1.0f;

    m_fRimSize  = 0.2f;
    m_nRimStyle = RIM_STYLE_SOFT;
}

//*****************************************************************************
// Destructor
//*****************************************************************************
RimlitMaterial::~RimlitMaterial()
{

}

//*****************************************************************************
// SetRenderState
//*****************************************************************************
void RimlitMaterial::SetRenderState(void*        pScene,
                                    unsigned int hProg)
{

    float arViewVec[3] = {0.0f};

    // Fetch uniform locations
    int hDiffuseColor = glGetUniformLocation(hProg, "uDiffuseColor");
    int hRimColor     = glGetUniformLocation(hProg, "uRimColor");
    int hViewVector   = glGetUniformLocation(hProg, "uViewVector");
    int hRimSize      = glGetUniformLocation(hProg, "uRimSize");
    int hRimStyle     = glGetUniformLocation(hProg, "uRimStyle");


    // Set uniforms
    glUniform4fv(hDiffuseColor, 1, m_arColor);
    glUniform4fv(hRimColor, 1, m_arRimColor);

    reinterpret_cast<Scene*>(pScene)->GetCamera()->GetViewVector(arViewVec);
    glUniform3fv(hViewVector, 1, arViewVec);

    glUniform1f(hRimSize,  m_fRimSize);
    glUniform1i(hRimStyle, m_nRimStyle);
}

//*****************************************************************************
// SetRimColor
//*****************************************************************************
void RimlitMaterial::SetRimColor(float arRim[4])
{
    m_arRimColor[0] = arRim[0];
    m_arRimColor[1] = arRim[1];
    m_arRimColor[2] = arRim[2];
    m_arRimColor[3] = arRim[3];
}

//*****************************************************************************
// SetRimSize
//*****************************************************************************
void RimlitMaterial::SetRimSize(float fSize)
{
    m_fRimSize = fSize;
}

//*****************************************************************************
// SetRimStyle
//*****************************************************************************
void RimlitMaterial::SetRimStyle(int nStyle)
{
    m_nRimStyle = nStyle;
}

int RimlitMaterial::GetRimStyle()
{
    return m_nRimStyle;
}

float RimlitMaterial::GetRimSize()
{
    return m_fRimSize;
}

float* RimlitMaterial::GetRimColor()
{
    return m_arRimColor;
}
