#ifndef VINPUT_H
#define VINPUT_H

#include <string.h>

#if defined(WINDOWS)
enum VakzKeyEnum
{
    VKEY_BACK = 10,

    VKEY_0 = 48,
    VKEY_1 = 49,
    VKEY_2 = 50,
    VKEY_3 = 51,
    VKEY_4 = 52,
    VKEY_5 = 53,
    VKEY_6 = 54,
    VKEY_7 = 55,
    VKEY_8 = 56,
    VKEY_9 = 57,

    VKEY_A = 65,
    VKEY_B = 66,
    VKEY_C = 67,
    VKEY_D = 68,
    VKEY_E = 69,
    VKEY_F = 70,
    VKEY_G = 71,
    VKEY_H = 72,
    VKEY_I = 73,
    VKEY_J = 74,
    VKEY_K = 75,
    VKEY_L = 76,
    VKEY_M = 77,
    VKEY_N = 78,
    VKEY_O = 79,
    VKEY_P = 80,
    VKEY_Q = 81,
    VKEY_R = 82,
    VKEY_S = 83,
    VKEY_T = 84,
    VKEY_U = 85,
    VKEY_V = 86,
    VKEY_W = 87,
    VKEY_X = 88,
    VKEY_Y = 89,
    VKEY_Z = 90,

    VKEY_SPACE     = 32,
    VKEY_ENTER     = 13,
    VKEY_BACKSPACE = 8,
    VKEY_TAB       = 9,

    VKEY_SHIFT     = 16,
    VKEY_CONTROL   = 17,
    
    VKEY_UP        = 38,
    VKEY_DOWN      = 40,
    VKEY_LEFT      = 37,
    VKEY_RIGHT     = 39
};

enum VakzButtonEnum
{
    VBUTTON_LEFT   = 0,
    VBUTTON_RIGHT  = 1,
    VBUTTON_MIDDLE = 2,
    VBUTTON_X1     = 3,
    VBUTTON_X2     = 4
};

#elif defined(ANDROID)
enum VakzKeyEnum
{

    VKEY_BACK            = 4,

    VKEY_0               = 7,
    VKEY_1               = 8,
    VKEY_2               = 9,
    VKEY_3               = 10,
    VKEY_4               = 11,
    VKEY_5               = 12,
    VKEY_6               = 13,
    VKEY_7               = 14,
    VKEY_8               = 15,
    VKEY_9               = 16,

    VKEY_A               = 29,
    VKEY_B               = 30,
    VKEY_C               = 31,
    VKEY_D               = 32,
    VKEY_E               = 33,
    VKEY_F               = 34,
    VKEY_G               = 35,
    VKEY_H               = 36,
    VKEY_I               = 37,
    VKEY_J               = 38,
    VKEY_K               = 39,
    VKEY_L               = 40,
    VKEY_M               = 41,
    VKEY_N               = 42,
    VKEY_O               = 43,
    VKEY_P               = 44,
    VKEY_Q               = 45,
    VKEY_R               = 46,
    VKEY_S               = 47,
    VKEY_T               = 48,
    VKEY_U               = 49,
    VKEY_V               = 50,
    VKEY_W               = 51,
    VKEY_X               = 52,
    VKEY_Y               = 53,
    VKEY_Z               = 54,

    VKEY_SPACE           = 62,
    VKEY_ENTER           = 66,
    VKEY_BACKSPACE       = 67,
    VKEY_TAB             = 61,

    VKEY_SHIFT           = 60,
    VKEY_CONTROL         = 113,

    VKEY_UP              = 19,
    VKEY_DOWN            = 20,
    VKEY_LEFT            = 21,
    VKEY_RIGHT           = 22,
};

enum VakzButtonEnum
{
    VBUTTON_LEFT   = 0,
    VBUTTON_RIGHT  = 1,
    VBUTTON_MIDDLE = 2,
    VBUTTON_X1     = 3,
    VBUTTON_X2     = 4
};

#endif

