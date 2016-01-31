#ifndef EFFECT_H
#define EFFECT_H

enum EffectType
{
    EFFECT_NO_TYPE   = 0,
    EFFECT_BLUR      = 1
};

class Effect
{

public:

    //## **********************************************************************
    //## Constructor
    //## **********************************************************************
    Effect();

    //## **********************************************************************
    //## Destructor
    //## **********************************************************************
    ~Effect();
    
    //## **********************************************************************
    //## IsEnabled
    //##
    //## Returns:
    //##   int - 1 if effect is enabled
    //##         0 otherwise
    //## **********************************************************************
    int IsEnabled();

    //## **********************************************************************
    //## SetEnable
    //##
    //## Enables the effect, allowing it be rendered in post processing 
    //## portion of scene rendering.
    //##
    //## Input:
    //##   nEnable - new enable value
    //## **********************************************************************
    void SetEnable(int nEnable);

    //## **********************************************************************
    //## GetType
    //##
    //## Returns:
    //##   int - Type of effect shader (refer to EffectType enum above).
    //## **********************************************************************
    int GetType();

    //## **********************************************************************
    //## Render
    //##
    //## Renders effect using previously bound FBO's color attachment.
    //##
    //## Input:
    //##   pScene - pointer to current scene being rendered.
    //##   hFBO   - handle to previous framebuffer object
    //##   hColorAttach - handle to previous framebuffer's color texture.
    //##   hDepthAttach - handle to previous framebuffer's depth texture.
    //## **********************************************************************
    virtual void Render(void*         pScene,
                        unsigned int  hFBO,
                        unsigned int  hColorAttach,
                        unsigned int  hDepthAttach) = 0;

protected:

    // Type of effect
    int m_nType;
    
    // Enable flag
    int m_nEnabled;

};

#endif
