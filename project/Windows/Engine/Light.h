#ifndef LIGHT_H
#define LIGHT_H

class Light
{

public:

    Light();

    ~Light();

    void SetEnable(int nEnable);

    int IsEnabled();

    float GetIntensity();

    void SetIntensity(float fIntensity);

    void SetColor(float fRed,
                  float fGreen,
                  float fBlue,
                  float fAlpha);

    virtual void SetRenderState(void*        pScene,
                                unsigned int hProg,
                                int          nLightIndex) = 0;

protected:

    int m_nEnable;

    float m_fIntensity;

    float m_arColor[4];  
};

#endif
