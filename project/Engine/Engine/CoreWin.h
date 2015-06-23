#ifndef COREWIN_H
#define COREWIN_H

#include "Scene.h"

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
//## Returns:
//##   int - 1 if successful, 0 otherwise.
//## **************************************************************************
int Initialize();

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
//## **************************************************************************
int SetScene(Scene* pScene);

#endif
