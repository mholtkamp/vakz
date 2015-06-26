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

#include "Log.h"
#include "Vakz.h"
#include "Quad.h"


void android_main(struct android_app* state) {

    // Make sure glue isn't stripped.
    app_dummy();
    LogWarning("Okay...");
    Initialize(state->window);

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
        Render();
    }

    delete pTestQuad;
    delete pTestScene;
}

