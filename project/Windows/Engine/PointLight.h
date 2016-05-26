#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "Light.h"
#include "Box.h"

class PointLight : public Light
{
public:

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
    //## **********************************************************************
    void SetRenderState(void*        pScene,
                        unsigned int hProg);

    void SetIntensity(float fIntensity);

    void SetPosition(float fX,
                     float fY,
                     float fZ);

    void SetBox(Box& box);

    float GetIntensity();

    Box* GetBox();

    float* GetPosition();

private:

    //## Position in world coordinates
    float m_arPosition[3];

    //## Space that light should affect
    Box m_box;

    //## Intensity of the light
    float m_fIntensity;
    
};

#endif