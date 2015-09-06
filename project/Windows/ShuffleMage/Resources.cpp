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

// Animated Mesh Resources
AnimatedMesh* g_pMageAnimatedMesh = 0;

// Materials
DiffuseMaterial* g_pDiffuseMaterial = 0;

// Card Textures
Texture* g_pCardBulletTex = 0;
Texture* g_pCardLaserTex = 0;
Texture* g_pCardHealTex = 0;
Texture* g_pCardInvisTex = 0;
Texture* g_pCardShockwaveTex = 0;
Texture* g_pCardBombTex = 0;
Texture* g_pCardSpearsTex = 0;
Texture* g_pCardBubbleTex = 0;
Texture* g_pCardSwipeTex = 0;
Texture* g_pCardAreaGrabTex = 0;

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
        g_pRedTileTex->LoadBMP("Textures/tile_red_small.bmp");

        g_pBlueTileTex = new Texture();
        g_pBlueTileTex->LoadBMP("Textures/tile_blue_small.bmp");

        // Static Meshes
        g_pMageMesh = new StaticMesh();
        g_pMageMesh->Load("Meshes/mage.obj");

        g_pTileMesh = new StaticMesh();
        g_pTileMesh->Load("Meshes/tile.obj");
        
        // Animated Meshes
        g_pMageAnimatedMesh = new AnimatedMesh();
        g_pMageAnimatedMesh->Load("Meshes/Mage_AM/mage.amf");

        // Set flag to indicate resources have been loaded
        g_pDiffuseMaterial = new DiffuseMaterial();
        g_pDiffuseMaterial->SetColor(1.0f, 1.0f, 1.0f, 1.0f);

        // Card Textures
        g_pCardBulletTex = new Texture();
        g_pCardBulletTex->LoadBMP("Textures/Cards/CardBullet.bmp");
        g_pCardLaserTex = new Texture();
        g_pCardLaserTex->LoadBMP("Textures/Cards/CardLaser.bmp");
        g_pCardHealTex = new Texture();
        g_pCardHealTex->LoadBMP("Textures/Cards/CardHeal.bmp");
        g_pCardInvisTex = new Texture();
        g_pCardInvisTex->LoadBMP("Textures/Cards/CardInvis.bmp");
        g_pCardShockwaveTex = new Texture();
        g_pCardShockwaveTex->LoadBMP("Textures/Cards/CardShockwave.bmp");
        g_pCardBombTex = new Texture();
        g_pCardBombTex->LoadBMP("Textures/Cards/CardBomb.bmp");
        g_pCardSpearsTex = new Texture();
        g_pCardSpearsTex->LoadBMP("Textures/Cards/CardSpears.bmp");
        g_pCardBubbleTex = new Texture();
        g_pCardBubbleTex->LoadBMP("Textures/Cards/CardBubble.bmp");
        g_pCardSwipeTex = new Texture();
        g_pCardSwipeTex->LoadBMP("Textures/Cards/CardSwipe.bmp");
        g_pCardAreaGrabTex = new Texture();
        g_pCardAreaGrabTex->LoadBMP("Textures/Cards/CardAreaGrab.bmp");


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

        // Static Meshes
        delete g_pMageMesh;
        delete g_pTileMesh;

        // Animated Meshes
        delete g_pMageAnimatedMesh;
        
        // Materials
        delete g_pDiffuseMaterial;

        // Card Textures
        delete g_pCardBulletTex;
        delete g_pCardLaserTex;
        delete g_pCardHealTex;
        delete g_pCardInvisTex;
        delete g_pCardShockwaveTex;
        delete g_pCardBombTex;
        delete g_pCardSpearsTex;
        delete g_pCardBubbleTex;
        delete g_pCardSwipeTex;
        delete g_pCardAreaGrabTex;
    }
}
