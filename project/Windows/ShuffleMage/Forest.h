#ifndef FOREST_H
#define FOREST_H

#include "Scene.h"
#include "Matter.h"
#include "StaticMesh.h"
#include "Texture.h"

class Forest
{
public:

    Forest();
    ~Forest();

    void Register(Scene* pScene);

    static void LoadResources();

    void SetSide(int nSide);

private:

    // Forest scene objects
    Matter m_matGround;
    Matter m_matGrass;
    Matter m_matRock;
    Matter m_matMushroom;
    Matter m_matSignpost;
    Matter m_matLantern;
    Matter m_matTree;

    // Forest resources
    static StaticMesh s_GroundMesh;
    static StaticMesh s_GrassMesh;
    static StaticMesh s_RockMesh;
    static StaticMesh s_MushroomMesh;
    static StaticMesh s_SignpostMesh;
    static StaticMesh s_LanternMesh;
    static StaticMesh s_TreeMesh;

    static Texture s_GroundTex;
    static Texture s_GrassTex;
    static Texture s_RockTex;
    static Texture s_MushroomTex;
    static Texture s_SignpostTex;
    static Texture s_LanternTex;
    static Texture s_TreeTex;

    static int s_nResLoaded;
};



#endif