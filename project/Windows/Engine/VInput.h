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

enum VInputEnum
{
    VINPUT_MAX_KEYS = 256
};

void SetKey(int nKey);

void ClearKey(int nKey);

void ClearAllKeys();

int IsKeyDown(int nKey);

#elif defined(ANDROID)

#endif
#endif