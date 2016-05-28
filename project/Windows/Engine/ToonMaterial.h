#ifndef TOON_MATERIAL_H
#define TOON_MATERIAL_H

#include "Material.h"

class ToonMaterial : public Material
{
public:

    ToonMaterial();

    ~ToonMaterial();

    void SetRenderState(void*        pScene,
                        unsigned int hProg);

    void SetColor(float fRed,
                  float fGreen,
                  float fBlue,
                  float fAlpha);

    void SetLightingIntervalCount(int nIntervals);

private:

    float m_arColor[4];

    int m_nIntervals;
};

#endif