#include "VInput.h"
#include "Log.h"
#include <string.h>

static int s_arKeys[VINPUT_MAX_KEYS]       = {0};
static int s_arButtons[VINPUT_MAX_BUTTONS] = {0};
static int s_arTouches[VINPUT_MAX_TOUCHES] = {0};

static int s_arPointerX[VINPUT_MAX_TOUCHES] = {0};
static int s_arPointerY[VINPUT_MAX_TOUCHES] = {0};

void SetKey(int nKey)
{
    if (nKey >= 0 &&
        nKey <  VINPUT_MAX_KEYS)
    {
        s_arKeys[nKey] = 1;
    }
}

void ClearKey(int nKey)
{
    if (nKey >= 0 &&
        nKey <  VINPUT_MAX_KEYS)
    {
        s_arKeys[nKey] = 0;
    }
}

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

void SetButton(int nButton)
{
    if (nButton >= 0 &&
        nButton <  VINPUT_MAX_BUTTONS)
    {
        s_arButtons[nButton] = 1;
    }
}

void ClearButton(int nButton)
{
    if (nButton >= 0 &&
        nButton <  VINPUT_MAX_BUTTONS)
    {
        s_arButtons[nButton] = 0;
    }
}

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

void SetTouch(int nTouch)
{
    if (nTouch >= 0 &&
        nTouch <  VINPUT_MAX_TOUCHES)
    {
        s_arTouches[nTouch] = 1;
    }
}

void ClearTouch(int nTouch)
{
    if (nTouch >= 0 &&
        nTouch <  VINPUT_MAX_TOUCHES)
    {
        s_arTouches[nTouch] = 0;
    }
}

int IsTouchDown(int nTouch)
{
    if (nTouch >= 0 &&
        nTouch <  VINPUT_MAX_TOUCHES)
    {
        return s_arTouches[nTouch];
    }
    else
    {
        LogWarning("Invalid touch queried in IsTouchDown().");
        return 0;
    }
}

int IsPointerDown(int nPointer)
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

void GetMousePosition(int& nMouseX, int& nMouseY)
{
    // First pointer location is for mouse.
    nMouseX = s_arPointerX[0];
    nMouseY = s_arPointerY[0];
}


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

void GetPointerPosition(int& nPointerX,
                        int& nPointerY,
                        int nPointer)
{
    GetTouchPosition(nPointerX,
                     nPointerY,
                     nPointer);
}

void SetMousePosition(int nMouseX, int nMouseY)
{
    // First index in pointer array is mouse position.
    s_arPointerX[0] = nMouseX;
    s_arPointerY[0] = nMouseY;
}

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
