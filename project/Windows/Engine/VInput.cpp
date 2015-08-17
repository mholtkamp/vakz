#include "VInput.h"
#include "Log.h"
#include "Settings.h"
#include "Keyboard.h"
#include <string.h>
#include <ctype.h>

static int s_arKeys[VINPUT_MAX_KEYS]       = {0};
static int s_arButtons[VINPUT_MAX_BUTTONS] = {0};
static int s_arTouches[VINPUT_MAX_TOUCHES] = {0};

static int s_arPointerX[VINPUT_MAX_TOUCHES] = {0};
static int s_arPointerY[VINPUT_MAX_TOUCHES] = {0};

static Controller s_arControllers[VINPUT_MAX_CONTROLLERS];
static int s_nNumControllers = 0;

static Keyboard* s_pKeyboard = 0;
static int s_nKeyboardEnable = 0;

//*****************************************************************************
// SetKey
//*****************************************************************************
void SetKey(int nKey)
{
    if (nKey >= 0 &&
        nKey <  VINPUT_MAX_KEYS)
    {
        s_arKeys[nKey] = 1;
    }
}

//*****************************************************************************
// ClearKey
//*****************************************************************************
void ClearKey(int nKey)
{
    if (nKey >= 0 &&
        nKey <  VINPUT_MAX_KEYS)
    {
        s_arKeys[nKey] = 0;
    }
}

//*****************************************************************************
// ClearAllKeys
//*****************************************************************************
void ClearAllKeys()
{
    int i = 0;

    for (i = 0; i < VINPUT_MAX_KEYS; i++)
    {
        s_arKeys[i] = 0;
    }
}

//*****************************************************************************
// IsKeyDown
//*****************************************************************************
int IsKeyDown(int nKey)
{
    if (nKey >= 0 &&
        nKey <  VINPUT_MAX_KEYS)
    {
        return s_arKeys[nKey];
    }
    else
    {
        LogWarning("Invalid key queried in IsKeyDown().");
        return 0;
    }
}

//*****************************************************************************
// SetButton
//*****************************************************************************
void SetButton(int nButton)
{
    if (nButton >= 0 &&
        nButton <  VINPUT_MAX_BUTTONS)
    {
        s_arButtons[nButton] = 1;
    }
}

//*****************************************************************************
// ClearButton
//*****************************************************************************
void ClearButton(int nButton)
{
    if (nButton >= 0 &&
        nButton <  VINPUT_MAX_BUTTONS)
    {
        s_arButtons[nButton] = 0;
    }
}

//*****************************************************************************
// IsButtonDown
//*****************************************************************************
int IsButtonDown(int nButton)
{
    if (nButton >= 0 &&
        nButton <  VINPUT_MAX_BUTTONS)
    {
        return s_arButtons[nButton];
    }
    else
    {
        LogWarning("Invalid button queried in IsButtonDown().");
        return 0;
    }
}

//*****************************************************************************
// SetTouch
//*****************************************************************************
void SetTouch(int nTouch)
{
    if (nTouch >= 0 &&
        nTouch <  VINPUT_MAX_TOUCHES)
    {
        s_arTouches[nTouch] = 1;
    }
}

//*****************************************************************************
// ClearTouch
//*****************************************************************************
void ClearTouch(int nTouch)
{
    if (nTouch >= 0 &&
        nTouch <  VINPUT_MAX_TOUCHES)
    {
        s_arTouches[nTouch] = 0;
    }
}

