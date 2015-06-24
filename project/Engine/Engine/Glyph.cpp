#include "Glyph.h"

//*****************************************************************************
// Constructor
//*****************************************************************************
Glyph::Glyph()
{
    m_fX = 0.0f;
    m_fY = 0.0f;

    m_arColor[0] = 0.0f;
    m_arColor[1] = 0.0f;
    m_arColor[2] = 0.0f;
    m_arColor[3] = 0.0f;

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
