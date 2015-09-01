#include "Pawn.h"
#include "Resources.h"
#include "Constants.h"
#include "Game.h"
#include "NetworkManager.h"

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
    m_nX = nX;
    m_nZ = nZ;

#ifdef SM_CLIENT
    m_matter.SetPosition(m_nX * TILE_WIDTH, 0.0f, m_nZ * TILE_HEIGHT);
    reinterpret_cast<Game*>(m_pGame)->SendPosition(m_nSide, m_nX, m_nZ);
#endif

}

void Pawn::UpdatePosition(int nX,
                          int nZ)
{
    m_nX = nX;
    m_nZ = nZ;

#ifdef SM_CLIENT
    m_matter.SetPosition(m_nX * TILE_WIDTH, 0.0f, m_nZ * TILE_HEIGHT);
#endif
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
        if (pGame->m_arTiles[nNewX][nNewZ].GetOwner() == m_nSide)
        {
            SetPosition(nNewX, nNewZ);
        }
    }
}

void Pawn::Damage(int nDamage)
{
    m_nHealth -= nDamage;
}

void Pawn::Heal(int nHealth)
{
    m_nHealth += nHealth;
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
        SetPosition(1, 1);

#ifdef SM_CLIENT
        m_matter.SetRotation(0.0f, -90.0f, 0.0f);
        m_matter.SetMesh(g_pMageMesh);
        m_matter.SetTexture(g_pRedMageTex);
        m_matter.SetMaterial(g_pDiffuseMaterial);
#endif
    }
    else
    {
        m_nX = 4;
        m_nZ = 1;

        SetPosition(4, 1);

#ifdef SM_CLIENT
        m_matter.SetRotation(0.0f, 90.0f, 0.0f);
        m_matter.SetMesh(g_pMageMesh);
        m_matter.SetTexture(g_pBlueMageTex);
        m_matter.SetMaterial(g_pDiffuseMaterial);
#endif
    }
}

void Pawn::SetGame(void* pGame)
{
    m_pGame   = pGame;
}
