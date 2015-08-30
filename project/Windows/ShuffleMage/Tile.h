#ifndef TILE_H
#define TILE_H

#include "Matter.h"
#include "Scene.h"
#include "Constants.h"

class Tile
{
public:

    Tile();
    ~Tile();

    void Register(Scene* pScene);

    int GetOwner();

    int GetTileType();

    void SetOwner(int nOwner);
    
    void SetPosition(int nX,
                     int nZ);

private:

    int m_nX;
    int m_nZ;

    int m_nOwner;
    int m_nType;

    Matter m_matter;
};

#endif