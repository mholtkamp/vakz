#ifndef RESOURCES_H
#define RESOURCES_H

#include "StaticMesh.h"
#include "Texture.h"
#include "AnimatedMesh.h"
#include "DiffuseMaterial.h"
#include "Sound.h"

// Texture Resources
extern Texture* g_pRedMageTex;
extern Texture* g_pBlueMageTex;
extern Texture* g_pRedTileTex;
extern Texture* g_pBlueTileTex;
extern Texture* g_pCastTex;
extern Texture* g_pRotateTex;

// Static Mesh Resources
extern StaticMesh* g_pMageMesh;
extern StaticMesh* g_pTileMesh;
extern StaticMesh* g_pPlaneXY;
extern StaticMesh* g_pPlaneXZ;

// Animated Mesh Resources
extern AnimatedMesh* g_pMageAnimatedMesh;

// Materials
extern DiffuseMaterial* g_pDiffuseMaterial;

// Card Textures
extern Texture* g_pCardBulletTex;
extern Texture* g_pCardLaserTex;
extern Texture* g_pCardHealTex;
extern Texture* g_pCardInvisTex;
extern Texture* g_pCardShockwaveTex;
extern Texture* g_pCardBombTex;
extern Texture* g_pCardSpearsTex;
extern Texture* g_pCardBubbleTex;
extern Texture* g_pCardSwipeTex;
extern Texture* g_pCardAreaGrabTex;

// Activations
extern StaticMesh* g_pActLaserMesh;
extern Texture*    g_pActLaserTex;
extern Sound*      g_pActLaserSound;

extern StaticMesh* g_pActBombMesh;
extern Texture*    g_pActBombTex;

extern Texture*    g_pActHealTex;

extern Texture*    g_pActInvisTex;

extern StaticMesh* g_pActAreaGrabDropMesh;
extern Texture*    g_pActAreaGrabDropTex;
extern Texture*    g_pActAreaGrabWaveTex;

void LoadResources();

void DeleteResources();

#endif