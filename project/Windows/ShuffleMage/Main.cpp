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
#include "Menu.h"

Scene sceneMenu;
Scene sceneBattle;
Menu* pMenu;
char* pTestBuffer = 0;

int nGameState;

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

    SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    pMenu = new Menu();
    
    pMenu->RegisterScene();
    pMenu->SetState(MENU_STATE_LOGIN);

    Text MsgText;
    MsgText.SetPosition(0.0f, 0.0f);
    MsgText.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    MsgText.SetText("Waiting");
    pMenu->GetScene()->AddGlyph(&MsgText);

    Socket serverSocket;
    Socket* servclientSocket = 0;

    Socket clientSocket;

    char pBuffer[256] = {0};


    int nServer    = 1;
    int nConnected = 0;

    if (nServer != 0)
    {
        LogDebug("Opening Server Socket");
        serverSocket.Open(0, 27772);
    }
    else
    {
        LogDebug("Connecting to server.");
        clientSocket.Connect(0, "192.168.2.4", 27772);
    }

    char arWord[4] = {0};

    while ((GetStatus() & VAKZ_QUIT) == 0)
    {

        Update();
        pMenu->Update();
        Render();

        if (nServer)
        {
            if (nConnected == 0)
            {
                //LogDebug("Waiting on Connection");
                servclientSocket = serverSocket.Accept();
                if (servclientSocket != 0)
                {
                    LogDebug("Connection received!");
                    nConnected = 1;
                }
            }
            else
            {
                int nBytes = servclientSocket->Receive(pBuffer, 256);
                if (nBytes > 0)
                {
                    MsgText.SetText(pBuffer);
                    memset(pBuffer, 0, 256);
                }
            }
        }

        if (nServer == 0)
        {
            if (nConnected == 0)
            {
                clientSocket.Send("HINEIL", sizeof("HINEIL"));
                nConnected = 1;
            }
        }

    }

    exit(0);
}
