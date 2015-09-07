#ifndef RESOURCES_H
#define RESOURCES_H

#include "StaticMesh.h"
#include "Texture.h"
#include "AnimatedMesh.h"
#include "DiffuseMaterial.h"

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


void LoadResources();

void DeleteResources();

#endif