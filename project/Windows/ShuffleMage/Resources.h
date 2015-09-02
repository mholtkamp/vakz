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

// Static Mesh Resources
extern StaticMesh* g_pMageMesh;
extern StaticMesh* g_pTileMesh;

// Animated Mesh Resources
extern AnimatedMesh* g_pMageAnimatedMesh;

// Materials
extern DiffuseMaterial* g_pDiffuseMaterial;

void LoadResources();

void DeleteResources();

#endif