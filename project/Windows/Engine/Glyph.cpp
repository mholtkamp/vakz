#include "Glyph.h"
#include <string.h>

//*****************************************************************************
// Constructor
//*****************************************************************************
Glyph::Glyph()
{
    m_fX = 0.0f;
    m_fY = 0.0f;

    // Default glyph color
    m_arColor[0] = 1.0f;
    m_arColor[1] = 1.0f;
    m_arColor[2] = 1.0f;
    m_arColor[3] = 1.0f;

    m_nVisible = 1;
}

//*****************************************************************************
// Destructor
//*****************************************************************************
Glyph::~Glyph()
{

}

//*****************************************************************************
// SetColor
//*****************************************************************************
void Glyph::SetColor(float fRed,
                     float fGreen,
                     float fBlue,
                     float fAlpha)
{
    m_arColor[0] = fRed;
    m_arColor[1] = fGreen;
    m_arColor[2] = fBlue;
    m_arColor[3] = fAlpha;
}

void Glyph::SetColor(const float* arColor)
{
    memcpy(m_arColor, arColor, sizeof(float) * 4);
}

//*****************************************************************************
// SetPosition
//*****************************************************************************
void Glyph::SetPosition(float fX,
                        float fY)
{
    m_fX = fX;
    m_fY = fY;
}

//*****************************************************************************
// SetVisible
//*****************************************************************************
void Glyph::SetVisible(int nVisible)
{
    m_nVisible = nVisible;
}
