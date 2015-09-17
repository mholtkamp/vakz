#ifndef GAME_H
#define GAME_H

#include "Scene.h"
#include "Camera.h"
#include "Constants.h"
#include "Mage.h"
#include "Card.h"
#include "Tile.h"
#include "HUD.h"
#include "Activation.h"

#include "MsgPosition.h"
#include "MsgCard.h"

class Game
{
public:

    enum GameEnum
    {
        MAGE_1    = 0,
        MAGE_2    = 1,
        NUM_MAGES = 2,

        INDEX_LEFT  = 0,
        INDEX_RIGHT = 1
    };

    Game();
    Game(int nSide);
    ~Game();

    void Construct();

    void Update();

    void RegisterScene();

    void SetupCamera();

    void RotateHandLeft();

    void RotateHandRight();

    void RemoveCardFromHand(int nIndex);

    void SendPosition(int nPlayer,
                      int nX,
                      int nZ);

    void UpdatePosition(int nPlayer,
                        int nX,
                        int nZ);

    void UpdateHealth(int nPlayer,
                      int nHealth);

    void UseCard(int nCard,
                 int nCaster);

    void AddCardsToHand(int* arCards);

    Mage* GetMage(int nIndex);

    Tile m_arTiles[GRID_WIDTH][GRID_HEIGHT];

    void* m_pNetworkManager;

    // Activations
    Activation* m_arActivations[MAX_ACTIVATIONS];
    Matter      m_arActMatters[MAX_ACTIVATIONS];

private:

    int m_nPlayerSide;

    Scene m_scene;
    Camera m_camera;
    DirectionalLight m_light;
    Mage m_arMages[NUM_MAGES];
    HUD m_hud;
    Card* m_arHand[HAND_SIZE];
    
    // Timers
    Timer m_timerMana;
    Timer m_timerDraw;

    // Touch/Gesture controls
    int m_nTouchDownLeft;
    int m_nJustTouchedLeft;
    int m_nJustUpLeft;
    float m_fTouchDownXLeft;
    float m_fTouchDownYLeft;
    float m_fTouchUpXLeft;
    float m_fTouchUpYLeft;

    int m_nTouchDownRight;
    int m_nJustTouchedRight;

    // Messages
    MsgPosition m_msgPosition;
    MsgCard     m_msgCard;
};

#endif