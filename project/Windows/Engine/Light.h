#ifndef LIGHT_H
#define LIGHT_H

#include "Actor.h"

enum LightType
{
    LIGHT_NO_TYPE     = -1,
    LIGHT_DIRECTIONAL = 0,
    LIGHT_POINT       = 1
};

class Light : public Actor
{

public:

    //## **********************************************************************
    //## Constructor
    //## **********************************************************************
    Light();

    //## **********************************************************************
    //## Destructor
    //## **********************************************************************
    virtual ~Light();

    //## **********************************************************************
    //## SetEnable
    //##
    //## Enable the light to be factored into the rendering pipeline.
    //##
    //## Input:
    //##   nEnable - '1' to enable light.
    //##             '0' to disable light.
    //## **********************************************************************
    void SetEnable(int nEnable);

    //## **********************************************************************
    //## IsEnabled
    //##
    //## Used to check if the light is enabled.
    //##
    //## Returns:
    //##   int - '1' if enabled.
    //##       - '0' if disabled.
    //## **********************************************************************
    int IsEnabled();

    //## **********************************************************************
    //## SetColor
    //##
    //## Sets the color of the light. Values should be between 0.0f and 1.0f.
    //##
    //## Input:
    //##   fRed   - red component of color.
    //##   fGreen - green component.
    //##   fBlue  - blue component.
    //## **********************************************************************
    void SetColor(float fRed,
                  float fGreen,
                  float fBlue);

    float* GetColor();

    //## **********************************************************************
    //## SetRenderState
    //##
    //## Pure virtual function that should be overridden by every child Light
    //## class. This method sets up the opengl state for rendering a light,
    //## specifically by setting shader uniforms.
    //##
    //## Input:
    //##   pScene      - pointer to current scene.
    //##   hProg       - handle to currently bound shader program.
    //## **********************************************************************
    virtual void SetRenderState(void*        pScene,
                                unsigned int hProg) = 0;

    //## **********************************************************************
    //## GetType
    //##
    //## Returns the type of light. Refer to LightType enum above.
    //##
    //## Returns:
    //##   int - '1' if enabled.
    //##       - '0' if disabled.
    //## **********************************************************************
    int GetType();

protected:

    //## Type of light
    int m_nType;

    //## Enable flag
    int m_nEnable;

    //## Color of the light
    float m_arColor[3];  
};

#endif
