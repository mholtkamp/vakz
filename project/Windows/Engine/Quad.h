#ifndef QUAD_H
#define QUAD_H

#include "Glyph.h"
#include "Texture.h"

class Quad : public Glyph
{

public:

    //## **********************************************************************
    //## Constructor
    //## **********************************************************************
    Quad();

    //## **********************************************************************
    //## Destructor
    //## **********************************************************************
    virtual ~Quad();

    //## **********************************************************************
    //## Render
    //##
    //## Renders the quad to the screen. 
    //## **********************************************************************
    virtual void Render();

    //## **********************************************************************
    //## SetPosition [OVERRIDE]
    //##
    //## Sets the position of the quad.
    //## 
    //## Input:
    //##   fX - the x position in screen coordinates.
    //##   fY - the y position in screen coordinates.
    //## **********************************************************************
    virtual void SetPosition(float fX,
                             float fY);

    //## **********************************************************************
    //## SetWidth
    //##
    //## Sets the width of the quad (specified in screen space).
    //## 
    //## Input:
    //##   fWidth - new width of quad
    //## **********************************************************************
    virtual void SetWidth(float fWidth);

    //## **********************************************************************
    //## SetHeight
    //##
    //## Sets the height of the quad (specificed in screen space).
    //## 
    //## Input:
    //##   fHeight - new height of quad
    //## **********************************************************************
    virtual void SetHeight(float fHeight);

    //## **********************************************************************
    //## SetDimensions
    //##
    //## Sets both the height and width of quad.
    //##
    //## Input:
    //##   fWidth  - new width of quad
    //##   fHeight - new height of quad
    //## **********************************************************************
    virtual void SetDimensions(float fWidth,
                               float fHeight);

    //## **********************************************************************
    //## SetBox
    //##
    //## Sets all of the spatial qualities of the quad.
    //##
    //## Input:
    //##   fX      - x coordinate.
    //##   fY      - y coordinate.
    //##   fWidth  - width
    //##   fHeight - height
    //## **********************************************************************
    virtual void SetBox(float fX,
                        float fY,
                        float fWidth,
                        float fHeight);

    //## **********************************************************************
    //## SetTiling
    //##
    //## Sets the tiling of the quad, indicating how many times the texture
    //## should be rendered on the quad. Tiling does nothing when no texture
    //## is assigned to the quad.
    //##
    //## Input:
    //##   fTileS - the s tiling of the texture.
    //##   fTileT - the t tiling of the texture.
    //## **********************************************************************
    virtual void SetTiling(float fTileS,
                           float fTileT);
    //## **********************************************************************
    //## SetTexture
    //##
    //## Sets the texture of the quad. If no texture is supplied, then the
    //## quad will be rendered with uniform color.
    //##
    //## Input:
    //##   pTexture - pointer to texture object.
    //## **********************************************************************
    virtual void SetTexture(Texture* pTexture);

    //## **********************************************************************
    //## SetBorderColor
    //##
    //## Sets the color of the border around the quad. The border is rendered
    //## only if enabled for this individual quad.
    //##
    //## Input:
    //##   fRed   - red component of border color.
    //##   fGreen - green component of border color.
    //##   fBlue  - blue component of border color.
    //##   fAlpha - alpha component of border color.
    //## **********************************************************************
    void SetBorderColor(float fRed,
                        float fGreen,
                        float fBlue,
                        float fAlpha);

    //## **********************************************************************
    //## SetBorderWidth
    //##
    //## Sets the width of the border in pixels. Border must be enabled in 
    //## order to be rendered.
    //##
    //## Input:
    //##   fWidth - width of the border in pixels.
    //## **********************************************************************
    void SetBorderWidth (float fWidth);

    //## **********************************************************************
    //## EnableBorder
    //## 
    //## Enables a border around the quad.
    //## 
    //## Input:
    //##   nEnable - flag to enable border rendering.
    //## **********************************************************************
    void EnableBorder(int nEnable);

    enum QuadEnum
    {
        POSITION_ARRAY_SIZE = 8,
        TEXCOORD_ARRAY_SIZE = 8
    };

protected:

    //## Calculates position array to send to GPU
    //## based on position and dimensions.
    virtual void GeneratePositionArray();

    //## Calculate the texture coordinate array to
    //## send to the GPU based on tiling.
    virtual void GenerateTexCoordArray();

    //## Texture object to sample color from
    Texture* m_pTexture;
    
    //## Dimensions of quad.
    float m_fWidth;
    float m_fHeight;

    //## Tiling of texture coordinates. Default = 1.0f
    float m_fTileS;
    float m_fTileT;

    //## Vertex arrays 
    float m_arPosition[POSITION_ARRAY_SIZE];
    float m_arTexCoord[TEXCOORD_ARRAY_SIZE];

    //## Border enable
    int m_nBorderEnable;

    //## Border color
    float m_arBorderColor[4];

    //## Border line width
    float m_fBorderWidth;
};

#endif
