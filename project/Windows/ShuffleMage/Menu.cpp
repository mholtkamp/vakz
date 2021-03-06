#include "Menu.h"
#include "Vakz.h"
#include "VInput.h"
#include "Log.h"
#include "Toast.h"

#include "NetworkManager.h"
#include "Constants.h"

#define MAIN_BUTTON_X     0.55f
#define MAIN_BUTTON_WIDTH 0.50f
#define MAIN_BUTTON_HEIGHT 0.15f

#define BLUR_FACTOR 1
#define BLUR_MIN_SAMPLE_DISTANCE 1
#define BLUR_MAX_SAMPLE_DISTANCE 100
#define BLUR_TIME 0.4f
#define BLUR_FOREWARD 0
#define BLUR_BACKWARD 1

Menu::Menu()
{
    m_pNetworkManager = 0;
    m_pGame           = 0;

    m_nState = MENU_STATE_LOGIN;
    m_nTouchDown   = 0;
    m_nJustTouched = 0;
    m_nLoginStatus = LOGIN_STATUS_NONE;
    m_pPlayerData  = 0;

    // Setup Login State
    m_btLogin.SetRect(0.1f, 0.2f, 0.28f, 0.15f);
    m_btLogin.SetTextString("Login");
    m_btLogin.AddToScene(m_scene);
    m_btLogin.SetVisible(1);

    m_btRegister.SetRect(0.48f, 0.2f, 0.42f, 0.15f);
    m_btRegister.SetTextString("Register");
    m_btRegister.AddToScene(m_scene);
    m_btRegister.SetVisible(1);

    m_tfUsername.SetRect(0.25f, 0.75f, 0.65f, 0.15f);
    m_tfUsername.SetMaxSize(16);
    m_tfUsername.SetTextScale(0.85f, 1.0f);
    m_tfUsername.AddToScene(m_scene);

    m_tfPassword.SetRect(0.25f, 0.53f, 0.65f, 0.15f);
    m_tfPassword.SetMaxSize(16);
    m_tfPassword.SetTextScale(0.85f, 1.0f);
    m_tfPassword.AddToScene(m_scene);

    m_txUsername.SetText("User:");
    m_txUsername.SetPosition(0.0f, 0.8f);
    m_txUsername.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    m_scene.AddGlyph(&m_txUsername);

    m_txPassword.SetText("Pass:");
    m_txPassword.SetPosition(0.0f, 0.58f);
    m_txPassword.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    m_scene.AddGlyph(&m_txPassword);

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

    m_btLogout.SetRect(-1.005f, -0.6f, 0.35f, MAIN_BUTTON_HEIGHT);
    m_btLogout.SetTextString("Logout");
    m_btLogout.AddToScene(m_scene);

    m_btExit.SetRect(-1.005f, -0.8f, 0.35f, MAIN_BUTTON_HEIGHT);
    m_btExit.SetTextString("Exit");
    m_btExit.AddToScene(m_scene);

    m_scene.AddEffect(&m_blur);
    m_blur.SetBlurFactor(1);
    m_blur.SetSampleDistance(1);
    m_blur.SetEnable(1);
    m_timerBlur.Start();
    m_nBlurDirection = BLUR_BACKWARD;
    m_nBlurOut = 0;
    m_nNextState = MENU_STATE_LOGIN;
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
        if (m_nBlurOut == 0)
        {
            m_nBlurOut = 1;
            m_nBlurDirection = BLUR_FOREWARD;
            m_blur.SetEnable(1);
            m_timerBlur.Start();
            m_nNextState = nState;
            return;
        }
        else
        {
            m_nBlurOut = 0;
            m_nState = nState;
        }
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

    m_timerBlur.Start();
    m_nBlurDirection = BLUR_BACKWARD;
    m_blur.SetEnable(1);
}


