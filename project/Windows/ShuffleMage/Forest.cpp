#include "Forest.h"
#include "Resources.h"

#define X_SHIFT_1 3.75f
#define Y_SHIFT_1 0.0f
#define Z_SHIFT_1 1.0f

#define X_SHIFT_2 3.75f
#define Y_SHIFT_2 0.0f
#define Z_SHIFT_2 1.0f

StaticMesh Forest::s_GroundMesh;
StaticMesh Forest::s_GrassMesh;
StaticMesh Forest::s_RockMesh;
StaticMesh Forest::s_MushroomMesh;
StaticMesh Forest::s_SignpostMesh;
StaticMesh Forest::s_LanternMesh;
StaticMesh Forest::s_TreeMesh;
Texture Forest::s_GroundTex;
Texture Forest::s_GrassTex;
Texture Forest::s_RockTex;
Texture Forest::s_MushroomTex;
Texture Forest::s_SignpostTex;
Texture Forest::s_LanternTex;
Texture Forest::s_TreeTex;

int Forest::s_nResLoaded = 0;

Forest::Forest()
{
    LoadResources();

    m_matGround.SetMesh(&s_GroundMesh);
    m_matGround.SetTexture(&s_GroundTex);
    m_matGround.SetMaterial(g_pDiffuseMaterial);

    m_matGrass.SetMesh(&s_GrassMesh);
    m_matGrass.SetTexture(&s_GrassTex);
    m_matGrass.SetMaterial(g_pDiffuseMaterial);

    m_matRock.SetMesh(&s_RockMesh);
    m_matRock.SetTexture(&s_RockTex);
    m_matRock.SetMaterial(g_pDiffuseMaterial);
    
    m_matMushroom.SetMesh(&s_MushroomMesh);
    m_matMushroom.SetTexture(&s_MushroomTex);
    m_matMushroom.SetMaterial(g_pDiffuseMaterial);

    m_matSignpost.SetMesh(&s_SignpostMesh);
    m_matSignpost.SetTexture(&s_SignpostTex);
    m_matSignpost.SetMaterial(g_pDiffuseMaterial);

    m_matLantern.SetMesh(&s_LanternMesh);
    m_matLantern.SetTexture(&s_LanternTex);
    m_matLantern.SetMaterial(g_pDiffuseMaterial);

    m_matTree.SetMesh(&s_TreeMesh);
    m_matTree.SetTexture(&s_TreeTex);
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
    pScene->AddActor(&m_matGround);
    pScene->AddActor(&m_matGrass);
    pScene->AddActor(&m_matRock);
    pScene->AddActor(&m_matMushroom);
    pScene->AddActor(&m_matSignpost);
    pScene->AddActor(&m_matLantern);
    pScene->AddActor(&m_matTree);
}

void Forest::LoadResources()
{
    if (s_nResLoaded == 0)
    {
        s_GroundMesh.Load("Meshes/Forest_Scene/ground.obj");
        s_GrassMesh.Load("Meshes/Forest_Scene/grass.obj");
        s_RockMesh.Load("Meshes/Forest_Scene/rocks.obj");
        s_MushroomMesh.Load("Meshes/Forest_Scene/mushrooms.obj");
        s_SignpostMesh.Load("Meshes/Forest_Scene/signpost.obj");
        s_LanternMesh.Load("Meshes/Forest_Scene/lantern.obj");
        s_TreeMesh.Load("Meshes/Forest_Scene/trees.obj");

         s_GroundTex.LoadBMP("Textures/Forest_Scene/ground_tex.bmp");
         s_GrassTex.LoadBMP("Textures/Forest_Scene/grass_tex.bmp");
         s_RockTex.LoadBMP("Textures/Forest_Scene/rock_tex.bmp");
         s_MushroomTex.LoadBMP("Textures/Forest_Scene/mushroom_tex.bmp");
         s_SignpostTex.LoadBMP("Textures/Forest_Scene/sign_tex.bmp");
         s_LanternTex.LoadBMP("Textures/Forest_Scene/lantern_tex.bmp");
         s_TreeTex.LoadBMP("Textures/Forest_Scene/tree_tex.bmp", 1);
         s_TreeTex.SetFiltering(Texture::NEAREST);

        s_nResLoaded = 1;
    }
}
