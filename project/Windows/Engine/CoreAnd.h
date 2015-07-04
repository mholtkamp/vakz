#ifndef COREAND_H
#define COREAND_H

#include "VGL.h"
#include "Scene.h"

//## VakzData struct to hold information needed when rendering.
typedef struct VakzData
               {
                    ANativeWindow* window;
                    int animating;
                    EGLDisplay display;
                    EGLSurface surface;
                    EGLContext context;
                    int32_t width;
                    int32_t height;
               } VakzData;

// Create static struct to hold data needed by engine.
static VakzData vakzData;

//## **************************************************************************
//## SetWindowSize
//## 
//## Sets window size.
//##
//## Input:
//##   nWidth  - width of window.
//##   nHeight - height of window.
//##
//## Returns:
//##   int - 1 if successful, otherwise 0.
//## **************************************************************************
int SetWindowSize(int nWidth,
                  int nHeight);

//## **************************************************************************
//## Initialize
//##
//## Initializes the Vakz Engine
//##
//## Input:
//##   pData - pointer to native winow.
//## 
//## Returns:
//##   int - 1 if successful, 0 otherwise.
//## **************************************************************************
int Initialize(void* pData);

//## **************************************************************************
//## Render
//##
//## Renders the current scene.
//##  int - 1 if successful, 0 otherwise.
//## **************************************************************************
int Render();

//## **************************************************************************
//## SetScene
//##
//## Assigns the scene to be rendered.
//##
//## Input:
//##   pScene - pointer to new scene.
//## **************************************************************************
int SetScene(Scene* pScene);

//## **************************************************************************
//## SetClearColor
//##
//## Assigns color that will be used to clear the screen before rendering
//## the scene every frame. Colors are specified in normalized floating
//## point form (in the range 0.0f to 1.0f).
//##
//## Input:
//##   fRed   - red component
//##   fGreen - green component
//##   fBlue  - blue component
//##   fAlpha - alpha component
//## **************************************************************************
void SetClearColor(float fRed,
                   float fGreen,
                   float fBlue,
                   float fAlpha);

#endif
