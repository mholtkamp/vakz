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

    Effect();
    ~Effect();

    int IsEnabled();

    void SetEnable(int nEnable);

    int GetType();

    virtual void Render(void*         pScene,
                        unsigned int  hFBO,
                        unsigned int  hColorAttach,
                        unsigned int  hDepthAttach) = 0;

protected:

    int m_nType;
    int m_nEnabled;

};

#endif
