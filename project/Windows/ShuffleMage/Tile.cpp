#include "Tile.h"
#include "Resources.h"

#if defined (SM_CLIENT)
#include "Game.h"
#else
#include "ServerGame.h"
#endif

Tile::Tile()
{
    // Set position of matter
    SetPosition(0, 0);

#ifdef SM_CLIENT
    m_matter.SetMesh(g_pTileMesh);
    m_matter.SetTexture(g_pRedTileTex);
    m_matter.SetMaterial(g_pDiffuseMaterial);
#endif

    m_nOwner = SIDE_1;
    m_nType  = TILE_TYPE_NORMAL;
    m_pPawn  = 0;
    m_pGame  = 0;
}

Tile::~Tile()
{

}

void Tile::Register(Scene* pScene)
{
#ifdef SM_CLIENT
    pScene->AddMatter(&m_matter);
#endif
}

int Tile::GetOwner()
{
    return m_nOwner;
}

int Tile::GetTileType()
{
    return m_nType;
}

void Tile::SetOwner(int nOwner)
{
    m_nOwner = nOwner;
    
#ifdef SM_CLIENT
    UpdateTexture();
#else
    reinterpret_cast<ServerGame*>(m_pGame)->UpdateTile(m_nX, m_nZ, m_nOwner, m_nType);
#endif
}

void Tile::SetPosition(int nX,
                       int nZ)
{   
    m_nX = nX;
    m_nZ = nZ;

#ifdef SM_CLIENT
    m_matter.SetPosition(nX * TILE_WIDTH, 0.0f, nZ * TILE_HEIGHT);
#endif
}

Pawn* Tile::GetPawn()
{
    return m_pPawn;
}

void Tile::SetPawn(Pawn* pPawn)
{
    m_pPawn = pPawn;
}

void Tile::SetGame(void* pGame)
{
    m_pGame = pGame;
}

void Tile::SetType(int nType)
{
    m_nType = nType;

#if defined (SM_SERVER)
    reinterpret_cast<ServerGame*>(m_pGame)->UpdateTile(m_nX, m_nZ, m_nOwner, m_nType);
#endif
}

void Tile::UpdateTexture()
{
#if defined (SM_CLIENT)
    // This function needs to be updated so that the proper texture is used,
    // not just the "Normal" texture.
    m_matter.SetTexture((m_nOwner == SIDE_1) ? g_pRedTileTex : g_pBlueTileTex);
#endif
}
