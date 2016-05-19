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
    pScene->AddActor(&m_matter);
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
    int i = 0;
    ServerGame* pTheGame = reinterpret_cast<ServerGame*>(m_pGame);

    // Check if the owned pawn is a mage of the opposite side.
    // If so, push back the mage.
    Mage* pMage = reinterpret_cast<ServerGame*>(m_pGame)->GetMage(!nOwner);

    if (pMage == m_pPawn)
    {
        if (nOwner == SIDE_2)
        {
            for (i = m_nX; i >= 0; i--)
            {
                if (pTheGame->m_arTiles[i][m_nZ].GetPawn()  == 0 &&
                    pTheGame->m_arTiles[i][m_nZ].GetOwner() == SIDE_1)
                {
                    pMage->SetPosition(i, m_nZ);
                    break;
                }
            }

            if (i == -1)
            {
                // No availble tile to move back to...
                m_nOwner = !m_nOwner;
            }
        }
        else
        {
            for (i = m_nX; i < GRID_WIDTH; i++)
            {
                if (pTheGame->m_arTiles[i][m_nZ].GetPawn()  == 0 &&
                    pTheGame->m_arTiles[i][m_nZ].GetOwner() == SIDE_2)
                {
                    pMage->SetPosition(i, m_nZ);
                    break;
                }
            }

            if (i == GRID_WIDTH)
            {
                // No availble tile to move back to...
                m_nOwner = !m_nOwner;
            }
        }
    }

    // Send Tile update message
    pTheGame->UpdateTile(m_nX, m_nZ, m_nOwner, m_nType);

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
#if defined (SM_SERVER)
    // Check if a pawn left cracked tile
    if (m_pPawn != 0 &&
        pPawn   == 0 &&
        m_nType ==TILE_TYPE_CRACKED)
    {
        SetType(TILE_TYPE_EMPTY);
    }
#endif

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
    m_timer.Start();
#else
    UpdateTexture();
#endif
}

void Tile::UpdateTexture()
{
#if defined (SM_CLIENT)
    // This function needs to be updated so that the proper texture is used,
    // not just the "Normal" texture.
    m_matter.SetTexture((m_nOwner == SIDE_1) ? g_pRedTileTex : g_pBlueTileTex);

    if (m_nOwner == SIDE_1)
    {
        switch(m_nType)
        {
        case TILE_TYPE_NORMAL:
            m_matter.SetTexture(g_pRedTileTex);
            break;
        case TILE_TYPE_CRACKED:
            m_matter.SetTexture(g_pRedCrackedTileTex);
            break;
        case TILE_TYPE_EMPTY:
            m_matter.SetTexture(g_pRedEmptyTileTex);
            break;
        default:
            m_matter.SetTexture(g_pRedTileTex);
            break;
        }
    }
    else
    {
        switch(m_nType)
        {
        case TILE_TYPE_NORMAL:
            m_matter.SetTexture(g_pBlueTileTex);
            break;
        case TILE_TYPE_CRACKED:
            m_matter.SetTexture(g_pBlueCrackedTileTex);
            break;
        case TILE_TYPE_EMPTY:
            m_matter.SetTexture(g_pBlueEmptyTileTex);
            break;
        default:
            m_matter.SetTexture(g_pBlueTileTex);
            break;
        }
    }
#endif
}

void Tile::RestoreOwnership()
{
    if (m_nX < GRID_WIDTH/2)
    {
        SetOwner(SIDE_1);
    }
    else
    {
        SetOwner(SIDE_2);
    }
}

void Tile::Update()
{

#if defined (SM_SERVER)
    float fTime = 0.0f;

    if (m_nType != TILE_TYPE_NORMAL)
    {
        m_timer.Stop();
        fTime = m_timer.Time();

        if (fTime >= TILE_RESET_TIME)
        {
            SetType(TILE_TYPE_NORMAL);
        }
    }
#endif

}
