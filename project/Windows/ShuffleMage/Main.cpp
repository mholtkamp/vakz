// Vakz includes
#include "Vakz.h"
#include "Scene.h"
#include "Quad.h"
#include "VGL.h"
#include "VInput.h"
#include "VMath.h"
#include "Log.h"
#include "Timer.h"
#include "Matrix.h"
#include "BoxCollider.h"
#include "ResourceLibrary.h"
#include "DiffuseMaterial.h"
#include "DirectionalLight.h"
#include "MeshCollider.h"
#include "Text.h"
#include "VSocket.h"

// C stdlib includes
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// OS includes
#if defined (ANDROID)
#include <android_native_app_glue.h>
#include <unistd.h>
#endif

// Shuffle Mage includes
#include "Constants.h"
#include "Menu.h"
#include "Game.h"
#include "NetworkManager.h"
#include "Resources.h"

Scene sceneMenu;
Scene sceneBattle;
Menu* pMenu = 0;
Game* pGame = 0;
NetworkManager* pNetworkManager = 0;

char* pTestBuffer = 0;

int nGameState = GAME_STATE_MENU;

#if defined (ANDROID)
void android_main(struct android_app* state)
{
    app_dummy();
    Initialize(state);
#else
int main()
{
    SetWindowSize(1024,768);
    Initialize();
#endif

    LogDebug("Loading resources...");
    LoadResources();
    LogDebug("Finished Loading resources");

    SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    pMenu = new Menu();
    pNetworkManager = new NetworkManager();

    pMenu->m_pNetworkManager = pNetworkManager;
    pNetworkManager->m_pMenu = pMenu;
    
    pMenu->RegisterScene();
    pMenu->SetState(MENU_STATE_LOGIN);

    while ((GetStatus() & VAKZ_QUIT) == 0)
    {

        Update();
        if (nGameState == GAME_STATE_MENU)
        {
            pMenu->Update();
        }
        else if (nGameState == GAME_STATE_GAME)
        {
            pGame->Update();
        }
        
        pNetworkManager->Update();
        Render();

    }

    exit(0);
}

void SetGameState(int nState)
{
    nGameState = nState;
}

void SetGame(void* pNewGame)
{
    pGame =  reinterpret_cast<Game*>(pNewGame);
}
