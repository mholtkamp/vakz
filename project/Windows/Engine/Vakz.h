#ifndef VAKZ_H 
#define VAKZ_H

#define VAKZ_QUIT         0x01
#define VAKZ_INITIALIZED  0x02
#define VAKZ_ACTIVE       0x04

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
//## SetResolution
//## 
//## Sets resolution of game, to be rendered on entirety of window.
//##
//## Input:
//##   nResX - x resolution in pixels.
//##   nResY - y resolution in pixels.
//##
//## Returns:
//##   int - 1 if successful, otherwise 0.
//## **************************************************************************
int SetResolution(int nResX,
                  int nResY);

//## **************************************************************************
//## Initialize
//##
//## Initializes the Vakz Engine
//## 
//## Input:
//##   pData - data needed to initialize. (Only needed on Android)
//## 
//## Returns:
//##   int - 1 if successful, 0 otherwise.
//## **************************************************************************
int Initialize(void* pData = 0);

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
int SetScene(void* pScene);

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

//## **************************************************************************
//## Shutdown
//##
//## Cleans up resources created by engine.
//## **************************************************************************
void Shutdown();

//## **************************************************************************
//## Update
//##
//## Performs general upkeep of engine. Should be called once every frame.
//## **************************************************************************
void Update();

//## **************************************************************************
//## GetStatus
//##
//## Returns an 8-bit field, where each value represents a status flag.
//##
//## Returns:
//##   int - status of engine.
//## **************************************************************************
unsigned char GetStatus();

#endif
