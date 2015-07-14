#include "Vakz.h"
#include "Scene.h"
#include "Quad.h"
#include "VGL.h"
#include "VInput.h"
#include "Timer.h"
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
    pCamera->SetProjectionType(Camera::CAMERA_PERSPECTIVE);
    pTestScene->SetCamera(pCamera);

    // Create ResourceLibrary
    ResourceLibrary* pLibrary = new ResourceLibrary();

    // Create diffuse material
    DiffuseMaterial* pCubeMat = new DiffuseMaterial();
    pCubeMat->SetColor(1.0f, 1.0f, 1.0f, 1.0f);

    DiffuseMaterial* pCubeMat2 = new DiffuseMaterial();
    pCubeMat2->SetColor(0.3f, 0.3f, 0.9f, 1.0f);

    // Create Matter
    Matter* pTestCube = new Matter();
    pTestCube->SetMesh(reinterpret_cast<StaticMesh*>(pLibrary->GetPrimitive(PRIMITIVE_CUBE)));
    pTestCube->SetMaterial(pCubeMat2);
    pTestCube->SetPosition(5.0f, 0.0f, 0.0f);

    Matter* pTestMonkey = new Matter();
    StaticMesh* pMonkeyMesh = new StaticMesh();
    pMonkeyMesh->Load("E:/Projects/vakz/project/Windows/Island/druid.obj");
    pTestMonkey->SetMesh(pMonkeyMesh);
    pTestMonkey->SetMaterial(pCubeMat);
    pTestMonkey->SetPosition(0.0f, 0.0f, 0.0f);

    Matter* pTestAnim = new Matter();
    AnimatedMesh* pAnimMesh = new AnimatedMesh();
    pAnimMesh->Load("E:\Projects\vakz\project\Windows\Island\Druid_AM\druid.amf");
    pTestAnim->SetMesh(pAnimMesh);
    pTestAnim->SetMaterial(pCubeMat);
    pTestMonkey->SetPosition(-3.5f, 0.0f, 0.0f);

    Texture* pTestTexture = new Texture();
    pTestTexture->LoadBMP("E:/Projects/vakz/project/Windows/Island/trueform_base_color.bmp");
    pTestMonkey->SetTexture(pTestTexture);

    // Create sun
    DirectionalLight* pSun = new DirectionalLight();
    pSun->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    pSun->SetDirectionVector(3.0f, -1.0f, -2.0f);
    pTestScene->AddLight(pSun);

    // Add the test cube to the scene
    pTestScene->AddMatter(pTestCube);
    pTestScene->AddMatter(pTestMonkey);
    pTestScene->AddMatter(pTestAnim);

    float fSeconds = 0.0f;
    float fZ = 10.0f;
    float fY = 0.0f;
    float fX = 0.0f;
    float fRotX = 0.0f;
    float fRotY = 0.0f;
    float fRotZ = 0.0f;
    int nLock = 0;
    float fCube2Rot = 0.0f;
    Timer timer;
    timer.Start();

    while ((GetStatus() & VAKZ_QUIT) == 0)
    {
        timer.Stop();

        fSeconds = timer.Time();

        // Update
        Update();

        // Rotate camera
        if (IsKeyDown(VKEY_UP))
        {
            fRotX += fSeconds * ROT_SPEED;
        }
        if (IsKeyDown(VKEY_DOWN))
        {
            fRotX -= fSeconds * ROT_SPEED;
        }
        if (IsKeyDown(VKEY_RIGHT))
        {
            fRotY -= fSeconds * ROT_SPEED;
        }
        if (IsKeyDown(VKEY_LEFT))
        {
            fRotY += fSeconds * ROT_SPEED;
        }

        // Rotate
        if (IsKeyDown(VKEY_W))
        {
            fZ -= fSeconds * MOVE_SPEED;
        }
        if (IsKeyDown(VKEY_S))
        {
            fZ += fSeconds * MOVE_SPEED;
        }
        if (IsKeyDown(VKEY_A))
        {
            fX -= fSeconds * MOVE_SPEED;
        }
        if (IsKeyDown(VKEY_D))
        {
            fX += fSeconds * MOVE_SPEED;
        }
        if (IsKeyDown(VKEY_SPACE))
        {
            fY += fSeconds * MOVE_SPEED;
        }
        if (IsKeyDown(VKEY_CONTROL))
        {
            fY -= fSeconds * MOVE_SPEED;
        }

        if (IsButtonDown(VBUTTON_LEFT))
        {
            int nX = 0;
            int nY = 0;
            GetMousePosition(nX,nY);
            printf("X: %d   Y: %d\n", nX, nY);
        }
        if (IsButtonDown(VBUTTON_RIGHT))
        {
            printf("Right Mouse Down\n");
        }
        if (IsButtonDown(VBUTTON_MIDDLE))
        {
            printf("Middle Mouse Down\n");
        }
        if (IsButtonDown(VBUTTON_X1))
        {
            printf("X1 Mouse Down\n");
        }
        if (IsButtonDown(VBUTTON_X2))
        {
            printf("X2 Mouse Down\n");
        }

        pCamera->SetPosition(fX, fY, fZ);
        pCamera->SetRotation(fRotX, fRotY, fRotZ);

        if (IsPointerDown())
        {
            fCube2Rot += fSeconds * ROT_SPEED;
            pTestMonkey->SetRotation(0.0f, fCube2Rot, 0.0f);
            pTestCube->SetRotation(fCube2Rot, 0.0f, 0.0f);
            pTestAnim->SetRotation(-1.0f * fCube2Rot, 0.0f, 0.0f);
        }

        timer.Start();
        Render();
    }

    delete pLibrary;
    delete pSun;
    delete pTestCube;
    delete pTestAnim;
    delete pTestMonkey;
    delete pMonkeyMesh;
    delete pCamera;
    delete pCubeMat;
    delete pTestScene;

    printf("Exiting.\n");
}
