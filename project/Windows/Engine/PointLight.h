#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "Light.h"

class PointLight : public Light
{
    //## **********************************************************************
    //## Constructor
    //## **********************************************************************
    PointLight();

    //## **********************************************************************
    //## Destructor
    //## **********************************************************************
    ~PointLight();

    //## **********************************************************************
    //## SetRenderState
    //##
    //## This method sets up the opengl state for rendering a light,
    //## specifically by setting shader uniforms.
    //##
    //## Input:
    //##   pScene      - pointer to current scene.
    //##   hProg       - handle to currently bound shader program.
    //##   nLightIndex - currently not used, left for growth.
    //## **********************************************************************
    void SetRenderState(void*        pScene,
                        unsigned int hProg,
                        int          nLightIndex);
};

#endif