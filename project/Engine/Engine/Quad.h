#ifndef QUAD_H
#define QUAD_H

#include "Glyph.h"

class Quad : Glyph
{

public:

    //## **********************************************************************
    //## Constructor
    //## **********************************************************************
    Quad();

    //## **********************************************************************
    //## Destructor
    //## **********************************************************************
    ~Quad();

    //## **********************************************************************
    //##  Render
    //##
    //##
    //## **********************************************************************
    virtual void Render();

    //## **********************************************************************
    //## SetWidth
    //##
    //##
    //## **********************************************************************
    virtual void SetWidth(float fWidth);

    //## **********************************************************************
    //## SetHeight
    //##
    //##
    //## **********************************************************************
    virtual void SetHeight(float fHeight);

    //## **********************************************************************
    //## SetDimensions
    //##
    //##
    //## **********************************************************************
    virtual void SetDimensions(float fWidth,
                               float fHeight);

    //## **********************************************************************
    //## SetBox
    //##
    //##
    //## **********************************************************************
    virtual void SetBox(float fX,
                        float fY,
                        float fWidth,
                        float fHeight);

    enum QuadEnum
    {
        POSITION_ARRAY_SIZE = 8,
        TEXCOORD_ARRAY_SIZE = 8
    };

protected:

    //## ID of texture to render.
    unsigned int m_unTexture;
    
    //## Dimensions of quad.
    float m_fWidth;
    float m_fHeight;

    //## Vertex arrays 
    float m_arPosition[POSITION_ARRAY_SIZE];
    float m_arTexCoord[TEXCOORD_ARRAY_SIZE];

};

#endif
