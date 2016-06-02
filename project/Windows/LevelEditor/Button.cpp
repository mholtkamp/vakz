#include "Button.h"
#include "VInput.h"

#define DEFAULT_X_OFFSET 0.008f
#define DEFAULT_Y_OFFSET 0.01f

Button::Button()
{
    m_quad.SetVisible(1);
    m_text.SetVisible(1);
    m_quad.SetColor(0.7f, 0.2f, 0.22f, 1.0f);
    m_quad.EnableBorder(1);
    m_quad.SetBorderColor(1.0f, 1.0f, 1.0f, 1.0f);
    m_quad.SetBorderWidth(2.0f);
    m_text.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    m_fTextOffsetX = DEFAULT_X_OFFSET;
    m_fTextOffsetY = DEFAULT_Y_OFFSET;

    m_text.SetScale(0.4f, 0.7f);
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

int Button::IsPointerHovering()
{
    float fTouchX;
    float fTouchY;

    GetTouchPositionNormalized(fTouchX,
                               fTouchY,
                               0);

    return m_rect.Contains(fTouchX, fTouchY);
}

void Button::SetRect(float fX,
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

void Button::SetTextScale(float fScaleX,
                          float fScaleY)
{
    m_text.SetScale(fScaleX, fScaleY);
}

void Button::SetTextOffsetX(float fOffsetX)
{
    m_fTextOffsetX = fOffsetX;

    m_text.SetPosition(m_rect.m_fX + m_fTextOffsetX,
                       m_rect.m_fY + m_fTextOffsetY);
}

void Button::SetTextOffsetY(float fOffsetY)
{
    m_fTextOffsetY = fOffsetY;

    m_text.SetPosition(m_rect.m_fX + m_fTextOffsetX,
                       m_rect.m_fY + m_fTextOffsetY);
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

void Button::EnableBorder(int nEnable)
{
    m_quad.EnableBorder(nEnable);
}

void Button::SetBackgroundColor(float fRed,
                                float fGreen,
                                float fBlue,
                                float fAlpha)
{
    m_quad.SetColor(fRed, fGreen, fBlue, fAlpha);
}

void Button::SetTextColor(float fRed,
                          float fGreen,
                          float fBlue,
                          float fAlpha)
{
    m_text.SetColor(fRed, fGreen, fBlue, fAlpha);
}

void Button::SetBorderColor(float fRed,
                            float fGreen,
                            float fBlue,
                            float fAlpha)
{
    m_quad.SetBorderColor(fRed, fGreen, fBlue, fAlpha);
}
