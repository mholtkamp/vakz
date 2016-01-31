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

    //## **********************************************************************
    //## SetRenderState
    //##
    //## Sets up the opengl to properly render matter.
    //##
    //## Input:
    //##   pScene - pointer to scene in which rendering is being performed.
    //##   hProg  - handle to shader program that is bound.
    //## **********************************************************************
    void SetRenderState(void*        pScene,
                        unsigned int hProg);

    //## **********************************************************************
    //## SetDiffuseColor
    //##
    //## Sets the diffuse material color. Values should be in range 0.0f to 1.0f
    //##
    //## Input:
    //##   fRed   - red component.
    //##   fGreen - green component.
    //##   fBlue  - blue component.
    //##   fAlpha - alpha component.
    //## **********************************************************************
    void SetDiffuseColor(float arDiffuseColor[4]);

    //## **********************************************************************
    //## SetColor
    //##
    //## Sets the diffuse material color. Values should be in range 0.0f to 1.0f
    //##
    //## Input:
    //##   fRed   - red component.
    //##   fGreen - green component.
    //##   fBlue  - blue component.
    //##   fAlpha - alpha component.
    //## **********************************************************************
    void SetRimColor(float arRimColor[4]);

    //## **********************************************************************
    //## SetRimSize
    //##
    //## Sets the size of the rim lighting effect.
    //##
    //## Input:
    //##   fSize - the size of the rim lighting, should be between 0.0f
    //##           1.0f and represents the range of normals that will be 
    //##           shaded. 1.0f means all fragments will be shaded with rim
    //##           color. 0.0f means no fragments will be shaded with rim
    //##           color.  Values in between will result in various amounts of
    //##           rim shading.
    //## **********************************************************************
    void SetRimSize(float fSize);

    //## **********************************************************************
    //## SetRimStyle
    //##
    //## Controls how rim color is modulated with texture color.
    //##
    //## Input:
    //##   nStyle - RIM_STYLE_HARD: any fragments that would be affected by
    //##              by rim lighting (based on rim size) will overtake
    //##              texture color.
    //##            RIM_STYLE_SOFT: any fragments that would be affected by
    //##              by rim lighting (based on rim size) will be mixed
    //##              with texture color based on the direction of the 
    //##              fragments normals. Normals that face at 90 degree angles
    //##              from the camera vector will be shaded purely rim color.
    //##              Normals that point in the same direction as the eye 
    //##              vectory will be shaded only using texture color.
    //## **********************************************************************
    void SetRimStyle(int nStyle);

private:

    // Diffuse color
    float m_arDiffuseColor[4];

    // Rim color
    float m_arRimColor[4];

    // Rim size cutoff
    float m_fRimSize;

    // Style of rim shading (refer to RimlitMaterialEnum)
    int m_nRimStyle;
};

#endif