#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include "Light.h"

class DirectionalLight : public Light
{

public:

    //## **********************************************************************
    //## Constructor
    //## **********************************************************************
    DirectionalLight();

    //## **********************************************************************
    //## Destructor
    //## **********************************************************************
    ~DirectionalLight();

    //## **********************************************************************
    //## SetRenderState
    //## 
    //## Sets up the opengl rendering state for using directional lighting in
    //## the fragment shader.
    //## 
    //## Input:
    //##   pScene      - pointer to the scene being rendered.
    //##   hProg       - handle to the shader program that is already bound.
    //##   nLightIndex - currently unused, but left for growth.
    //## **********************************************************************
    void SetRenderState(void*        pScene,
                        unsigned int hProg,
                        int          nLightIndex);

    //## **********************************************************************
    //## SetDirectionVector
    //## 
    //## Gives the directional light its direction. The vector does not need
    //## to be normalized, as this is done already in the fragment shader.
    //## 
    //## Input:
    //##   fX - x component of vector.
    //##   fY - y component of vector.
    //##   fZ - z component of vector.
    //## **********************************************************************
    void SetDirectionVector(float fX,
                            float fY,
                            float fZ);

private:

    //## Float array to hold 3 dimensional direction vector.
    float m_arDirection[3];
};


#endif

