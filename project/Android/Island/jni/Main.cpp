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

#include "Log.h"
#include "Vakz.h"
#include "Quad.h"

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

    // Force a warning to occur.
    Camera* pCamera = 0;
    pTestScene->SetCamera(pCamera);

    Quad* pTestQuad = new Quad();
    pTestScene->AddGlyph(pTestQuad);

    pTestQuad->SetBox(100.0f,
                      100.0f,
                      100.0f,
                      200.0f);
    pTestQuad->SetColor(0.0f, 0.0f, 0.6f, 1.0f);

    while (1)
    {
        while ((ident=ALooper_pollAll(animating ? 1 : -1, NULL, &events,
                (void**)&source)) >= 0)
        {
            // Process this event.
            if (source != NULL)
            {
                source->process(state, source);
            }
        }

        Render();
    }

    delete pTestQuad;
    delete pTestScene;
}

