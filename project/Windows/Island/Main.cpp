#include "Vakz.h"
#include "Quad.h"
#include "VGL.h"
#include "Matrix.h"
#include "ResourceLibrary.h"
#include "DiffuseMaterial.h"
#include "DirectionalLight.h"

#include <stdio.h>

#define ROT_SPEED 30.0f
#define MOVE_SPEED 5.0f

int main()
{
    SetWindowSize(1024,768);
    Initialize();
    Scene* pTestScene = new Scene();
    SetScene(pTestScene);

    // Create camera
    Camera* pCamera = new Camera();
    pCamera->SetPosition(0.0f, 10.0f, 10.0f);
    pCamera->SetRotation(-45.0f, 0.0f, 0.0f);
    pTestScene->SetCamera(pCamera);

    // Create ResourceLibrary
    ResourceLibrary* pLibrary = new ResourceLibrary();

    // Create diffuse material
    DiffuseMaterial* pCubeMat = new DiffuseMaterial();
    pCubeMat->SetColor(0.3f, 0.3f, 0.8f, 1.0f);

    // Create Matter
    Matter* pTestCube = new Matter();
    pTestCube->SetStaticMesh(reinterpret_cast<StaticMesh*>(pLibrary->GetPrimitive(PRIMITIVE_CUBE)));
    pTestCube->SetMaterial(pCubeMat);

    Matter* pTestCube2 = new Matter();
    pTestCube2->SetStaticMesh(reinterpret_cast<StaticMesh*>(pLibrary->GetPrimitive(PRIMITIVE_CUBE)));
    pTestCube2->SetMaterial(pCubeMat);
    pTestCube2->SetPosition(-5.0f, 0.0f, 0.0f);

    // Create sun
    DirectionalLight* pSun = new DirectionalLight();
    pSun->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    pSun->SetIntensity(1.0f);
    pSun->SetDirectionVector(3.0f, -1.0f, -2.0f);
    pTestScene->AddLight(pSun);

    // Add the test cube to the scene
    pTestScene->AddMatter(pTestCube);
    pTestScene->AddMatter(pTestCube2);

    unsigned long unStart = GetTickCount();
    unsigned long unEnd = GetTickCount();
    float fSeconds = 0.0;
    float fZ = 10.0f;
    float fY = 0.0f;
    float fX = 0.0f;
    float fRotX = 0.0f;
    float fRotY = 0.0f;
    float fRotZ = 0.0f;
    int nLock = 0;
    float fCube2Rot = 0.0f;


    while (1)
    {
        unEnd = GetTickCount();

        fSeconds = static_cast<float>(unEnd - unStart)/1000.0f;

        // Update
        
        // Rotate camera
        if (GetAsyncKeyState(VK_UP))
        {
            fRotX += fSeconds * ROT_SPEED;
        }
        if (GetAsyncKeyState(VK_DOWN))
        {
            fRotX -= fSeconds * ROT_SPEED;
        }
        if (GetAsyncKeyState(VK_RIGHT))
        {
            fRotY -= fSeconds * ROT_SPEED;
        }
        if (GetAsyncKeyState(VK_LEFT))
        {
            fRotY += fSeconds * ROT_SPEED;
        }

        // Rotate 
        if (GetAsyncKeyState('W'))
        {
            fZ -= fSeconds * MOVE_SPEED;
        }
        if (GetAsyncKeyState('S'))
        {
            fZ += fSeconds * MOVE_SPEED;
        }
        if (GetAsyncKeyState('A'))
        {
            fX -= fSeconds * MOVE_SPEED;
        }
        if (GetAsyncKeyState('D'))
        {
            fX += fSeconds * MOVE_SPEED;
        }
        if (GetAsyncKeyState(VK_SPACE))
        {
            fY += fSeconds * MOVE_SPEED;
        }
        if (GetAsyncKeyState(VK_CONTROL))
        {
            fY -= fSeconds * MOVE_SPEED;
        }

        // Camera Lock
        if (GetAsyncKeyState('L'))
        {
            nLock = 1;
        }
        if (GetAsyncKeyState('U'))
        {
            nLock = 0;
        }

        pCamera->SetPosition(fX, fY, fZ);

        if (nLock == 0)
        {
            pCamera->SetRotation(fRotX, fRotY, fRotZ);
        }
        else
        {
            pCamera->LookAt(0.0f, 0.0f, 0.0f);
        }
        
        fCube2Rot += fSeconds * ROT_SPEED;
        pTestCube2->SetRotation(fCube2Rot, 0.0f, 0.0f);

        unStart = GetTickCount();
        Render();
    }
    
    delete pSun;
    delete pTestCube;
    delete pTestCube2;
    delete pCamera;
    delete pCubeMat;
    delete pLibrary;
    delete pTestScene;

    printf("Exiting.\n");
}
