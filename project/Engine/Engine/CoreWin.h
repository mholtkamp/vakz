#ifndef COREWIN_H
#define COREWIN_H

#include "Scene.h"

// Sets the window size
int SetWindowSize(int nWidth,
                  int nHeight);

// Initializes the Vakz Engine
int Initialize();

// Render the current scene 
int Render();

// Set the scene
int SetScene(Scene* pScene);

#endif
