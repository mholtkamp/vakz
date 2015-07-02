#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include "Light.h"

class DirectionalLight : public Light
{

public:

    DirectionalLight();

    ~DirectionalLight();

    void SetRenderState(void*        pScene,
                        unsigned int hProg,
                        int          nLightIndex);

    void SetDirectionVector(float fX,
                            float fY,
                            float fZ);

private:

    float m_arDirection[3];
};


#endif

