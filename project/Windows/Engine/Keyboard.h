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
        NUM_KEYS              = 40,
        TRIANGLES_PER_KEY     = 2,
        VERTICES_PER_TRIANGLE = 3,
        FLOATS_PER_VERTEX     = 2
    };

private:

    void GenerateRects();

    void GenerateVertexArrays();

    Rect m_arRects[NUM_KEYS];
    char m_arChars[NUM_KEYS];

    float m_arPosition[NUM_KEYS * TRIANGLES_PER_KEY * VERTICES_PER_TRIANGLE * FLOATS_PER_VERTEX];
    float m_arTexCoord[NUM_KEYS * TRIANGLES_PER_KEY * VERTICES_PER_TRIANGLE * FLOATS_PER_VERTEX];

    float m_arBackColor[4];
    float m_arKeyColor[4];
    float m_arTextColor[4];

    Font* m_pFont;
};

#endif
