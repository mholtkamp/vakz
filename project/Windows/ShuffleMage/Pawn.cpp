#include "Pawn.h"
#include "Resources.h"
#include "Constants.h"
#include "Game.h"
#include "NetworkManager.h"
#include "Log.h"
#include "Tile.h"

#if defined SM_SERVER
#include "../ShuffleMageServer/ServerGame.h"
#endif

#define PAWN_DEFAULT_HEALTH 100
#define PAWN_DEFAULT_SPEED 0.3f

Pawn::Pawn()
{
    Construct();
}

Pawn::Pawn(int nSide)
{
    Construct();
    SetSide(nSide);
}

Pawn::~Pawn()
{

}

void Pawn::Construct()
{
    m_nSide = SIDE_1;
    m_nX = 0;
    m_nZ = 0;

    m_nVisible = 1;
    m_nHealth  = PAWN_DEFAULT_HEALTH;
    m_fSpeed   = PAWN_DEFAULT_SPEED;
}

void Pawn::Register(Scene* pScene)
{
#ifdef SM_CLIENT
    pScene->AddMatter(&m_matter);
#endif
}

void Pawn::SetPosition(int nX,
                       int nZ)
{


#ifdef SM_CLIENT
    m_matter.SetPosition(nX * TILE_WIDTH, 0.0f, nZ * TILE_HEIGHT);
    reinterpret_cast<Game*>(m_pGame)->SendPosition(m_nSide, nX, nZ);
    reinterpret_cast<Game*>(m_pGame)->m_arTiles[m_nX][m_nZ].SetPawn(0);
    reinterpret_cast<Game*>(m_pGame)->m_arTiles[nX][nZ].SetPawn(this);
#else
    reinterpret_cast<ServerGame*>(m_pGame)->m_arTiles[m_nX][m_nZ].SetPawn(0);
    reinterpret_cast<ServerGame*>(m_pGame)->m_arTiles[nX][nZ].SetPawn(this);
    reinterpret_cast<ServerGame*>(m_pGame)->UpdatePositionAll(m_nSide, nX, nZ);
#endif

    m_nX = nX;
    m_nZ = nZ;
}

void Pawn::UpdatePosition(int nX,
                          int nZ)
{
#ifdef SM_CLIENT
    m_matter.SetPosition(nX * TILE_WIDTH, 0.0f, nZ * TILE_HEIGHT);
    reinterpret_cast<Game*>(m_pGame)->m_arTiles[m_nX][m_nZ].SetPawn(0);
    reinterpret_cast<Game*>(m_pGame)->m_arTiles[nX][nZ].SetPawn(this);
#else
    reinterpret_cast<ServerGame*>(m_pGame)->m_arTiles[m_nX][m_nZ].SetPawn(0);
    reinterpret_cast<ServerGame*>(m_pGame)->m_arTiles[nX][nZ].SetPawn(this);
#endif

    // Assign these member values afterwards.
    m_nX = nX;
    m_nZ = nZ;
}

void Pawn::Move(int nX,
                int nZ)
{
    int nNewX = m_nX + nX;
    int nNewZ = m_nZ + nZ;

#ifdef SM_CLIENT
        Game* pGame = reinterpret_cast<Game*>(m_pGame);
#endif

#ifdef SM_SERVER
        ServerGame* pGame = reinterpret_cast<ServerGame*>(m_pGame);
#endif

    if (nNewX >= 0          &&
        nNewX < GRID_WIDTH  &&
        nNewZ >= 0          &&
        nNewZ < GRID_HEIGHT)
    {
        // Location is in grid boundaries
        // Now check if the new tile is owned.
        if (pGame->m_arTiles[nNewX][nNewZ].GetOwner()    == m_nSide &&
            pGame->m_arTiles[nNewX][nNewZ].GetPawn()     == 0       &&
            pGame->m_arTiles[nNewX][nNewZ].GetTileType() != TILE_TYPE_EMPTY)
        {
            // Set the new pawn position
            SetPosition(nNewX, nNewZ);
        }
    }
}

void Pawn::Damage(int nDamage)
{
    m_nHealth -= nDamage;
    
#if defined (SM_SERVER)
        reinterpret_cast<ServerGame*>(m_pGame)->UpdateHealth(m_nSide);
#endif
}

void Pawn::Heal(int nHealth)
{
    m_nHealth += nHealth;
#if defined (SM_SERVER)
        reinterpret_cast<ServerGame*>(m_pGame)->UpdateHealth(m_nSide);
#endif
}

void Pawn::SetVisible(int nVisible)
{
    m_nVisible = nVisible;
    // need to set matter visibility too once thats implemented.
}

void Pawn::SetSpeed(float fSpeed)
{
    m_fSpeed = fSpeed;
}

void Pawn::SetSide(int nSide)
{
    m_nSide = nSide;
    if (nSide == 0)
    {
        UpdatePosition(1, 1);

#ifdef SM_CLIENT
        m_matter.SetRotation(0.0f, -90.0f, 0.0f);
        m_matter.SetTexture(g_pRedMageTex);
        m_matter.SetMaterial(g_pDiffuseMaterial);
#endif
    }
    else
    {
        m_nX = 4;
        m_nZ = 1;

        UpdatePosition(4, 1);

#ifdef SM_CLIENT
        m_matter.SetRotation(0.0f, 90.0f, 0.0f);
        m_matter.SetTexture(g_pBlueMageTex);
        m_matter.SetMaterial(g_pDiffuseMaterial);
#endif
    }
}

void Pawn::SetGame(void* pGame)
{
    m_pGame   = pGame;
}

int Pawn::GetHealth()
{
    return m_nHealth;
}

void Pawn::SetHealth(int nHealth)
{
    m_nHealth = nHealth;

#if defined (SM_SERVER)
        reinterpret_cast<ServerGame*>(m_pGame)->UpdateHealth(m_nSide);
#endif
}

void Pawn::GetPosition(int& nX,
                       int& nZ)
{
    nX = m_nX;
    nZ = m_nZ;
}