enum VInputEnum
{
    VINPUT_MAX_KEYS        = 256,
    VINPUT_MAX_BUTTONS     = 16,
    VINPUT_MAX_TOUCHES     = 4,
    VINPUT_MAX_CONTROLLERS = 4,
    VINPUT_CONT_BUTTONS    = 14,
    VINPUT_CONT_AXES       = 18
};

enum VControllerEnum
{
    VCONT_A        = 96,
    VCONT_B        = 97,
    VCONT_C        = 98,
    VCONT_X        = 99,
    VCONT_Y        = 100,
    VCONT_Z        = 101,
    VCONT_L1       = 102,
    VCONT_R1       = 103,
    VCONT_L2       = 104,
    VCONT_R2       = 105,
    VCONT_THUMBL   = 106,
    VCONT_THUMBR   = 107,
    VCONT_START    = 108,
    VCONT_SELECT   = 109,

    VCONT_AXIS_X        = 0,
    VCONT_AXIS_Y        = 1,
    VCONT_AXIS_Z        = 11,
    VCONT_AXIS_RZ       = 14,
    VCONT_AXIS_HAT_X    = 15,
    VCONT_AXIS_HAT_Y    = 16,
    VCONT_AXIS_LTRIGGER = 17,
    VCONT_AXIS_RTRIGGER = 18
};

class Controller
{
public:
    Controller()
    {
        nDevice = -1;
        memset(arButtons, 0, VINPUT_CONT_BUTTONS * sizeof(int));
        memset(arAxes, 0, VINPUT_CONT_AXES * sizeof(float));
    }

    int   nDevice;
    int   arButtons[VINPUT_CONT_BUTTONS];
    float arAxes[VINPUT_CONT_AXES];
};

//## **********************************************************************
//## SetKey
//## 
//## Registers that a key is currently being pressed.
//## Should only be called internally by the engine.
//##
//## Input:
//##   nKey - keycode to set.
//## **********************************************************************
void SetKey(int nKey);

//## **********************************************************************
//## ClearKey
//##
//## Indicate that a key is no longer being pressed.
//## Should only be called internally by the engine.
//## 
//## Input:
//##   nKey - keycode to clear.
//## **********************************************************************
void ClearKey(int nKey);

void ClearAllKeys();

//## **********************************************************************
//## IsKeyDown
//##
//## Used to tell if a key is currently being pressed.
//##
//## Input:
//##   nKey - keycode to check.
//##
//## Returns:
//##   int - '1' if key is down.
//##         '0' if key is up.
//## **********************************************************************
int IsKeyDown(int nKey);

//## **********************************************************************
//## SetButton
//##
//## Registers that a mouse button is pressed down.
//## Should only be called internally by the engine.
//## 
//## Input:
//##   nButton - button code to register.
//## **********************************************************************
void SetButton(int nButton);

//## **********************************************************************
//## ClearButton
//## 
//## Indicate that a button is not being pressed.
//## Should only be called internally by the engine.
//## 
//## Input:
//##   nButton - button code to register.
//## **********************************************************************
void ClearButton(int nButton);

//## **********************************************************************
//## IsButtonDown
//##
//## Used to check if a mouse button is being held down.
//##
//## Input:
//##   nButton - mouse button code to check (see above).
//## 
//## Returns:
//##   int - '1' if mouse button is pressed down.
//##       - '0' if mouse button is not pressed.
//## **********************************************************************
int IsButtonDown(int nButton);

//## **********************************************************************
//## SetTouch
//##
//## Indicate that a touch index is being used (finger touching screen).
//## Should only be used internally by the engine.
//## 
//## Input:
//##   nTouch - touch index.
//## **********************************************************************
void SetTouch(int nTouch);

//## **********************************************************************
//## ClearTouch
//##
//## Indicate that a touch index is not being used.
//## Should only be used internally by the engine.
//##
//## Input:
//##   nTouch - touch index.
//## **********************************************************************
void ClearTouch(int nTouch);

//## **********************************************************************
//## IsTouchDown
//## 
//## Used to check if a touch is down on screen.
//## 
//## Input:
//##   nTouch - touch index to check.
//##
//## Returns:
//##   int - '1' if touch index is down.
//##         '0' otherwise.
//## **********************************************************************
int IsTouchDown(int nTouch);

