#include "Tile.h"
#include "Resources.h"

Tile::Tile()
{
    // Set position of matter
    SetPosition(0, 0);

    m_matter.SetMesh(g_pTileMesh);
    m_matter.SetTexture(g_pRedTileTex);
    m_matter.SetMaterial(g_pDiffuseMaterial);

    m_nOwner = SIDE_1;
    m_nType  = TILE_TYPE_NORMAL;
}

Tile::~Tile()
{

}

void Tile::Register(Scene* pScene)
{
    pScene->AddMatter(&m_matter);
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
    
    // This function needs to be updated so that the proper texture is used,
    // not just the "Normal" texture.
    m_matter.SetTexture((m_nOwner == SIDE_1) ? g_pRedTileTex : g_pBlueTileTex);
}

void Tile::SetPosition(int nX,
                       int nZ)
{   
    m_nX = nX;
    m_nZ = nZ;

    m_matter.SetPosition(nX * TILE_WIDTH, 0.0f, nZ * TILE_HEIGHT);
}
