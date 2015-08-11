#ifndef SM_MENU_H
#define SM_MENU_H

#include "Button.h"
#include "Scene.h"

enum MenuStateEnum
{
    MENU_STATE_LOADING  = 0,
    MENU_STATE_MAIN     = 1,
    MENU_STATE_REGISTER = 2,
    MENU_STATE_LOGIN    = 3,
    MENU_STATE_DECK     = 4,
    MENU_STATE_QUEUE    = 5,
    MENU_STATE_SHOP     = 6,
    MENU_STATE_SETTINGS = 7,
    MENU_MAX_STATES     = 8
};

class Menu
{
public:

    Menu();
    ~Menu();

    void Update();

    void RegisterScene();

    void SetState(int nState);

private:

    void UpdateLogin();
    void UpdateMain();
    void UpdateQueue();
    void UpdateDeck();
    void UpdateShop();

    void EnableLogin(int nEnable);
    void EnableMain(int nEnable);
    void EnableQueue(int nEnable);
    void EnableDeck(int nEnable);
    void EnableShop(int nEnable);

    int m_nState;

    Scene m_scene;

    // Login State
    Button m_btLogin;
    Button m_btRegister;

    // Main State
    Button m_btPlay;
    Button m_btDeck;
    Button m_btShop;
    Button m_btPractice;
    Button m_btSettings;
    Button m_btLogout;
    Button m_btExit;

    // Queue
    Button m_btCancel;

    // Deck State
    Button m_btEquip;
    Button m_btSave;
    Button m_btBackDeck;

    // Shop State
    Button m_btBackShop;
    Button m_btBuy;

    // Settings State
    // TBD

};

#endif