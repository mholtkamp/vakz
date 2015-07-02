#ifndef SCENE_H
#define SCENE_H

#include "Camera.h"
#include "Matter.h"
#include "Light.h"
#include "Glyph.h"
#include "Effect.h"

class Scene
{
public:

    //## **********************************************************************
    //## Constructor
    //## **********************************************************************
    Scene(int nMaxMatters = DEFAULT_MAX_MATTERS,
          int nMaxLights  = DEFAULT_MAX_LIGHTS,
          int nMaxGlyphs  = DEFAULT_MAX_GLYPHS,
          int nMaxEffects = DEFAULT_MAX_EFFECTS);

    //## **********************************************************************
    //## Destructor
    //## **********************************************************************
    ~Scene();

    //## **********************************************************************
    //## Render
    //##
    //## Renders the scene in a specific order to produce the proper 3D look.
    //## **********************************************************************
    void Render();

    //## **********************************************************************
    //## AddMatter
    //##
    //## Adds a Matter object to the scene. A Matter object is anything that 
    //## can be rendered in 3D, such as meshes and particles.
    //## 
    //## Input:
    //##   pMatter - pointer to a matter object to be added to the scene.
    //## **********************************************************************
    void AddMatter(Matter* pMatter);

    //## **********************************************************************
    //## AddLight
    //##
    //## Adds a Light object to the scene. A Light object is anything that will
    //## effect fragment color of Matters rendered to the screen.
    //##
    //## Input:
    //##   pLight - pointer to a light object to add to the scene.
    //## **********************************************************************
    void AddLight(Light* pLight);

    //## **********************************************************************
    //## AddGlyph
    //##
    //## Adds a Glyph object to the scene. A Glyph object is anything that can
    //## be rendered in 2D, independent of the scene's camera.
    //## 
    //## Input:
    //##   pGlyph - pointer to a glyph object to add to the scene.
    //## **********************************************************************
    void AddGlyph(Glyph* pGlyph);

    //## **********************************************************************
    //## AddEffect
    //##
    //## Adds an Effect object to the scene. An Effect object is anything that 
    //## alters the contents of the framebuffer after all Matters have been 
    //## rendered.
    //##
    //## Input:
    //##   pEffect - pointer to effect object to be added to the scene.
    //## **********************************************************************
    void AddEffect(Effect* pEffect);

    //## **********************************************************************
    //## SetCamera
    //##
    //## Sets the camera for the scene. The camera determines the viewpoint and
    //## perspective from which the scene will be rendered.
    //##
    //## Input:
    //##   pCamera - Pointer to the new camera object. 
    //## **********************************************************************
    void SetCamera(Camera* pCamera);

    //## **********************************************************************
    //## GetCamera
    //##
    //## Gets the currently assigned camera.
    //##
    //## Returns:
    //##   Camera* - pointer to camera
    //## **********************************************************************
    Camera* GetCamera();

    Light** GetLightArray();

    void SetAmbientLight(float fRed,
                         float fGreen,
                         float fBlue,
                         float fAlpha);

    float* GetAmbientLight();

    int GetNumLights();

    enum SceneEnum
    {
        DEFAULT_MAX_MATTERS = 1024,
        DEFAULT_MAX_LIGHTS  = 128,
        DEFAULT_MAX_GLYPHS  = 512,
        DEFAULT_MAX_EFFECTS = 16
    };

private:

    //## Values that hold the maximum counts for each scene component.
    int m_nMaxMatters;
    int m_nMaxLights;
    int m_nMaxGlyphs;
    int m_nMaxEffects;

    //## Values that hold the current number for each component.
    int m_nNumMatters;
    int m_nNumLights;
    int m_nNumGlyphs;
    int m_nNumEffects;

    //## Arrays to hold scene components.
    Matter** m_pMatters;
    Light**  m_pLights;
    Glyph**  m_pGlyphs;
    Effect** m_pEffects;

    //## Pointer to camera that the 3D scene should be rendered from.
    Camera* m_pCamera;

    // Ambient light
    float m_arAmbientColor[4];
};

#endif
