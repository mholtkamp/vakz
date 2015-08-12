#ifndef SM_BUTTON_H
#define SM_BUTTON_H

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

    void SetRect(float fX,
                 float fY,
                 float fWidth,
                 float fHeight);

    void SetTexture(Texture* pTexture);

    void SetTextString(const char* pText);

    void AddToScene(Scene& scene);

    void SetVisible(int nVisible);

private:

    Rect m_rect;
    Quad m_quad;
    Text m_text;

    float m_fTextOffsetX;
    float m_fTextOffsetY;
};


#endif