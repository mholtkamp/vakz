#ifndef RIMLIT_MATERIAL_H
#define RIMLIT_MATERIAL_H

#include "Material.h"

class RimlitMaterial : public Material
{
public:

    RimlitMaterial();
    ~RimlitMaterial();

    void SetRenderState(void*        pScene,
                        unsigned int hProg);

    void SetDiffuseColor(float arDiffuseColor[4],
                         float arRimColor[4]);

private:

    float m_arDiffuseColor[4];
    float m_arRimColor[4];
};

#endif