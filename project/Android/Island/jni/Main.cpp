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

#include "Log.h"
#include "Vakz.h"
#include "Quad.h"
#include "Text.h"
#include "VGL.h"
#include "VInput.h"
#include "VMath.h"
#include "Timer.h"
#include "Matrix.h"
#include "ResourceLibrary.h"
#include "DiffuseMaterial.h"
#include "DirectionalLight.h"

#include "Camera.h"
#include "Scene.h"

#define ROT_SPEED 70.0f
#define MOVE_SPEED 5.0f
#define THRESH 0.4f
int animating = 1;

void android_main(struct android_app* state) {

    //state->onAppCmd = handle_cmd;
    Initialize(state);

    //usleep(55000);

    // Make sure glue isn't stripped.
    app_dummy();

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
    pMonkeyMesh->Load("druid.obj");
    pTestMonkey->SetMesh(pMonkeyMesh);
    pTestMonkey->SetMaterial(pCubeMat);
    pTestMonkey->SetPosition(0.0f, 0.0f, 0.0f);

    Matter* pTestAnim = new Matter();
    AnimatedMesh* pAnimMesh = new AnimatedMesh();
    pAnimMesh->Load("Druid_AM/druid.amf");
    pTestAnim->SetMesh(pAnimMesh);
    pTestAnim->SetLoopMode(Matter::LOOP_NONE);
    pTestAnim->SetAnimation("No");
    pTestAnim->StartAnimation();
    pTestAnim->SetMaterial(pCubeMat);
    pTestAnim->SetPosition(-3.5f, 0.0f, 0.0f);

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
    pTestText->SetPosition(-0.9, 0.8f);
    pTestText->SetColor(1.0f, 0.0f, 0.0f, 0.8f);
    pTestText->SetScale(0.45f, 0.8f);
    pTestText->SetText("Beep");

    // Add the test cube to the scene
    pTestScene->AddMatter(pTestCube);
    //pTestScene->AddMatter(pTestMonkey);
    pTestScene->AddMatter(pTestAnim);

    // Add text to screen
    pTestScene->AddGlyph(pTestText);

    float fSeconds = 0.0f;
    float fZ = 10.0f;
    float fY = 0.0f;
    float fX = 0.0f;
    float fRotX = 0.0f;
    float fRotY = 0.0f;
    float fRotZ = 0.0f;
    int nLock = 0;
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
                pTestText->SetText("Animation: Wave");
                pTestAnim->ResetAnimation();
            }

            if (IsControllerButtonDown(VCONT_B, 0))
            {
                pTestAnim->SetAnimation("No");
                pTestText->SetText("Animation: No");
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
            pTestText->SetText("Animation: Wave");
            pTestAnim->ResetAnimation();
        }

        if (IsKeyDown(VKEY_2))
        {
            pTestAnim->SetAnimation("No");
            pTestText->SetText("Animation: No");
            pTestAnim->ResetAnimation();
        }

        pCamera->SetPosition(fX, fY, fZ);
        pCamera->SetRotation(fRotX, fRotY, fRotZ);

        if (IsPointerDown())
        {
            fCube2Rot += fSeconds * ROT_SPEED;
            pTestMonkey->SetRotation(0.0f, fCube2Rot, 0.0f);
            pTestCube->SetRotation(fCube2Rot, 0.0f, 0.0f);
            pTestAnim->SetRotation(0.0f, -1.0f * fCube2Rot, 0.0f);
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

    exit(0);
}

