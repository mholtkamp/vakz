#include "DiffuseMaterial.h"
#include "VGL.h"

DiffuseMaterial::DiffuseMaterial()
{
    m_nType = MATERIAL_DIFFUSE;

    m_arColor[0] = 0.0f;
    m_arColor[1] = 0.0f;
    m_arColor[2] = 0.0f;
    m_arColor[3] = 0.0f;
}

DiffuseMaterial::~DiffuseMaterial()
{

}

void DiffuseMaterial::SetRenderState(unsigned int hProg)
{
    int hColor = glGetUniformLocation(hProg, "uColor");
    glUniform4fv(hColor, 1, m_arColor);
}

void DiffuseMaterial::SetColor(float fRed,
                               float fGreen,
                               float fBlue,
                               float fAlpha)
{
    m_arColor[0] = fRed;
    m_arColor[1] = fGreen;
    m_arColor[2] = fBlue;
    m_arColor[3] = fAlpha;
}