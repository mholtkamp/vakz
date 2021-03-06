#ifndef MATERIAL_H
#define MATERIAL_H

enum MaterialType
{
    MATERIAL_NO_TYPE     = -1,
    MATERIAL_DIFFUSE     = 0,
    MATERIAL_FULLBRIGHT  = 1,
    MATERIAL_RIMLIT      = 2,
    MATERIAL_TOON        = 3,

    MATERIAL_NUM_TYPES   = 4,
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

    void SetColor(float fRed,
                  float fGreen,
                  float fBlue,
                  float fAlpha);

    void SetColor(float* arColor);

    float* GetColor();

protected:

    //## Material Type
    int m_nType;

    // Material color
    float m_arColor[4];
};

#endif
