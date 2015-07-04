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
    //## **********************************************************************
    //## Constructor
    //## **********************************************************************
    Material();

    //## **********************************************************************
    //## Destructor
    //## **********************************************************************
    ~Material();

    //## **********************************************************************
    //## GetType
    //## 
    //## Returns the material type. Refer to MaterialType enum above.
    //##
    //## Returns:
    //##   int - material type enumeration.
    //## **********************************************************************
    int GetType();

    //## **********************************************************************
    //## SetRenderState
    //## 
    //## Sets up the opengl state for rendering a matter. This function is pure
    //## virtual and should be implemented by every child Material.
    //##
    //## Input:
    //##   pScene - pointer to current scene.
    //##   hProg  - handle to currently bound shader program.
    //## **********************************************************************
    virtual void SetRenderState(void*        pScene,
                                unsigned int hProg) = 0;

protected:

    //## Material Type
    int m_nType;
};

#endif
