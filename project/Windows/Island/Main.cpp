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
#include "Text.h"

#include <stdio.h>
#include <math.h>

#define ROT_SPEED 70.0f
#define MOVE_SPEED 5.0f
#define THRESH 0.4f

int main()
{
    SetWindowSize(1024,768);
    Initialize();

    //@@ DEBUG INVERSE TEST
    float arValues[16] = {2.0f,  6.0f, -1.0f,  3.0f,
                          3.0f,  0.0f,  3.0f, -2.0f,
                          8.0f, -3.0f,  2.0f, -4.0f,
                          5.0f,  2.0f, -1.0f,  4.0f};
    Matrix matTest;
    matTest.Load(arValues);
    matTest.Inverse();

    matTest.Load(arValues);
    matTest.Transpose();
    //@@ END

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
    BoxCollider* pCubeCollider = new BoxCollider();
    pCubeCollider->SetExtents(-1.0f, 1.0f,
                              -1.0f, 1.0f,
                              -1.0f, 1.0f);
    pTestCube->SetCollider(pCubeCollider);

    Matter* pTestMonkey = new Matter();
    StaticMesh* pMonkeyMesh = new StaticMesh();
    pMonkeyMesh->Load("druid.obj");
    pTestMonkey->SetMesh(pMonkeyMesh);
    pTestMonkey->SetMaterial(pCubeMat);
    pTestMonkey->SetPosition(0.0f, 0.0f, 0.0f);

    // Create TestAnim
    Matter* pTestAnim = new Matter();
    AnimatedMesh* pAnimMesh = new AnimatedMesh();
    pAnimMesh->Load("Druid_AM/druid.amf");
    pTestAnim->SetMesh(pAnimMesh);
    pTestAnim->SetLoopMode(Matter::LOOP_NONE);
    pTestAnim->SetAnimation("No");
    pTestAnim->StartAnimation();
    pTestAnim->SetMaterial(pCubeMat);
    pTestAnim->SetPosition(-3.5f, 0.0f, 0.0f);
    BoxCollider* pTestCollider = new BoxCollider();
    pTestCollider->SetExtents(-0.706f, 0.706f,
                               0.0f,   2.464f,
                              -0.706f, 0.706f);
    pTestAnim->SetCollider(pTestCollider);
    pTestAnim->SetScale(0.25f, 0.25f, 0.25f);

    LogDebug("Loaded both meshes.");

    Texture* pTestTexture = new Texture();
    pTestTexture->LoadBMP("trueform_base_color.bmp");
    pTestMonkey->SetTexture(pTestTexture);
    pTestAnim->SetTexture(pTestTexture);

    // Create sun
    DirectionalLight* pSun = new DirectionalLight();
    pSun->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    pSun->SetDirectionVector(3.0f, -1.0f, -2.0f);
    pTestScene->AddLight(pSun);

    // Create some screen text
    Text* pTestText = new Text();
    float fTextScaleX = 0.6f;
    float fTextScaleY = 0.8f;
    pTestText->SetPosition(-0.9f, 0.8f);
    pTestText->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    pTestText->SetScale(fTextScaleX, fTextScaleY);
    pTestText->SetText("Animation:\nWave");

    // Add the test cube to the scene
    pTestScene->AddMatter(pTestCube);
    //pTestScene->AddMatter(pTestMonkey);
    pTestScene->AddMatter(pTestAnim);

    // Add text to screen
    pTestScene->AddGlyph(pTestText);

    // Collision text
    Text colText;
    int nCollision = 0;
    colText.SetColor(0.2f, 0.8f, 0.28f, 1.0f);
    colText.SetPosition(0.5f, 0.8f);
    colText.SetScale(0.6f, 0.8f);
    colText.SetText("No Collision");
    pTestScene->AddGlyph(&colText);

    float fSeconds = 0.0f;
    float fZ = 10.0f;
    float fY = 0.0f;
    float fX = 0.0f;
    float fRotX = 0.0f;
    float fRotY = 0.0f;
    float fRotZ = 0.0f;
    int nLock = 0;
    float fBearX = -3.5f;
    float fBearY = 0.0f;
    float fBearZ = 0.0f;
    float fCube2Rot = 0.0f;
    float fAnimSpeed = 1.0f;
    Timer timer;
    timer.Start();

    while ((GetStatus() & VAKZ_QUIT) == 0)
    {
        timer.Stop();

        fSeconds = timer.Time();

        // Update
        Update();

        if (IsControllerConnected(0))
        {
            // Camera
            if (GetControllerAxisValue(VCONT_AXIS_RZ, 0) < -THRESH)
            {
                fRotX += fSeconds * ROT_SPEED;
            }
            if (GetControllerAxisValue(VCONT_AXIS_RZ, 0) > THRESH)
            {
                fRotX -= fSeconds * ROT_SPEED;
            }
            if (GetControllerAxisValue(VCONT_AXIS_Z, 0) > THRESH)
            {
                fRotY -= fSeconds * ROT_SPEED;
            }
            if (GetControllerAxisValue(VCONT_AXIS_Z, 0) < -THRESH)
            {
                fRotY += fSeconds * ROT_SPEED;
            }

            // Movement
            if (GetControllerAxisValue(VCONT_AXIS_Y, 0) < -THRESH)
            {
                fZ -= fSeconds * MOVE_SPEED * cos(fRotY * DEGREES_TO_RADIANS);
                fX -= fSeconds * MOVE_SPEED * sin(fRotY * DEGREES_TO_RADIANS);
            }
            if (GetControllerAxisValue(VCONT_AXIS_Y, 0) > THRESH)
            {
                fZ += fSeconds * MOVE_SPEED * cos(fRotY * DEGREES_TO_RADIANS);
                fX += fSeconds * MOVE_SPEED * sin(fRotY * DEGREES_TO_RADIANS);
            }
            if (GetControllerAxisValue(VCONT_AXIS_X, 0) < -THRESH)
            {
                fZ -= fSeconds * MOVE_SPEED * cos((90.0f + fRotY) * DEGREES_TO_RADIANS);
                fX -= fSeconds * MOVE_SPEED * sin((90.0f + fRotY) * DEGREES_TO_RADIANS);
            }
            if (GetControllerAxisValue(VCONT_AXIS_X, 0) > THRESH)
            {
                fZ += fSeconds * MOVE_SPEED * cos((90.0f + fRotY) * DEGREES_TO_RADIANS);
                fX += fSeconds * MOVE_SPEED * sin((90.0f + fRotY) * DEGREES_TO_RADIANS);
            }
            if (IsControllerButtonDown(VCONT_R1, 0))
            {
                fY += fSeconds * MOVE_SPEED;
            }
            if (IsControllerButtonDown(VCONT_L1, 0))
            {
                fY -= fSeconds * MOVE_SPEED;
            }

            if (IsControllerButtonDown(VCONT_A, 0))
            {
                pTestAnim->SetAnimation("Wave");
                pTestText->SetText("Animation:\nWave");
                pTestAnim->ResetAnimation();
            }

            if (IsControllerButtonDown(VCONT_B, 0))
            {
                pTestAnim->SetAnimation("No");
                pTestText->SetText("Animation:\nNo");
                pTestAnim->ResetAnimation();
            }
        }
        else
        {
            //LogDebug("Controller is not connected");
        }

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

        // Move
        if (IsKeyDown(VKEY_W))
        {
            fZ -= fSeconds * MOVE_SPEED * cos(fRotY * DEGREES_TO_RADIANS);
            fX -= fSeconds * MOVE_SPEED * sin(fRotY * DEGREES_TO_RADIANS);
        }
        if (IsKeyDown(VKEY_S))
        {
            fZ += fSeconds * MOVE_SPEED * cos(fRotY * DEGREES_TO_RADIANS);
            fX += fSeconds * MOVE_SPEED * sin(fRotY * DEGREES_TO_RADIANS);
        }
        if (IsKeyDown(VKEY_A))
        {
            fZ -= fSeconds * MOVE_SPEED * cos((90.0f + fRotY) * DEGREES_TO_RADIANS);
            fX -= fSeconds * MOVE_SPEED * sin((90.0f + fRotY) * DEGREES_TO_RADIANS);
        }
        if (IsKeyDown(VKEY_D))
        {
            fZ += fSeconds * MOVE_SPEED * cos((90.0f + fRotY) * DEGREES_TO_RADIANS);
            fX += fSeconds * MOVE_SPEED * sin((90.0f + fRotY) * DEGREES_TO_RADIANS);
        }
        if (IsKeyDown(VKEY_SPACE))
        {
            fY += fSeconds * MOVE_SPEED;
        }
        if (IsKeyDown(VKEY_CONTROL))
        {
            fY -= fSeconds * MOVE_SPEED;
        }

        if (IsButtonDown(VBUTTON_MIDDLE))
        {
            pTestAnim->SetLoopMode(Matter::LOOP_PING_PONG);
            pTestAnim->ResetAnimation();
        }
        if (IsButtonDown(VBUTTON_X1))
        {
            pTestAnim->SetLoopMode(Matter::LOOP_NONE);
            pTestAnim->ResetAnimation();
        }
        if (IsButtonDown(VBUTTON_X2))
        {
            pTestAnim->SetLoopMode(Matter::LOOP_CYCLE);
            pTestAnim->ResetAnimation();
        }

        if (IsKeyDown(VKEY_U))
        {
            fAnimSpeed -= 0.005f;
            pTestAnim->SetAnimationSpeed(fAnimSpeed);
        }

        if (IsKeyDown(VKEY_I))
        {
            fAnimSpeed += 0.005f;
            pTestAnim->SetAnimationSpeed(fAnimSpeed);
        }

        if (IsKeyDown(VKEY_O))
        {
            fAnimSpeed = 1.0f;
            pTestAnim->SetAnimationSpeed(fAnimSpeed);
        }

        if (IsKeyDown(VKEY_1))
        {
            pTestAnim->SetAnimation("Wave");
            pTestText->SetText("Animation:\nWave");
            pTestAnim->ResetAnimation();
        }

        if (IsKeyDown(VKEY_2))
        {
            pTestAnim->SetAnimation("No");
            pTestText->SetText("Animation:\nNo");
            pTestAnim->ResetAnimation();
        }

        if (IsKeyDown(VKEY_N))
        {
            if (IsKeyDown(VKEY_Z))
            {
                fTextScaleX -= 0.005f;
            }
            else
            {
                fTextScaleX += 0.005f;
            }

            pTestText->SetScale(fTextScaleX, fTextScaleY);
        }

        if (IsKeyDown(VKEY_M))
        {
            if (IsKeyDown(VKEY_Z))
            {
                fTextScaleY -= 0.005f;
            }
            else
            {
                fTextScaleY += 0.005f;
            }

            pTestText->SetScale(fTextScaleX, fTextScaleY);
        }

        if (IsKeyDown(VKEY_Z))
        {
            if (IsKeyDown(VKEY_C))
            {
                fBearX -= 0.01;
            }
            if (IsKeyDown(VKEY_V))
            {
                fBearY -= 0.01;
            }
            if (IsKeyDown(VKEY_B))
            {
                fBearZ -= 0.01;
            }
        }
        else
        {
            if (IsKeyDown(VKEY_C))
            {
                fBearX += 0.01;
            }
            if (IsKeyDown(VKEY_V))
            {
                fBearY += 0.01;
            }
            if (IsKeyDown(VKEY_B))
            {
                fBearZ += 0.01;
            }
        }
        pTestAnim->SetPosition(fBearX, fBearY, fBearZ);



        pCamera->SetPosition(fX, fY, fZ);
        pCamera->SetRotation(fRotX, fRotY, fRotZ);

        if (IsPointerDown())
        {
            //fCube2Rot += fSeconds * ROT_SPEED;
            //pTestMonkey->SetRotation(0.0f, fCube2Rot, 0.0f);
            //pTestCube->SetRotation(fCube2Rot, 0.0f, 0.0f);
            //pTestAnim->SetRotation(0.0f, -1.0f * fCube2Rot, 0.0f);
        }

        if (pTestAnim->Overlaps(pTestCube))
        {
            if (nCollision == 0)
            {
                nCollision = 1;
                colText.SetColor(0.8f, 0.2f, 0.28f, 1.0f);
                colText.SetText("Collision!");
            }
        }
        else
        {
            if (nCollision == 1)
            {
                nCollision = 0;
                colText.SetColor(0.2f, 0.8f, 0.28f, 1.0f);
                colText.SetText("No Collision");
            }
        }
        timer.Start();
        Render();
    }

    delete pTestCollider;
    delete pCubeCollider;
    delete pLibrary;
    delete pSun;
    delete pTestCube;
    delete pTestAnim;
    delete pTestMonkey;
    delete pMonkeyMesh;
    delete pCamera;
    delete pCubeMat;
    delete pTestScene;

    exit(0);
}
