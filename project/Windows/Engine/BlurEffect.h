#ifndef BLUR_EFFECT_H
#define BLUE_EFFECT_H

#include "Effect.h"

class BlurEffect : public Effect
{
public:

    //## **********************************************************************
    //## Constructor
    //## **********************************************************************
    BlurEffect();

    //## **********************************************************************
    //## Destructor
    //## **********************************************************************
    ~BlurEffect();

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
    void Render(void*         pScene,
                unsigned int  hFBO,
                unsigned int  hColorAttach,
                unsigned int  hDepthAttach);

    //## **********************************************************************
    //## SetBlurFactor
    //## 
    //## Set's blur factor used by blur effect fragment shader.
    //##
    //## Input:
    //##   nFactor - texture sampling radius.
    //## **********************************************************************
    void SetBlurFactor(int nFactor);

    //## **********************************************************************
    //## SetSampleDistance
    //##
    //## Set's sample distance used by blur effect fragment shader.
    //##
    //## Input:
    //##   nDistance - distance between texels being sampled.
    //## **********************************************************************
    void SetSampleDistance(int nDistance);
    
    enum BlurEffectEnum
    {
        DEFAULT_BLUR_FACTOR     = 2,
        DEFAULT_SAMPLE_DISTANCE = 1,

        MAX_BLUR_FACTOR     = 10,
        MAX_SAMPLE_DISTANCE = 5000,

        MIN_BLUR_FACTOR     = 1,
        MIN_SAMPLE_DISTANCE = 1
    };

private:

    // Kernel size of blur
    // Texture samples required in blue effect shader  = (2n + 1)^2
    int m_nBlurFactor;

    // Distance between texture samples
    int m_nSampleDistance;
};

#endif