//## **********************************************************************
//## IsPointerDown
//## 
//## Same as IsTouchDown(), but with the default touch index set to the 
//## mouse index.
//##
//## Input:
//##   nPointer - index of pointer (mouse or touch).
//##
//## Returns:
//##   int - '1' if pointer is pressed down.
//##       - '0' otherwise.
//## **********************************************************************
int IsPointerDown(int nPointer = 0);

//## **********************************************************************
//## GetMousePosition
//## 
//## Used to check the mouse position
//## **********************************************************************
void GetMousePosition(int& nMouseX,
                      int& nMouseY);

//## **********************************************************************
//## GetTouchPosition
//## 
//## Gets the position of a pointer in screen coordinates. Coordinate
//## origin is at bottom left corner of the screen.
//##
//## Input:
//##   nTouch - touch index to query.
//##
//## Output:
//##   nTouchX - x coordinate of touch position.
//##   nTouchY - y coordinate of touch position.
//## **********************************************************************
void GetTouchPosition(int& nTouchX,
                      int& nTouchY,
                      int  nTouch);

//## **********************************************************************
//## GetTouchPositionNormalized
//## 
//## Gets the position of a pointer in normalized screen coordinates. 
//## Coordinate origin is at bottom left corner of the screen.
//##
//## Input:
//##   nTouch - touch index to query.
//##
//## Output:
//##   fTouchX - x coordinate of touch position.
//##   fTouchY - y coordinate of touch position.
//## **********************************************************************
void GetTouchPositionNormalized(float& fTouchX,
                                float& fTouchY,
                                int    nTouch);

//## **********************************************************************
//## GetPointerPosition
//## 
//## Refer to GetTouchPosition. It is the same as that function except 
//## the touch index is defaulted to 0 (mouse index).
//## **********************************************************************
void GetPointerPosition(int& nPointerX,
                        int& nPointerY,
                        int  nPointer = 0);

//## **********************************************************************
//## GetPointerPosition
//## 
//## Refer to GetTouchPositionNormalized. It is the same as that function
//##  except the touch index is defaulted to 0 (mouse index).
//## **********************************************************************
void GetPointerPositionNormalized(float& fPointerX,
                                  float& fPointerY,
                                  int  nPointer = 0);

//## **********************************************************************
//## SetMousePosition
//## 
//## Register the current mouse location. Should only be called internally
//## by the engine. Origin is at bottom left of screen.
//##
//## Input:
//##   nMouseX - x coordinate of mouse position.
//##   nMouseY - y coordinate of mouse position.
//## **********************************************************************
void SetMousePosition(int nMouseX,
                      int nMouseY);

//## **********************************************************************
//## SetTouchPosition
//## 
//## Register the current touch position. Should only be called internally
//## by the engine. Origin as at the bottom left of the screen.
//## 
//## Input:
//##   nTouchX - x coordinate of the touch position.
//##   nTouchY - y cooridnate of the touch position.
//##   nTouch  - touch index.
//## **********************************************************************
void SetTouchPosition(int nTouchX,
                      int nTouchY,
                      int nTouch = 0);

void SetControllerButton(int nControllerButton,
                         int nControllerNumber);

void ClearControllerButton(int nControllerButton,
                           int nControllerNumber);

int IsControllerButtonDown(int nControllerButton,
                           int nControllerNumber);

void SetControllerAxisValue(int   nControllerAxis,
                            float fAxisValue,
                            int   nControllerNumber);

float GetControllerAxisValue(int nControllerAxis,
                             int nControllerNumber);

int GetControllerIndex(int nInputDevice);

void AssignController(int nInputDevice);

int IsControllerConnected(int nIndex);

void ShowSoftKeyboard();

void HideSoftKeyboard();

void InitializeSoftKeyboard();

int IsSoftKeyboardEnabled();

void RenderSoftKeyboard();

void UpdateSoftKeyboard();

int IsPointerDownRaw(int nPointer = 0);

int IsTouchDownRaw(int nTouch);

int CharToKey(char cTarget);

#endif