void Menu::Update()
{
    float fBlurTime = 0.0f;
    int nSampDist   = 1;

    // Do not blur out these states
    if (m_nBlurOut != 0 &&
        m_nState   == MENU_STATE_WAIT_LOGIN)
    {
        SetState(m_nNextState);
    }
    // Blur out the rest
    else if (m_nBlurOut != 0)
    {
        m_timerBlur.Stop();
        if (m_timerBlur.Time() >= BLUR_TIME)
        {
            SetState(m_nNextState);
        }
    }
    else
    {

        // Check if "just-touched"
        int nDown = IsPointerDown();
        if (m_nTouchDown == 0 &&
            nDown        != 0)
        {
            m_nJustTouched = 1;
        }
        else
        {
            m_nJustTouched = 0;
        }
        m_nTouchDown = nDown;

        // Handle back button, why not.
        if (IsKeyDown(VKEY_BACK))
        {
            HideSoftKeyboard();
        }

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
        else if (m_nState == MENU_STATE_WAIT_LOGIN)
        {
            if (m_nLoginStatus == LOGIN_STATUS_OK)
            {
                m_pPlayerData->PrintData();
                LogDebug("Login successful!");
                SetState(MENU_STATE_MAIN);
            }
            else if (m_nLoginStatus > LOGIN_STATUS_OK)
            {
                if (m_nLoginStatus == LOGIN_STATUS_REGISTRATION_FAILED)
                {
                    Toast("Registration failed");
                }
                else if (m_nLoginStatus == LOGIN_STATUS_INVALID_PASS)
                {
                    Toast("Invalid password");
                }
                else if (m_nLoginStatus == LOGIN_STATUS_USER_NOT_FOUND)
                {
                    Toast("Username not found");
                }

                m_nLoginStatus = LOGIN_STATUS_NONE;
                SetState(MENU_STATE_LOGIN);
                m_tfUsername.ClearText();
                m_tfPassword.ClearText();
            }
        }
    }

    // Update Blur effect
    m_timerBlur.Stop();
    fBlurTime = m_timerBlur.Time();

    if (m_blur.IsEnabled() != 0 &&
        fBlurTime < BLUR_TIME)
    {
        m_blur.SetBlurFactor(BLUR_FACTOR);
        if (m_nBlurDirection == BLUR_FOREWARD)
        {
            nSampDist = static_cast<int>(BLUR_MAX_SAMPLE_DISTANCE * (fBlurTime/BLUR_TIME));

            if (nSampDist < 1)
            {
                nSampDist = 1;
            }
            m_blur.SetSampleDistance(nSampDist);
        }
        else
        {
            nSampDist = static_cast<int>(BLUR_MAX_SAMPLE_DISTANCE - BLUR_MAX_SAMPLE_DISTANCE * (fBlurTime/BLUR_TIME));

            if (nSampDist < 1)
            {
                nSampDist = 1;
            }
            m_blur.SetSampleDistance(nSampDist);
        }   
    }
    else if (m_blur.IsEnabled() != 0 &&
             fBlurTime >= BLUR_TIME)
    {
        // Blur transition is finished. Disable effect.
        m_blur.SetEnable(0);
    }

}

void Menu::UpdateLogin()
{
    float fX = 0.0f;
    float fY = 0.0f;

    GetPointerPositionNormalized(fX, fY);

    m_tfUsername.Update(m_nJustTouched,
                        fX,
                        fY);
    m_tfPassword.Update(m_nJustTouched,
                        fX,
                        fY);
    
    if (m_nJustTouched != 0)
    {
        if (m_btLogin.IsTouched())
        {
            reinterpret_cast<NetworkManager*>(m_pNetworkManager)->Connect();
            SetState(MENU_STATE_WAIT_LOGIN);
            memcpy(m_msgLogin.m_arUser, m_tfUsername.GetText(), USER_BUFFER_SIZE);
            memcpy(m_msgLogin.m_arPass, m_tfPassword.GetText(), PASS_BUFFER_SIZE);
            reinterpret_cast<NetworkManager*>(m_pNetworkManager)->Send(m_msgLogin);
        }

        if (m_btRegister.IsTouched())
        {
            reinterpret_cast<NetworkManager*>(m_pNetworkManager)->Connect();
            SetState(MENU_STATE_WAIT_LOGIN);
            memcpy(m_msgRegister.m_arUser, m_tfUsername.GetText(), USER_BUFFER_SIZE);
            memcpy(m_msgRegister.m_arPass, m_tfPassword.GetText(), PASS_BUFFER_SIZE);
            reinterpret_cast<NetworkManager*>(m_pNetworkManager)->Send(m_msgRegister);
        }
    }
}

void Menu::UpdateMain()
{
    float fX = 0.0f;
    float fY = 0.0f;

    GetPointerPositionNormalized(fX, fY);

    if (m_nJustTouched != 0)
    {
        if (m_btPlay.IsTouched())
        {
            SetState(MENU_STATE_QUEUE);
            m_msgQueue.m_nQueueType = QUEUE_TYPE_SOLO;
            reinterpret_cast<NetworkManager*>(m_pNetworkManager)->Send(m_msgQueue);
        }
    }
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
    m_txUsername.SetVisible(nEnable);
    m_txPassword.SetVisible(nEnable);
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

Scene* Menu::GetScene()
{
    return &m_scene;
}

void Menu::SetPlayerData(PlayerData* pData)
{
    m_pPlayerData = pData;
    memcpy(m_pPlayerData->m_arUser, m_tfUsername.GetText(), USER_BUFFER_SIZE);
    memcpy(m_pPlayerData->m_arPass, m_tfPassword.GetText(), PASS_BUFFER_SIZE);
}

void Menu::SetLoginStatus(int nStatus)
{
    m_nLoginStatus = nStatus;
}
