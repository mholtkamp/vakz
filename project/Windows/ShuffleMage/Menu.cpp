#include "Menu.h"
#include "Vakz.h"
#include "VInput.h"

#define MAIN_BUTTON_X     0.55f
#define MAIN_BUTTON_WIDTH 0.45f
#define MAIN_BUTTON_HEIGHT 0.15f

Menu::Menu()
{
    // Setup Login State
    m_btLogin.SetRect(0.1f, 0.2f, 0.28f, 0.15f);
    m_btLogin.SetTextString("Login");
    m_btLogin.AddToScene(m_scene);

    m_btRegister.SetRect(0.48f, 0.2f, 0.42f, 0.15f);
    m_btRegister.SetTextString("Register");
    m_btRegister.AddToScene(m_scene);

    m_tfUsername.SetRect(0.25f, 0.75, 0.65f, 0.15f);
    m_tfUsername.SetMaxSize(16);
    m_tfUsername.SetTextScale(0.85f, 1.0f);
    m_tfUsername.AddToScene(m_scene);

    m_tfPassword.SetRect(0.25f, 0.53f, 0.65f, 0.15f);
    m_tfPassword.SetMaxSize(16);
    m_tfPassword.SetTextScale(0.85f, 1.0f);
    m_tfPassword.AddToScene(m_scene);

    // Setup Main State
    m_btPlay.SetRect(MAIN_BUTTON_X, 0.8f, MAIN_BUTTON_WIDTH, MAIN_BUTTON_HEIGHT);
    m_btPlay.SetTextString("Play");
    m_btPlay.AddToScene(m_scene);

    m_btDeck.SetRect(MAIN_BUTTON_X, 0.6f, MAIN_BUTTON_WIDTH, MAIN_BUTTON_HEIGHT);
    m_btDeck.SetTextString("Deck");
    m_btDeck.AddToScene(m_scene);

    m_btShop.SetRect(MAIN_BUTTON_X, 0.4f, MAIN_BUTTON_WIDTH, MAIN_BUTTON_HEIGHT);
    m_btShop.SetTextString("Shop");
    m_btShop.AddToScene(m_scene);

    m_btPractice.SetRect(MAIN_BUTTON_X, 0.2f, MAIN_BUTTON_WIDTH, MAIN_BUTTON_HEIGHT);
    m_btPractice.SetTextString("Practice");
    m_btPractice.AddToScene(m_scene);

    m_btSettings.SetRect(MAIN_BUTTON_X, 0.0f, MAIN_BUTTON_WIDTH, MAIN_BUTTON_HEIGHT);
    m_btSettings.SetTextString("Settings");
    m_btSettings.AddToScene(m_scene);

    m_btLogout.SetRect(-1.0f, -0.6f, 0.35f, MAIN_BUTTON_HEIGHT);
    m_btLogout.SetTextString("Logout");
    m_btLogout.AddToScene(m_scene);

    m_btExit.SetRect(-1.0f, -0.8f, 0.35f, MAIN_BUTTON_HEIGHT);
    m_btExit.SetTextString("Exit");
    m_btExit.AddToScene(m_scene);
}

Menu::~Menu()
{

}

void Menu::RegisterScene()
{
    SetScene(&m_scene);
}

void Menu::SetState(int nState)
{
    if (nState >= 0 &&
        nState < MENU_MAX_STATES)
    {
        m_nState = nState;
    }

    EnableLogin(0);
    EnableMain(0);
    EnableQueue(0);
    EnableDeck(0);
    EnableShop(0);

    if (m_nState == MENU_STATE_LOGIN)
    {
        EnableLogin(1);
    }
    else if (m_nState == MENU_STATE_MAIN)
    {
        EnableMain(1);
    }
    else if (m_nState == MENU_STATE_QUEUE)
    {
        EnableQueue(1);
    }
    else if (m_nState == MENU_STATE_DECK)
    {
        EnableDeck(1);
    }
    else if (m_nState == MENU_STATE_SHOP)
    {
        EnableShop(1);
    }
}


void Menu::Update()
{
    if (m_nState == MENU_STATE_LOGIN)
    {
        UpdateLogin();
    }
    else if (m_nState == MENU_STATE_MAIN)
    {
        UpdateMain();
    }
    else if (m_nState == MENU_STATE_QUEUE)
    {
        UpdateQueue();
    }
    else if (m_nState == MENU_STATE_DECK)
    {
        UpdateDeck();
    }
    else if (m_nState == MENU_STATE_SHOP)
    {
        UpdateShop();
    }
}

void Menu::UpdateLogin()
{
    float fX = 0.0f;
    float fY = 0.0f;
    int nMouseDown = IsPointerDown();
    GetPointerPositionNormalized(fX, fY);
    
    m_tfUsername.Update(nMouseDown,
                        fX,
                        fY);
    m_tfPassword.Update(nMouseDown,
                        fX,
                        fY);
}

void Menu::UpdateMain()
{

}

void Menu::UpdateQueue()
{

}

void Menu::UpdateDeck()
{

}

void Menu::UpdateShop()
{

}

void Menu::EnableLogin(int nEnable)
{
    m_btLogin.SetVisible(nEnable);
    m_btRegister.SetVisible(nEnable);
    m_tfUsername.SetVisible(nEnable);
    m_tfPassword.SetVisible(nEnable);
}

void Menu::EnableMain(int nEnable)
{
    m_btPlay.SetVisible(nEnable);
    m_btDeck.SetVisible(nEnable);
    m_btShop.SetVisible(nEnable);
    m_btPractice.SetVisible(nEnable);
    m_btSettings.SetVisible(nEnable);
    m_btLogout.SetVisible(nEnable);
    m_btExit.SetVisible(nEnable);
}

void Menu::EnableQueue(int nEnable)
{
    m_btCancel.SetVisible(nEnable);
}

void Menu::EnableDeck(int nEnable)
{
    m_btEquip.SetVisible(nEnable);
    m_btSave.SetVisible(nEnable);
    m_btBackDeck.SetVisible(nEnable);
}

void Menu::EnableShop(int nEnable)
{
    m_btBackShop.SetVisible(nEnable);
    m_btBuy.SetVisible(nEnable);
}
