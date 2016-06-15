#ifndef GLYPH_H
#define GLYPH_H

#include "Actor.h"

class Glyph
{

public:

    //## **********************************************************************
    //## Constructor
    //## **********************************************************************
    Glyph();
    
    //## **********************************************************************
    //## Destructor
    //## **********************************************************************
    virtual ~Glyph();

    //## **********************************************************************
    //## Render
    //## 
    //## Renders the glyph object to screen.
    //## **********************************************************************
    virtual void Render() = 0;

    //## **********************************************************************
    //## SetColor
    //##
    //## Sets the color of the Glyph. Color can be used differently be each
    //## subclass of Glyph, but generally, if the glyph draws a texture, then
    //## the color that is set with this function will be used to modulate
    //## the texture color (multiplying color and texture color).
    //##
    //## All input parameters should be given in normalized floating point
    //## format (between 0.0f and 1.0f, inclusive).
    //##
    //## Input:
    //##   fRed   - red component
    //##   fGreen - green component
    //##   fBlue  - blue component 
    //##   fAlpha - alpha component
    //## **********************************************************************
    virtual void SetColor(float fRed,
                          float fGreen,
                          float fBlue,
                          float fAlpha);

    virtual void SetColor(const float* arColor);

    //## **********************************************************************
    //## SetPosition
    //##
    //## Sets the position of the glyph.
    //## 
    //## Input:
    //##   fX - the x position in screen coordinates.
    //##   fY - the y position in screen coordinates.
    //## **********************************************************************
    virtual void SetPosition(float fX,
                             float fY);

    //## **********************************************************************
    //## SetVisible
    //##
    //## Sets the visiblity of the glyph. Glyphs that are not visible will not
    //## attempt to be rendered. Glyphs are visible by default.
    //## 
    //## Input:
    //##   nVisible - 0 to hide glyph. 1 to make visible.
    //## **********************************************************************
    void SetVisible(int nVisible);

protected:

    //## Position of glyph in 2D space.
    float m_fX;
    float m_fY;

    //## Color of glyph (4 values, R/G/B/A)
    float m_arColor[4];

    //## Flag to indicate whether glyph should be rendered.
    int m_nVisible;
};

#endif
