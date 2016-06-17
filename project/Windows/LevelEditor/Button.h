#ifndef BUTTON_H
#define BUTTON_H

#include "Rect.h"
#include "Texture.h"
#include "Quad.h"
#include "Text.h"
#include "Scene.h"

class Button
{

public:

    Button();
    ~Button();

    int IsTouched();

    int IsPointerHovering();

    void SetRect(float fX,
                 float fY,
                 float fWidth,
                 float fHeight);

    void SetTextScale(float fScaleX,
                      float fScaleY);

    void SetTextOffsetX(float fOffsetX);

    void SetTextOffsetY(float fOffsetY);

    void SetTexture(Texture* pTexture);

    void SetTextString(const char* pText);

    void AddToScene(Scene& scene);

    void SetVisible(int nVisible);

    int IsVisible();

    void EnableBorder(int nEnable);

    void SetBackgroundColor(float fRed,
                            float fGreen,
                            float fBlue,
                            float fAlpha);

    void SetTextColor(float fRed,
                      float fGreen,
                      float fBlue,
                      float fAlpha);

    void SetBorderColor(float fRed,
                        float fGreen,
                        float fBlue,
                        float fAlpha);

    void SetTextString_Int(int nInt);

    void SetTextString_Float(float fFloat);

private:

    Rect m_rect;
    Quad m_quad;
    Text m_text;

    float m_fTextOffsetX;
    float m_fTextOffsetY;
    int m_nVisible;
};

#endif