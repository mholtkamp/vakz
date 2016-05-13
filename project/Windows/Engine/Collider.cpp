#include "Collider.h"

#define DEFAULT_COLLIDER_COLOR_R 0.2f
#define DEFAULT_COLLIDER_COLOR_G 0.8f
#define DEFAULT_COLLIDER_COLOR_B 0.25f
#define DEFAULT_COLLIDER_COLOR_A 0.3f

//*****************************************************************************
// Constructor
//*****************************************************************************
Collider::Collider()
{
    m_nType = COLLIDER_NO_TYPE;

    m_arPosition[0] = 0.0f;
    m_arPosition[1] = 0.0f;
    m_arPosition[2] = 0.0f;

    m_arRenderColor[0] = DEFAULT_COLLIDER_COLOR_R;
    m_arRenderColor[1] = DEFAULT_COLLIDER_COLOR_G;
    m_arRenderColor[2] = DEFAULT_COLLIDER_COLOR_B;
    m_arRenderColor[3] = DEFAULT_COLLIDER_COLOR_A;
}

//*****************************************************************************
// Destructor
//*****************************************************************************
Collider::~Collider()
{

}

//*****************************************************************************
// GetType
//*****************************************************************************
int Collider::GetType()
{
    return m_nType;
}

//*****************************************************************************
// SetPosition
//*****************************************************************************
void Collider::SetRelativePosition(float fX,
                                   float fY,
                                   float fZ)
{
    m_arPosition[0] = fX;
    m_arPosition[1] = fY;
    m_arPosition[2] = fZ;
}

//*****************************************************************************
// SetColor
//*****************************************************************************
void Collider::SetColor(float fRed,
                        float fGreen,
                        float fBlue,
                        float fAlpha)
{
    m_arRenderColor[0] = fRed;
    m_arRenderColor[1] = fGreen;
    m_arRenderColor[2] = fBlue;
    m_arRenderColor[3] = fAlpha;
}
