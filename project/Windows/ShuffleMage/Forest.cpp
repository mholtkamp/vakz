#include "Forest.h"
#include "Resources.h"

#define X_SHIFT_1 3.75f
#define Y_SHIFT_1 0.0f
#define Z_SHIFT_1 1.0f

#define X_SHIFT_2 3.75f
#define Y_SHIFT_2 0.0f
#define Z_SHIFT_2 1.0f

StaticMesh Forest::s_pForestGroundMesh;
StaticMesh Forest::s_pForestGrassMesh;
StaticMesh Forest::s_pForestRockMesh;
StaticMesh Forest::s_pForestMushroomMesh;
StaticMesh Forest::s_pForestSignpostMesh;
StaticMesh Forest::s_pForestLanternMesh;
StaticMesh Forest::s_pForestTreeMesh;
Texture Forest::s_pForestGroundTex;
Texture Forest::s_pForestGrassTex;
Texture Forest::s_pForestRockTex;
Texture Forest::s_pForestMushroomTex;
Texture Forest::s_pForestSignpostTex;
Texture Forest::s_pForestLanternTex;
Texture Forest::s_pForestTreeTex;

int Forest::s_nResLoaded = 0;

Forest::Forest()
{
    LoadResources();

    m_matGround.SetMesh(&s_pForestGroundMesh);
    m_matGround.SetTexture(&s_pForestGroundTex);
    m_matGround.SetMaterial(g_pDiffuseMaterial);

    m_matGrass.SetMesh(&s_pForestGrassMesh);
    m_matGrass.SetTexture(&s_pForestGrassTex);
    m_matGrass.SetMaterial(g_pDiffuseMaterial);

    m_matRock.SetMesh(&s_pForestRockMesh);
    m_matRock.SetTexture(&s_pForestRockTex);
    m_matRock.SetMaterial(g_pDiffuseMaterial);
    
    m_matMushroom.SetMesh(&s_pForestMushroomMesh);
    m_matMushroom.SetTexture(&s_pForestMushroomTex);
    m_matMushroom.SetMaterial(g_pDiffuseMaterial);

    m_matSignpost.SetMesh(&s_pForestSignpostMesh);
    m_matSignpost.SetTexture(&s_pForestSignpostTex);
    m_matSignpost.SetMaterial(g_pDiffuseMaterial);

    m_matLantern.SetMesh(&s_pForestLanternMesh);
    m_matLantern.SetTexture(&s_pForestLanternTex);
    m_matLantern.SetMaterial(g_pDiffuseMaterial);

    m_matTree.SetMesh(&s_pForestTreeMesh);
    m_matTree.SetTexture(&s_pForestTreeTex);
    m_matTree.SetMaterial(g_pDiffuseMaterial);
}

Forest::~Forest()
{

}

void Forest::SetSide(int nSide)
{
    if (nSide == 0)
    {
        m_matGround.SetPosition(X_SHIFT_1, Y_SHIFT_1, Z_SHIFT_1);
        m_matGrass.SetPosition(X_SHIFT_1, Y_SHIFT_1, Z_SHIFT_1);
        m_matRock.SetPosition(X_SHIFT_1, Y_SHIFT_1, Z_SHIFT_1);
        m_matMushroom.SetPosition(X_SHIFT_1, Y_SHIFT_1, Z_SHIFT_1);
        m_matSignpost.SetPosition(X_SHIFT_1, Y_SHIFT_1, Z_SHIFT_1);
        m_matLantern.SetPosition(X_SHIFT_1, Y_SHIFT_1, Z_SHIFT_1);
        m_matTree.SetPosition(X_SHIFT_1, Y_SHIFT_1, Z_SHIFT_1);
    }
    else
    {

        m_matGround.SetPosition(X_SHIFT_2, Y_SHIFT_2, Z_SHIFT_2);
        m_matGrass.SetPosition(X_SHIFT_2, Y_SHIFT_2, Z_SHIFT_2);
        m_matRock.SetPosition(X_SHIFT_2, Y_SHIFT_2, Z_SHIFT_2);
        m_matMushroom.SetPosition(X_SHIFT_2, Y_SHIFT_2, Z_SHIFT_2);
        m_matSignpost.SetPosition(X_SHIFT_2, Y_SHIFT_2, Z_SHIFT_2);
        m_matLantern.SetPosition(X_SHIFT_2, Y_SHIFT_2, Z_SHIFT_2);
        m_matTree.SetPosition(X_SHIFT_2, Y_SHIFT_2, Z_SHIFT_2);

        m_matGround.SetRotation(0.0f, 180.0f, 0.0f);
        m_matGrass.SetRotation(0.0f, 180.0f, 0.0f);
        m_matRock.SetRotation(0.0f, 180.0f, 0.0f);
        m_matMushroom.SetRotation(0.0f, 180.0f, 0.0f);
        m_matSignpost.SetRotation(0.0f, 180.0f, 0.0f);
        m_matLantern.SetRotation(0.0f, 180.0f, 0.0f);
        m_matTree.SetRotation(0.0f, 180.0f, 0.0f);
    }
}

void Forest::Register(Scene* pScene)
{
    pScene->AddMatter(&m_matGround);
    pScene->AddMatter(&m_matGrass);
    pScene->AddMatter(&m_matRock);
    pScene->AddMatter(&m_matMushroom);
    pScene->AddMatter(&m_matSignpost);
    pScene->AddMatter(&m_matLantern);
    pScene->AddMatter(&m_matTree);
}

void Forest::LoadResources()
{
    if (s_nResLoaded == 0)
    {
        s_pForestGroundMesh.Load("Meshes/Forest_Scene/ground.obj");
        s_pForestGrassMesh.Load("Meshes/Forest_Scene/grass.obj");
        s_pForestRockMesh.Load("Meshes/Forest_Scene/rocks.obj");
        s_pForestMushroomMesh.Load("Meshes/Forest_Scene/mushrooms.obj");
        s_pForestSignpostMesh.Load("Meshes/Forest_Scene/signpost.obj");
        s_pForestLanternMesh.Load("Meshes/Forest_Scene/lantern.obj");
        s_pForestTreeMesh.Load("Meshes/Forest_Scene/trees.obj");

         s_pForestGroundTex.LoadBMP("Textures/Forest_Scene/ground_tex.bmp");
         s_pForestGrassTex.LoadBMP("Textures/Forest_Scene/grass_tex.bmp");
         s_pForestRockTex.LoadBMP("Textures/Forest_Scene/rock_tex.bmp");
         s_pForestMushroomTex.LoadBMP("Textures/Forest_Scene/mushroom_tex.bmp");
         s_pForestSignpostTex.LoadBMP("Textures/Forest_Scene/sign_tex.bmp");
         s_pForestLanternTex.LoadBMP("Textures/Forest_Scene/lantern_tex.bmp");
         s_pForestTreeTex.LoadBMP("Textures/Forest_Scene/tree_tex.bmp", 1);
         s_pForestTreeTex.SetFiltering(Texture::NEAREST);

        s_nResLoaded = 1;
    }
}
