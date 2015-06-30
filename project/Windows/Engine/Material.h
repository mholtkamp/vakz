#ifndef MATERIAL_H
#define MATERIAL_H

enum MaterialType
{
    MATERIAL_NO_TYPE  = -1,
    MATERIAL_DIFFUSE  = 0,
    MATERIAL_SPECULAR = 1,
    MATERIAL_FLAT     = 2,
    MATERIAL_TOON     = 3
};

class Material
{

public:

    Material();

    ~Material();

    int GetType();

    virtual void SetRenderState(unsigned int hProg) = 0;

protected:

    int m_nType;
};

#endif
