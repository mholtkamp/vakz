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
};

#endif