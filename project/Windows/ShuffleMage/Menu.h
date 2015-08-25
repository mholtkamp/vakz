#ifndef SM_MENU_H
#define SM_MENU_H

#include "Button.h"
#include "TextField.h"
#include "Scene.h"
#include "PlayerData.h"

// Messages
#include "MsgLogin.h"
#include "MsgRegister.h"

enum MenuStateEnum
{
    MENU_STATE_LOADING    = 0,
    MENU_STATE_MAIN       = 1,
    MENU_STATE_REGISTER   = 2,
    MENU_STATE_LOGIN      = 3,
    MENU_STATE_DECK       = 4,
    MENU_STATE_QUEUE      = 5,
    MENU_STATE_SHOP       = 6,
    MENU_STATE_SETTINGS   = 7,
    MENU_STATE_WAIT_LOGIN = 8,
    MENU_STATE_WAIT_BUY   = 9,
    MENU_MAX_STATES       = 10
};

enum MenuEnum
{
    LOGIN_STATUS_NONE                = 0,
    LOGIN_STATUS_OK                  = 1,
    LOGIN_STATUS_INVALID_PASS        = 2,
    LOGIN_STATUS_USER_NOT_FOUND      = 3,
    LOGIN_STATUS_REGISTRATION_FAILED = 4
};

class Menu
{
public:

    Menu();
    ~Menu();

    void Update();

    void RegisterScene();

    void SetState(int nState);

    Scene* GetScene();

    void SetPlayerData(PlayerData* pPlayerData);

    void* m_pNetworkManager;
    void* m_pGame;

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

    int m_nTouchDown;
    int m_nJustTouched;

    int m_nLoginStatus;

    Scene m_scene;

    // Login State
    Button m_btLogin;
    Button m_btRegister;
    TextField m_tfUsername;
    TextField m_tfPassword;
    Text m_txUsername;
    Text m_txPassword;

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

    // Messages
    MsgLogin    m_msgLogin;
    MsgRegister m_msgRegister;

};

#endif