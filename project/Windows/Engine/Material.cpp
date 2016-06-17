#include "Material.h"

//*****************************************************************************
// Constructor
//*****************************************************************************
Material::Material()
{
    m_nType = MATERIAL_NO_TYPE;

    m_arColor[0] = 1.0f;
    m_arColor[1] = 1.0f;
    m_arColor[2] = 1.0f;
    m_arColor[3] = 1.0f;
}

//*****************************************************************************
// Destructor
//*****************************************************************************
Material::~Material()
{

}

//*****************************************************************************
// GetType
//*****************************************************************************
int Material::GetType()
{
    return m_nType;
}

//*****************************************************************************
// SetColor
//*****************************************************************************
void Material::SetColor(float fRed,
                               float fGreen,
                               float fBlue,
                               float fAlpha)
{
    m_arColor[0] = fRed;
    m_arColor[1] = fGreen;
    m_arColor[2] = fBlue;
    m_arColor[3] = fAlpha;
}

void Material::SetColor(float* arColor)
{
    m_arColor[0] = arColor[0];
    m_arColor[1] = arColor[1];
    m_arColor[2] = arColor[2];
    m_arColor[3] = arColor[3];
}

float* Material::GetColor()
{
    return m_arColor;
}
