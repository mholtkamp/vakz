#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "Rect.h"
#include "Text.h"

class Keyboard
{
public:

    Keyboard();
    ~Keyboard();

    void Render();

    void Update();

    void SetColors(float* arBackColor,
                   float* arKeyColor,
                   float* arTextColor);

    enum KeyboardEnum
    {
        NUM_KEYS              = 43,
        NUM_NORMAL_KEYS       = 40,
        NUM_SPECIAL_KEYS      = 3,

        INDEX_SPACE_KEY       = 40,
        INDEX_BACK_KEY        = 41,
        INDEX_ENTER_KEY       = 42,

        TRIANGLES_PER_KEY     = 2,
        VERTICES_PER_TRIANGLE = 3,
        FLOATS_PER_VERTEX     = 2
    };

private:

    void GenerateRects();

    void GenerateVertexArrays();

    Rect m_arRects[NUM_KEYS];
    char m_arChars[NUM_KEYS];

    unsigned int m_hPositionKeyVBO;
    unsigned int m_hPositionTextVBO;
    unsigned int m_hTexCoordTextVBO;

    float m_arBackColor[4];
    float m_arKeyColor[4];
    float m_arTextColor[4];

    Font* m_pFont;

    Text m_txSpace;
    Text m_txBack;
    Text m_txEnter;
};

#endif
