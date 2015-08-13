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

// C stdlib includes
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// OS includes
#if defined (ANDROID)
#include <android_native_app_glue.h>
#endif

// Shuffle Mage includes
#include "Menu.h"

#define ROT_SPEED 70.0f
#define MOVE_SPEED 5.0f
#define THRESH 0.4f

#define FLOWER_BOX_SIZE 100.0f
#define FLOWER_POT_COUNT 100

Scene sceneMenu;
Scene sceneBattle;
Menu* pMenu;

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

    while ((GetStatus() & VAKZ_QUIT) == 0)
    {
        Update();
        pMenu->Update();
        Render();
    }

    exit(0);
}
