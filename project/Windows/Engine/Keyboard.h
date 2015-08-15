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
        NUM_KEYS            = 64,
        TRIANGLES_PER_KEY   = 2,
        POSITION_FLOATS_PER_TRIANGLE = 3,
        TEXCOORD_FLOATS_PER_TRIANGLE = 2
    };

private:

    void GenerateVertexArrays();

    Rect m_arRects[NUM_KEYS];

    float m_arPosition[NUM_KEYS * TRIANGLES_PER_KEY * POSITION_FLOATS_PER_TRIANGLE];
    float m_arTexCoord[NUM_KEYS * TRIANGLES_PER_KEY * TEXCOORD_FLOATS_PER_TRIANGLE];

    float m_arBackColor[4];
    float m_arKeyColor[4];
    float m_arTextColor[4];
};

#endif
