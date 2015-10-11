#include "Resources.h"
#include "Log.h"

// Texture Resources
Texture* g_pRedMageTex  = 0;
Texture* g_pBlueMageTex = 0;
Texture* g_pRedTileTex  = 0;
Texture* g_pBlueTileTex = 0;
Texture* g_pCastTex     = 0;
Texture* g_pRotateTex   = 0;

// Static Mesh Resources
StaticMesh* g_pMageMesh = 0;
StaticMesh* g_pTileMesh = 0;
StaticMesh* g_pPlaneXY  = 0;
StaticMesh* g_pPlaneXZ  = 0;
StaticMesh* g_pSphere   = 0;

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

// Activations
StaticMesh* g_pActLaserMesh  = 0;
Texture*    g_pActLaserTex   = 0;
Sound*      g_pActLaserSound = 0;

StaticMesh* g_pActBombMesh = 0;
Texture*    g_pActBombTex  = 0;

Texture* g_pActHealTex = 0;

Texture* g_pActInvisTex = 0;

StaticMesh* g_pActAreaGrabDropMesh = 0;
Texture*    g_pActAreaGrabDropTex  = 0;
Texture*    g_pActAreaGrabWaveTex  = 0;

StaticMesh* g_pActSwipeMesh = 0;
Texture*    g_pActSwipeTex  = 0;

StaticMesh* g_pActSpearsMesh = 0;
Texture*    g_pActSpearsTex  = 0;

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

        g_pCastTex     = new Texture();
        g_pCastTex->LoadBMP("Textures/cast.bmp");

        g_pRotateTex   = new Texture();
        g_pRotateTex->LoadBMP("Textures/rotate.bmp");

        // Static Meshes
        g_pMageMesh = new StaticMesh();
        g_pMageMesh->Load("Meshes/mage.obj");

        g_pTileMesh = new StaticMesh();
        g_pTileMesh->Load("Meshes/tile.obj");

        g_pPlaneXY = new StaticMesh();
        g_pPlaneXY->Load("Meshes/planeXY.obj");

        g_pPlaneXZ = new StaticMesh();
        g_pPlaneXZ->Load("Meshes/planeXZ.obj");

        g_pSphere = new StaticMesh();
        g_pSphere->Load("Meshes/sphere.obj");
        
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

        // Activations
        g_pActLaserMesh = new StaticMesh();
        g_pActLaserMesh->Load("Meshes/Activations/laser_mesh.obj");
        g_pActLaserTex  = new Texture();
        g_pActLaserTex->LoadBMP("Textures/Activations/laser_tex.bmp");
        g_pActLaserSound = new Sound();
        g_pActLaserSound->Load("Sounds/laser_sound.wav");

        g_pActBombMesh = new StaticMesh();
        g_pActBombMesh->Load("Meshes/Activations/bomb_mesh.obj");
        g_pActBombTex  = new Texture();
        g_pActBombTex->LoadBMP("Textures/Activations/bomb_tex.bmp");

        g_pActHealTex = new Texture();
        g_pActHealTex->LoadBMP("Textures/Activations/heal_tex.bmp", 1);
        g_pActHealTex->SetFiltering(Texture::NEAREST);

        g_pActInvisTex = new Texture();
        g_pActInvisTex->LoadBMP("Textures/Activations/invis_tex.bmp", 1);

        g_pActAreaGrabDropMesh = new StaticMesh();
        g_pActAreaGrabDropMesh->Load("Meshes/Activations/areagrab_drop_mesh.obj");
        g_pActAreaGrabDropTex  = new Texture;
        g_pActAreaGrabDropTex->LoadBMP("Textures/Activations/areagrab_drop_gray_tex.bmp");
        g_pActAreaGrabWaveTex  = new Texture();
        g_pActAreaGrabWaveTex->LoadBMP("Textures/Activations/areagrab_wave_gray_tex.bmp", 1);

        g_pActSwipeMesh = new StaticMesh();
        g_pActSwipeMesh->Load("Meshes/Activations/swipe_mesh.obj");
        g_pActSwipeTex = new Texture();
        g_pActSwipeTex->LoadBMP("Textures/Activations/swipe_tex.bmp");

        g_pActSpearsMesh = new StaticMesh();
        g_pActSpearsMesh->Load("Meshes/Activations/spears_mesh.obj");
        g_pActSpearsTex = new Texture();
        g_pActSpearsTex->LoadBMP("Textures/Activations/spears_tex.bmp");

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
        delete g_pPlaneXY;
        delete g_pPlaneXZ;
        delete g_pSphere;

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

        // Activations
        delete g_pActLaserMesh;
        delete g_pActLaserTex;
        delete g_pActLaserSound;

        delete g_pActBombMesh;
        delete g_pActBombTex;

        delete g_pActHealTex;

        delete g_pActInvisTex;

        delete g_pActAreaGrabDropMesh;
        delete g_pActAreaGrabDropTex;
        delete g_pActAreaGrabWaveTex;

        delete g_pActSwipeMesh;
        delete g_pActSwipeTex;

        delete g_pActSpearsMesh;
        delete g_pActSpearsTex;
    }
}
