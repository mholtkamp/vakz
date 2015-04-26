#ifndef SCENE_H
#define SCENE_H

#include "Camera.h"
#include "Matter.h"
#include "Light.h"
#include "Glyph.h"
#include "Effect.h"

class Scene
{
    Scene();
    ~Scene();

    //#########################################################################
    // Renders the scene in a specific order to produce the proper 3D look.
    //#########################################################################
    void Render();

    //#########################################################################
    // Adds a Matter object to the scene. A Matter object is anything that 
    // can be rendered in 3D, such as meshes and particles.
    //#########################################################################
    void AddMatter(Matter* pMatter);

    //#########################################################################
    // Adds a Light object to the scene. A Light object is anything that will
    // effect fragment color of Matters rendered to the screen.
    //#########################################################################
    void AddLight(Light* pLight);

    //#########################################################################
    // Adds a Glyph object to the scene. A Glyph object is anything that can
    // be rendered in 2D, independent of the scene's camera.
    //#########################################################################
    void AddGlyph(Glyph* pGlyph);

    //#########################################################################
    // Adds an Effect object to the scene. An Effect object is anything that 
    // alters the contents of the framebuffer after all Matters have been 
    // rendered.
    //#########################################################################
    void AddEffect(Effect* pEffect);

    //#########################################################################
    // Sets the camera for the scene. The camera determines the viewpoint and
    // perspective from which the scene will be rendered.
    //#########################################################################
    void SetCamera(Camera* pCamera);
};

#endif
