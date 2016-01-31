#ifndef FULLBRIGHT_MATERIAL_H
#define FULLBRIGHT_MATERIAL_H

#include "Material.h"

class FullbrightMaterial : public Material
{
public:

    //## **********************************************************************
    //## Constructor
    //## **********************************************************************
    FullbrightMaterial();

    //## **********************************************************************
    //## Constructor
    //## **********************************************************************
    ~FullbrightMaterial();

    //## **********************************************************************
    //## GetType
    //##
    //## Returns:
    //##   int - type of material.
    //## **********************************************************************
    int GetType();

    //## **********************************************************************
    //## SetRenderState
    //##
    //## Prepares the OpenGL state for rendering a matter with a fullbright
    //## material.
    //##
    //## Input:
    //##   pScene - pointer to scene being rendered.
    //##   hProg  - handle to shader that will be used for rendering matter.
    //## **********************************************************************
    void SetRenderState(void*        pScene,
                        unsigned int hProg);

    //## **********************************************************************
    //## SetColor
    //## 
    //## Sets the new modulation color of material.
    //## 
    //## Input:
    //##   fRed   - red value.
    //##   fGreen - green value.
    //##   fBlue  - blue value.
    //##   fAlpha - alpha value.
    //## **********************************************************************
    void SetColor(float fRed,
                  float fGreen,
                  float fBlue,
                  float fAlpha);

private:

    // Modulation color
    float m_arColor[4];
};

#endif