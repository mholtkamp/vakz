#ifndef BLUR_EFFECT_H
#define BLUE_EFFECT_H

#include "Effect.h"

class BlurEffect : public Effect
{
public:

    BlurEffect();
    ~BlurEffect();

    void Render(void*         pScene,
                unsigned int  hFBO,
                unsigned int  hColorAttach,
                unsigned int  hDepthAttach);

    void SetBlurFactor(int nFactor);

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

    int m_nBlurFactor;
    int m_nSampleDistance;
};

#endif