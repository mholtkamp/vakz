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
    Game(int nSide);
    ~Game();

    void Update();

    void RegisterScene();

    void SetupCamera();

    Tile m_arTiles[GRID_WIDTH][GRID_HEIGHT];

private:

    int m_nPlayerSide;

    Scene m_scene;
    Camera m_camera;
    DirectionalLight m_light;
    Mage m_arMages[NUM_MAGES];
    

    int m_nTouchDown;
    int m_nJustTouched;
    int m_nJustUp;
    float m_fTouchDownX;
    float m_fTouchDownY;
    float m_fTouchUpX;
    float m_fTouchUpY;

};

#endif