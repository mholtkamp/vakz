#include "Resources.h"
#include "Log.h"

// Texture Resources
Texture* g_pRedMageTex  = 0;
Texture* g_pBlueMageTex = 0;
Texture* g_pRedTileTex  = 0;
Texture* g_pBlueTileTex = 0;

// Static Mesh Resources
StaticMesh* g_pMageMesh = 0;
StaticMesh* g_pTileMesh = 0;

// Materials
DiffuseMaterial* g_pDiffuseMaterial;

static int s_nLoaded = 0;

void LoadResources()
{
    if (s_nLoaded == 0)
    {

        // Textures
        g_pRedMageTex = new Texture();
        g_pRedMageTex->LoadBMP("Textures/mage_red.bmp");

        g_pBlueMageTex = new Texture();
        g_pBlueMageTex->LoadBMP("Textures/mage_blue.bmp");

        g_pRedTileTex = new Texture();
        g_pRedTileTex->LoadBMP("Textures/tile_red.bmp");

        g_pBlueTileTex = new Texture();
        g_pBlueTileTex->LoadBMP("Textures/tile_blue.bmp");

        // Meshes
        g_pMageMesh = new StaticMesh();
        g_pMageMesh->Load("Meshes/mage.obj");

        g_pTileMesh = new StaticMesh();
        g_pTileMesh->Load("Meshes/tile.obj");

        // Set flag to indicate resources have been loaded
        g_pDiffuseMaterial = new DiffuseMaterial();
        g_pDiffuseMaterial->SetColor(1.0f, 1.0f, 1.0f, 1.0f);

        s_nLoaded = 1;
    }
}

void DeleteResources()
{
    if (s_nLoaded != 0)
    {
        // Textures
        delete g_pRedMageTex;
        delete g_pBlueMageTex;
        delete g_pRedTileTex;
        delete g_pBlueTileTex;

        // Meshes
        delete g_pMageMesh;
        delete g_pTileMesh;
    }
}