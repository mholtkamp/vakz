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

    void SetLightingIntervalCount(int nIntervals);

    int GetLightingIntervalCount();

private:

    int m_nIntervals;
};

#endif