//*****************************************************************************
// IsTouchDown
//*****************************************************************************
int IsTouchDown(int nTouch)
{
    float fX = 0.0f;
    float fY = 0.0f;

    if (nTouch >= 0 &&
        nTouch <  VINPUT_MAX_TOUCHES)
    {

        if (s_nKeyboardEnable != 0)
        {
            GetPointerPositionNormalized(fX, fY, nTouch);
            if (fY > 0.0f)
            {
                return s_arTouches[nTouch];
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return s_arTouches[nTouch];
        }
    }
    else
    {
        LogWarning("Invalid touch queried in IsTouchDown().");
        return 0;
    }
}

//*****************************************************************************
// IsPointerDown
//*****************************************************************************
int IsPointerDown(int nPointer)
{
    float fX = 0.0f;
    float fY = 0.0f;

    if (nPointer >= 0 &&
        nPointer <  VINPUT_MAX_TOUCHES)
    {
        // If either the left mouse button is down or the specified
        // touch index is down, then return 1.
        if ((s_arButtons[VBUTTON_LEFT] != 0) ||
            (s_arTouches[nPointer]     != 0))
        {
            if (s_nKeyboardEnable != 0)
            {
                GetPointerPositionNormalized(fX, fY, nPointer);
                if (fY > 0.0f)
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                // Return true if down and no keyboard.
                return 1;
            }
        }
        else
        {
            return 0;
        }
    }
    else
    {
        LogWarning("Invalid pointer queried in IsPointerDown().");
        return 0;
    }
}

//*****************************************************************************
// GetMousePosition
//*****************************************************************************
void GetMousePosition(int& nMouseX, int& nMouseY)
{
    // First pointer location is for mouse.
    nMouseX = s_arPointerX[0];
    nMouseY = s_arPointerY[0];
}


//*****************************************************************************
// GetTouchPosition
//*****************************************************************************
void GetTouchPosition(int& nTouchX,
                      int& nTouchY,
                      int nTouch)
{
    if (nTouch >= 0 &&
        nTouch <  VINPUT_MAX_TOUCHES)
    {
        nTouchX = s_arPointerX[nTouch];
        nTouchY = s_arPointerY[nTouch];
    }
    else
    {
        LogWarning("Invalid touch index queried in GetTouchPosition().");
        return;
    }
}

//*****************************************************************************
// GetTouchPositionNormalized
//*****************************************************************************
void GetTouchPositionNormalized(float& fTouchX,
                                float& fTouchY,
                                int    nTouch)
{
    if (nTouch >= 0 &&
        nTouch <  VINPUT_MAX_TOUCHES)
    {
        fTouchX = (s_arPointerX[nTouch] - (g_nScreenWidth/2.0f)) / (g_nScreenWidth/2.0f);
        fTouchY = (s_arPointerY[nTouch] - (g_nScreenHeight/2.0f)) / (g_nScreenHeight/2.0f);
    }
    else
    {
        LogWarning("Invalid touch index queried in GetTouchPosition().");
        return;
    }
}

//*****************************************************************************
// GetPointerPosition
//*****************************************************************************
void GetPointerPosition(int& nPointerX,
                        int& nPointerY,
                        int nPointer)
{
    GetTouchPosition(nPointerX,
                     nPointerY,
                     nPointer);
}

//*****************************************************************************
// GetPointerPositionNormalized
//*****************************************************************************
void GetPointerPositionNormalized(float& fPointerX,
                                  float& fPointerY,
                                  int  nPointer)
{
    GetTouchPositionNormalized(fPointerX,
                               fPointerY,
                               nPointer);
}

//*****************************************************************************
// SetMousePosition
//*****************************************************************************
void SetMousePosition(int nMouseX, int nMouseY)
{
    // First index in pointer array is mouse position.
    s_arPointerX[0] = nMouseX;
    s_arPointerY[0] = nMouseY;
}

//*****************************************************************************
// SetTouchPosition
//*****************************************************************************
void SetTouchPosition(int nTouchX,
                      int nTouchY,
                      int nTouch)
{
    if (nTouch >= 0 &&
        nTouch <  VINPUT_MAX_TOUCHES)
    {
        s_arPointerX[nTouch] = nTouchX;
        s_arPointerY[nTouch] = nTouchY;
    }
    else
    {
        LogWarning("Invalid touch index in SetTouchPosition().");
        return;
    }
}


//*****************************************************************************
// SetControllerButton
//*****************************************************************************
void SetControllerButton(int nControllerButton,
                         int nControllerNumber)
{
    if (nControllerNumber >= 0 &&
        nControllerNumber <  VINPUT_MAX_CONTROLLERS)
    {
        if (nControllerButton >= VCONT_A &&
            nControllerButton <  VCONT_SELECT)
        {
            // Bias the controller button by VCONT_A to get correct array index
            s_arControllers[nControllerNumber].arButtons[nControllerButton - VCONT_A] = 1;
        }
    }
}

//*****************************************************************************
// ClearControllerButton
//*****************************************************************************
void ClearControllerButton(int nControllerButton,
                           int nControllerNumber)
{
    if (nControllerNumber >= 0 &&
        nControllerNumber <  VINPUT_MAX_CONTROLLERS)
    {
        if (nControllerButton >= VCONT_A &&
            nControllerButton <  VCONT_SELECT)
        {
            // Bias the controller button by VCONT_A to get correct array index
            s_arControllers[nControllerNumber].arButtons[nControllerButton - VCONT_A] = 0;
        }
    }
}

//*****************************************************************************
// IsControllerButtonDown
//*****************************************************************************
int IsControllerButtonDown(int nControllerButton,
                           int nControllerNumber)
{
    if (nControllerNumber >= 0 &&
        nControllerNumber <  VINPUT_MAX_CONTROLLERS)
    {
        if (nControllerButton >= VCONT_A &&
            nControllerButton <  VCONT_SELECT)
        {
            // Bias the controller button by VCONT_A to get correct array index
            return s_arControllers[nControllerNumber].arButtons[nControllerButton - VCONT_A];
        }
    }

    return 0;
}

//*****************************************************************************
// SetControllerAxisValue
//*****************************************************************************
void SetControllerAxisValue(int   nControllerAxis,
                             float fAxisValue, 
                             int   nControllerNumber)
{
    if (nControllerNumber >= 0 &&
        nControllerNumber <  VINPUT_MAX_CONTROLLERS)
    {
        if (nControllerAxis >= VCONT_AXIS_X &&
            nControllerAxis <  VCONT_AXIS_RTRIGGER)
        {
            s_arControllers[nControllerNumber].arAxes[nControllerAxis] = fAxisValue;
        }
    }
}

//*****************************************************************************
// GetControllerAxisValue
//*****************************************************************************
float GetControllerAxisValue(int nControllerAxis,
                             int nControllerNumber)
{
    if (nControllerNumber >= 0 &&
        nControllerNumber <  VINPUT_MAX_CONTROLLERS)
    {
        if (nControllerAxis >= VCONT_AXIS_X &&
            nControllerAxis <  VCONT_AXIS_RTRIGGER)
        {
            return s_arControllers[nControllerNumber].arAxes[nControllerAxis];
        }
    }

    return 0.0f;
}

int GetControllerIndex(int nInputDevice)
{
    int i = 0;

    for (i = 0; i < s_nNumControllers; i++)
    {
        if (s_arControllers[i].nDevice == nInputDevice)
        {
            return i;
        }
    }

    if (i < VINPUT_MAX_CONTROLLERS)
    {
        AssignController(nInputDevice);
        return i;
    }

    return -1;
}

void AssignController(int nInputDevice)
{
    if (s_nNumControllers < VINPUT_MAX_CONTROLLERS)
    {
        s_arControllers[s_nNumControllers].nDevice = nInputDevice;
        s_nNumControllers++;
    }
}

int IsControllerConnected(int nIndex)
{
    if (nIndex < s_nNumControllers)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void ShowSoftKeyboard()
{
    s_nKeyboardEnable = 1;
}

void HideSoftKeyboard()
{
    s_nKeyboardEnable = 0;
}

void InitializeSoftKeyboard()
{
    if (s_pKeyboard == 0)
    {
        s_pKeyboard = new Keyboard();
    }
}

int IsSoftKeyboardEnabled()
{
    return s_nKeyboardEnable;
}

void RenderSoftKeyboard()
{
    if (s_pKeyboard != 0)
    {
        s_pKeyboard->Render();
    }
    else
    {
        LogWarning("Soft Keyboard not initialized.");
    }
}

void UpdateSoftKeyboard()
{
    if (s_pKeyboard != 0)
    {
        s_pKeyboard->Update();
    }
}

int IsPointerDownRaw(int nPointer)
{
    if (nPointer >= 0 &&
        nPointer <  VINPUT_MAX_TOUCHES)
    {
        // If either the left mouse button is down or the specified
        // touch index is down, then return 1.
        if ((s_arButtons[VBUTTON_LEFT] != 0) ||
            (s_arTouches[nPointer]     != 0))
        {
            return 1;
        }
    }

    return 0;
}

int IsTouchDownRaw(int nTouch)
{
    if (nTouch >= 0 &&
        nTouch <  VINPUT_MAX_TOUCHES)
    {
        return s_arTouches[nTouch];
    }

    return 0;
}

int CharToKey(char cTarget)
{
     cTarget = toupper(cTarget);

#if defined (ANDROID)
     if (cTarget >= '0' &&
         cTarget <= '9')
     {
        return VKEY_0 + (cTarget - '0');
     }
     else if (cTarget >= 'A' &&
              cTarget <= 'Z')
     {
        return VKEY_A + (cTarget - 'A');
     }
#elif defined (WINDOWS)
    return static_cast<int>(cTarget);
#endif
}
