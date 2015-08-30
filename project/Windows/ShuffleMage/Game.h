#ifndef GAME_H
#define GAME_H

#include "Scene.h"
#include "Camera.h"
#include "Constants.h"
#include "Mage.h"
#include "Tile.h"

class Game
{
public:

    enum GameEnum
    {
        MAGE_1    = 0,
        MAGE_2    = 1,
        NUM_MAGES = 2
    };

    Game();
    ~Game();

    void Update();

    void SetScene();

private:

    Scene m_scene;
    Mage m_arMages[NUM_MAGES];
    Tile m_arTiles[GRID_HEIGHT][GRID_WIDTH];


};

#endif