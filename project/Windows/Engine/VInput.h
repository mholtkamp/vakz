#ifndef VINPUT_H
#define VINPUT_H

#if defined(WINDOWS)
enum VakzKeyEnum
{
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
    VINPUT_MAX_KEYS    = 256,
    VINPUT_MAX_BUTTONS = 16,
    VINPUT_MAX_TOUCHES = 16
};

void SetKey(int nKey);

void ClearKey(int nKey);

int IsKeyDown(int nKey);

void SetButton(int nButton);

void ClearButton(int nButton);

int IsButtonDown(int nButton);

void SetTouch(int nTouch);

void ClearTouch(int nTouch);

int IsTouchDown(int nTouch);

int IsPointerDown(int nPointer = 0);

void GetMousePosition(int& nMouseX,
                      int& nMouseY);

void GetTouchPosition(int& nTouchX,
                      int& nTouchY);

void GetPointerPosition(int& nPointerX,
                        int& nPointerY,
                        int  nPointer = 0);

void SetMousePosition(int nMouseX,
                      int nMouseY);

void SetTouchPosition(int nTouchX,
                      int nTouchY,
                      int nTouch = 0);



#endif
