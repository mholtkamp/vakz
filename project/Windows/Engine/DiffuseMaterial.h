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

};

#endif
