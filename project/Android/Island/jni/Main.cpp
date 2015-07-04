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
#include "VGL.h"
#include "Matrix.h"
#include "ResourceLibrary.h"
#include "DiffuseMaterial.h"
#include "DirectionalLight.h"

#define ROT_SPEED 30.0f
#define MOVE_SPEED 5.0f

int animating = 1;
static volatile int nInitialized = 0;

static void handle_cmd(struct android_app* app, int32_t cmd)
{
    switch (cmd) {
        case APP_CMD_INIT_WINDOW:
            LogWarning("**** APP_CMD_INIT_WINDOW ****");
            // The window is being shown, get it ready.
            if (app->window != NULL) {
                LogWarning("**** app->window != null ****");
                Initialize(app->window);
                nInitialized = 1;
                Render();
            }
            break;
    }
}

void android_main(struct android_app* state) {

    state->onAppCmd = handle_cmd;

    usleep(55000);

    int ident;
    int events;
    struct android_poll_source* source;

    LogWarning("****ISLAND APP STARTING****");

    // Keep processing events until the OpenGL context is created.
    while(nInitialized == 0)
    {
        while ((ident=ALooper_pollAll(animating ? 1 : -1, NULL, &events,
                (void**)&source)) >= 0)
        {
            LogWarning("****Processing Event****");
            // Process this event.
            if (source != NULL)
            {
                source->process(state, source);
            }
        }
    }

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
    pCubeMat->SetColor(0.3f, 0.3f, 0.8f, 1.0f);

    // Create Matter
    Matter* pTestCube = new Matter();
    pTestCube->SetMesh(reinterpret_cast<StaticMesh*>(pLibrary->GetPrimitive(PRIMITIVE_CUBE)));
    pTestCube->SetMaterial(pCubeMat);

    Matter* pTestCube2 = new Matter();
    pTestCube2->SetMesh(reinterpret_cast<StaticMesh*>(pLibrary->GetPrimitive(PRIMITIVE_CUBE)));
    pTestCube2->SetMaterial(pCubeMat);
    pTestCube2->SetPosition(-5.0f, 0.0f, 0.0f);

    // Create sun
    DirectionalLight* pSun = new DirectionalLight();
    pSun->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    pSun->SetDirectionVector(3.0f, -1.0f, -2.0f);
    pTestScene->AddLight(pSun);

    // Add the test cube to the scene
    pTestScene->AddMatter(pTestCube);
    pTestScene->AddMatter(pTestCube2);


    float fSeconds = 0.0;
    float fZ = 20.0f;
    float fY = 0.0f;
    float fX = 0.0f;
    float fRotX = 0.0f;
    float fRotY = 0.0f;
    float fRotZ = 0.0f;
    int nLock = 0;
    float fCube2Rot = 0.0f;

    struct timeval startTime;
    struct timeval endTime;

    gettimeofday(&startTime, 0);

    while (1)
    {
        gettimeofday(&endTime, 0);

        fSeconds = static_cast<float>(endTime.tv_sec - startTime.tv_sec);
        fSeconds += static_cast<float>(endTime.tv_usec - startTime.tv_usec)/1000000.0f;

        pCamera->SetPosition(fX, fY, fZ);
        pCamera->SetRotation(fRotX, fRotY, fRotZ);

        fCube2Rot += fSeconds * ROT_SPEED;
        pTestCube2->SetRotation(fCube2Rot, 0.0f, 0.0f);

        gettimeofday(&startTime, 0);
        Render();
    }

    delete pSun;
    delete pTestCube;
    delete pTestCube2;
    delete pCamera;
    delete pCubeMat;
    delete pLibrary;
    delete pTestScene;

}

