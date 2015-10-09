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
    static StaticMesh s_pForestGroundMesh;
    static StaticMesh s_pForestGrassMesh;
    static StaticMesh s_pForestRockMesh;
    static StaticMesh s_pForestMushroomMesh;
    static StaticMesh s_pForestSignpostMesh;
    static StaticMesh s_pForestLanternMesh;
    static StaticMesh s_pForestTreeMesh;

    static Texture s_pForestGroundTex;
    static Texture s_pForestGrassTex;
    static Texture s_pForestRockTex;
    static Texture s_pForestMushroomTex;
    static Texture s_pForestSignpostTex;
    static Texture s_pForestLanternTex;
    static Texture s_pForestTreeTex;

    static int s_nResLoaded;
};



#endif