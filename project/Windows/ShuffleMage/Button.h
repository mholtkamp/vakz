#ifndef SM_BUTTON_H
#define SM_BUTTON_H

#include "Rectangle.h"
#include "Texture.h"
#include "Quad.h"

class Button
{

public:

    Button();
    ~Button();

    int IsTouched();

    void SetRectangle(float fX,
                      float fY,
                      float fWidth,
                      float fHeight);

    void SetTexture(Texture* pTexture);

private:

    Rectangle m_rect;
    Quad m_quad;

};


#endif