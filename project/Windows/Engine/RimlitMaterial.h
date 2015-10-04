#ifndef RIMLIT_MATERIAL_H
#define RIMLIT_MATERIAL_H

#include "Material.h"

enum RimlitMaterialEnum
{
    RIM_STYLE_HARD = 0,
    RIM_STYLE_SOFT = 1
};

class RimlitMaterial : public Material
{
public:

    RimlitMaterial();
    ~RimlitMaterial();

    void SetRenderState(void*        pScene,
                        unsigned int hProg);

    void SetDiffuseColor(float arDiffuseColor[4]);

    void SetRimColor(float arRimColor[4]);

    void SetRimSize(float fSize);

    void SetRimStyle(int nStyle);

private:

    float m_arDiffuseColor[4];
    float m_arRimColor[4];
    float m_fRimSize;
    int m_nRimStyle;
};

#endif