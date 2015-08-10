#include "Button.h"
#include "VInput.h"
Button::Button()
{

}

Button::~Button()
{
    
}

int Button::IsTouched()
{
    float fTouchX;
    float fTouchY;

    GetTouchPositionNormalized(fTouchX,
                               fTouchY,
                               0);

    return IsPointerDown(0) && m_rect.Contains(fTouchX, fTouchY);
}

void Button::SetRectangle(float fX,
                          float fY,
                          float fWidth,
                          float fHeight)
{
     m_rect.m_fX      = fX;
     m_rect.m_fY      = fY;
     m_rect.m_fWidth  = fWidth;
     m_rect.m_fHeight = fHeight;
}

void Button::SetTexture(Texture* pTexture)
{
    m_quad.SetTexture(pTexture);
}
