#ifndef FULLBRIGHT_MATERIAL_H
#define FULLBRIGHT_MATERIAL_H

#include "Material.h"

class FullbrightMaterial : public Material
{
public:

    FullbrightMaterial();
    ~FullbrightMaterial();

    int GetType();

    void SetRenderState(void*        pScene,
                        unsigned int hProg);

    void SetColor(float fRed,
                  float fGreen,
                  float fBlue,
                  float fAlpha);

private:

    float m_arColor[4];
};

#endif