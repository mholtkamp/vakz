#ifndef DIFFUSE_MATERIAL_H
#define DIFFUSE_MATERIAL_H

#include "Material.h"

class DiffuseMaterial : public Material
{
    
public:

    DiffuseMaterial();

    ~DiffuseMaterial();

    int GetType();

    void SetRenderState(unsigned int hProg);

private:

    // Material properties
    float m_arColor[4];
    
};

#endif
