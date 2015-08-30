#include "Pawn.h"
#include "Resources.h"
#include "Constants.h"

#define PAWN_DEFAULT_HEALTH 100
#define PAWN_DEFAULT_SPEED 0.3f

Pawn::Pawn()
{
    m_nVisible = 1;
    m_nHealth  = PAWN_DEFAULT_HEALTH;
    m_fSpeed   = PAWN_DEFAULT_SPEED;
}

Pawn::Pawn(int nSide)
{

    Pawn();
    SetSide(nSide);
}

Pawn::~Pawn()
{

}

void Pawn::Register(Scene* pScene)
{
    pScene->AddMatter(&m_matter);
}

void Pawn::SetPosition(int nX,
                       int nZ)
{
    m_nX = nX;
    m_nZ = nZ;

    m_matter.SetPosition(m_nX * TILE_WIDTH, 0.0f, m_nZ * TILE_HEIGHT);
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
        m_matter.SetRotation(0.0f, 90.0f, 0.0f);
        m_matter.SetMesh(g_pMageMesh);
        m_matter.SetTexture(g_pBlueMageTex);
        m_matter.SetMaterial(g_pDiffuseMaterial);
    }
    else
    {
        m_nX = 4;
        m_nZ = 1;

        SetPosition(4, 1);
        m_matter.SetRotation(0.0f, -90.0f, 0.0f);
        m_matter.SetMesh(g_pMageMesh);
        m_matter.SetTexture(g_pRedMageTex);
        m_matter.SetMaterial(g_pDiffuseMaterial);
    }
}
