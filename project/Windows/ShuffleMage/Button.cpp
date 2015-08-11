#include "Button.h"
#include "VInput.h"

#define DEFAULT_X_OFFSET 0.01f
#define DEFAULT_Y_OFFSET 0.01f

Button::Button()
{
    m_quad.SetVisible(0);
    m_text.SetVisible(0);
    m_quad.SetColor(0.7f, 0.2f, 0.22f, 1.0f);
    m_fTextOffsetX = DEFAULT_X_OFFSET;
    m_fTextOffsetY = DEFAULT_Y_OFFSET;
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

    m_text.SetPosition(fX + m_fTextOffsetX,
                       fY + m_fTextOffsetY);

    m_quad.SetBox(fX, fY, fWidth, fHeight);
}

void Button::SetTexture(Texture* pTexture)
{
    m_quad.SetTexture(pTexture);
}

void Button::SetTextString(const char* pText)
{
    m_text.SetText(pText);
}

void Button::AddToScene(Scene& scene)
{
    scene.AddGlyph(&m_quad);
    scene.AddGlyph(&m_text);
}

void Button::SetVisible(int nVisible)
{
    m_quad.SetVisible(nVisible);
    m_text.SetVisible(nVisible);
}
