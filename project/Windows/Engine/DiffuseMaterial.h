#ifndef DIFFUSE_MATERIAL_H
#define DIFFUSE_MATERIAL_H

#include "Material.h"

class DiffuseMaterial : public Material
{
    
public:
    //## **********************************************************************
    //## Constructor
    //## **********************************************************************
    DiffuseMaterial();

    //## **********************************************************************
    //## Destructor
    //## **********************************************************************
    ~DiffuseMaterial();

    //## **********************************************************************
    //## GetType
    //##
    //## Returns:
    //##   int - type of Material. Refer to MaterialType enum in Material.h
    //## **********************************************************************
    int GetType();

    //## **********************************************************************
    //## SetRenderState
    //##
    //## Sets up the opengl to properly render matter.
    //##
    //## Input:
    //##   pScene - pointer to scene in which rendering is being performed.
    //##   hProg  - handle to shader program that is bound.
    //## **********************************************************************
    void SetRenderState(void*        pScene,
                        unsigned int hProg);

    //## **********************************************************************
    //## SetColor
    //##
    //## Sets the diffuse material color. Values should be in range 0.0f to 1.0f
    //##
    //## Input:
    //##   fRed   - red component.
    //##   fGreen - green component.
    //##   fBlue  - blue component.
    //##   fAlpha - alpha component.
    //## **********************************************************************
    void SetColor(float fRed,
                  float fGreen,
                  float fBlue,
                  float fAlpha);

private:

    // Material color
    float m_arColor[4];
};

#endif
