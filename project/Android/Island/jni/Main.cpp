/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <android_native_app_glue.h>

#include <errno.h>
#include <jni.h>
#include <sys/time.h>
#include <time.h>
#include <android/log.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/time.h>

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

#define ROT_SPEED 70.0f
#define MOVE_SPEED 5.0f
#define THRESH 0.4f

#define FLOWER_BOX_SIZE 100.0f
#define FLOWER_POT_COUNT 100

int animating = 1;

void android_main(struct android_app* state) {

    //state->onAppCmd = handle_cmd;.
    SetResolution(1280, 720);
    Initialize(state);

    //usleep(55000);

    // Make sure glue isn't stripped.
    app_dummy();


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
    DiffuseMaterial* pWhiteMat = new DiffuseMaterial();
    pWhiteMat->SetColor(1.0f, 1.0f, 1.0f, 1.0f);

    DiffuseMaterial* pBlueMat = new DiffuseMaterial();
    pBlueMat->SetColor(0.3f, 0.3f, 0.9f, 1.0f);


    // Create FlowerPot
    Matter* pFlowerPots               = new Matter[FLOWER_POT_COUNT];
    MeshCollider* pFlowerPotColliders = new MeshCollider[FLOWER_POT_COUNT];

    StaticMesh* pFlowerPotStaticMesh = new StaticMesh();
    pFlowerPotStaticMesh->Load("flowerpot.obj");
    StaticMesh* pFlowerPotCollisionMesh = new StaticMesh();
    pFlowerPotCollisionMesh->LoadGeometry("flowerpot_collider.obj");
    Texture* pFlowerPotTexture = new Texture();
    pFlowerPotTexture->LoadBMP("flowerpot_texture.bmp");

    for (int fp = 0; fp < FLOWER_POT_COUNT; fp++)
    {
        pFlowerPots[fp].SetMesh(pFlowerPotStaticMesh);
        pFlowerPots[fp].SetMaterial(pWhiteMat);
        pFlowerPots[fp].SetPosition(fp % 10, fp / 10, - fp / 10);
        pFlowerPots[fp].SetRotation(0.0f, 0.0f, 0.0f);

        pFlowerPots[fp].SetTexture(pFlowerPotTexture);

        pFlowerPotColliders[fp].AssignMesh(pFlowerPotCollisionMesh);
        //pFlowerPotCollider->EnableRendering();
        pFlowerPots[fp].SetCollider(&(pFlowerPotColliders[fp]));
        //pFlowerPot->SetRigid(1);

        // Add to scene
        //pTestScene->AddMatter(&(pFlowerPots[fp]));
    }

    // Create TestAnim
    Matter* pTestAnim = new Matter();
    AnimatedMesh* pAnimMesh = new AnimatedMesh();
    pAnimMesh->Load("Druid_AM/druid.amf");
    pTestAnim->SetMesh(pAnimMesh);
    pTestAnim->SetLoopMode(Matter::LOOP_NONE);
    pTestAnim->SetAnimation("No");
    pTestAnim->StartAnimation();
    pTestAnim->SetMaterial(pWhiteMat);
    pTestAnim->SetPosition(-3.5f, 0.0f, 0.0f);
    BoxCollider* pTestCollider = new BoxCollider();
    pTestCollider->SetExtents(-0.706f, 0.706f,
                               0.0f,   2.464f,
                              -0.706f, 0.706f);
    pTestAnim->SetCollider(pTestCollider);
    pTestAnim->SetScale(0.25f, 0.25f, 0.25f);
    //pTestAnim->SetPhysical(1);

    LogDebug("Loaded both meshes.");

    Texture* pTestTexture = new Texture();
    pTestTexture->LoadBMP("trueform_base_color.bmp");
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

    // Particle System Test
    ParticleSystem* pParticleSystem = new ParticleSystem();
    float arParMinColor[4] = {1.0f, 1.0f, 1.0, 0.1f};
    float arParMaxColor[4] = {1.0f, 1.0f, 1.0, 0.3f};
    pParticleSystem->SetColor(arParMinColor, arParMaxColor);

    Texture texPart;
    texPart.LoadBMP("poof.bmp", 1);
    pParticleSystem->SetTexture(&texPart);
    pParticleSystem->Initialize();
    pTestScene->AddParticleSystem(pParticleSystem);

    float fFlowerX = 0.0f;
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
    int nRenderCount = 0;
    Timer timer;
    Timer timerRender;
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
                pTestAnim->SetXVelocity(-3.0f);
            }
            if (IsKeyDown(VKEY_V))
            {

            }
            if (IsKeyDown(VKEY_B))
            {
                pTestAnim->SetZVelocity(-3.0f);
            }
        }
        else
        {
            if (IsKeyDown(VKEY_C))
            {
                pTestAnim->SetXVelocity(3.0f);
            }
            if (IsKeyDown(VKEY_V))
            {
                pTestAnim->SetYVelocity(8.0f);
            }
            if (IsKeyDown(VKEY_B))
            {
                pTestAnim->SetZVelocity(3.0f);
            }
        }

        if (IsKeyDown(VKEY_C) == 0)
        {
            pTestAnim->SetXVelocity(0);
        }
        if (IsKeyDown(VKEY_V) == 0)
        {

        }
        if (IsKeyDown(VKEY_B) == 0)
        {
            pTestAnim->SetZVelocity(0.0f);
        }
        //pTestAnim->SetPosition(fBearX, fBearY, fBearZ);



        pCamera->SetPosition(fX, fY, fZ);
        pCamera->SetRotation(fRotX, fRotY, fRotZ);

        if (IsPointerDown())
        {
            //fCube2Rot += fSeconds * ROT_SPEED;
            //pTestMonkey->SetRotation(0.0f, fCube2Rot, 0.0f);
            //pTestCube->SetRotation(fCube2Rot, 0.0f, 0.0f);
            //pTestAnim->SetRotation(0.0f, -1.0f * fCube2Rot, 0.0f);
        }

        //if (pTestAnim->Overlaps(pFlowerPot))
        //{
        //    if (nCollision == 0)
        //    {
        //        nCollision = 1;
        //        colText.SetColor(0.8f, 0.2f, 0.28f, 1.0f);
        //        colText.SetText("Collision!");
        //    }
        //}
        //else
        //{
        //    if (nCollision == 1)
        //    {
        //        nCollision = 0;
        //        colText.SetColor(0.2f, 0.8f, 0.28f, 1.0f);
        //        colText.SetText("No Collision");
        //    }
        //}
        timer.Start();
        if (nRenderCount == 0)
        {
            timerRender.Start();
        }

        Render();
        nRenderCount++;

        if(nRenderCount == 50)
        {
            char arMsg[32] = {0};
            timerRender.Stop();
            nRenderCount = 0;
            float fRenderTime = timerRender.Time()/50.0f;
            sprintf(arMsg, "Render Time: %f", fRenderTime);
            LogDebug(arMsg);
        }
    }

    delete pTestCollider;
    delete pLibrary;
    delete pSun;
    delete pTestAnim;
    delete pCamera;
    delete pBlueMat;
    delete pWhiteMat;
    delete pTestScene;
    delete [] pFlowerPots;
    exit(0);
}
