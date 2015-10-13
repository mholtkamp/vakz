#ifndef TILE_H
#define TILE_H

#include "Matter.h"
#include "Scene.h"
#include "Constants.h"
#include "Pawn.h"
#include "Timer.h"

class Tile
{
public:

    Tile();
    ~Tile();

    void Register(Scene* pScene);

    int GetOwner();

    int GetTileType();

    Pawn* GetPawn();

    void SetOwner(int nOwner);
    
    void SetPosition(int nX,
                     int nZ);

    void SetPawn(Pawn* pPawn);

    void SetGame(void* pGame);

    void SetType(int nType);

    void Update();

    void UpdateTexture();

    void RestoreOwnership();

    int m_nX;
    int m_nZ;
    int m_nOwner;
    int m_nType;

private:

    Pawn* m_pPawn;
    void* m_pGame;

#ifdef SM_CLIENT
    Matter m_matter;
#else
    Timer m_timer;
#endif
};

#endif
