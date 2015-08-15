#include "Keyboard.h"
#include <string.h>

Keyboard::Keyboard()
{
    memset(m_arPosition,
           0,
           NUM_KEYS *
           TRIANGLES_PER_KEY *
           POSITION_FLOATS_PER_TRIANGLE *
           sizeof(float));

    memset(m_arTexCoord,
           0,
           NUM_KEYS                     *
           TRIANGLES_PER_KEY            * 
           TEXCOORD_FLOATS_PER_TRIANGLE * 
           sizeof(float));

    memset(m_arBackColor, 0, 4 * sizeof(float));
    memset(m_arKeyColor,  0, 4 * sizeof(float));
    memset(m_arTextColor, 0, 4 * sizeof(float));

    GenerateVertexArrays();
}

Keyboard::~Keyboard()
{

}

void Keyboard::Render()
{

}

void Keyboard::Update()
{

}

void Keyboard::SetColors(float* arBackColor,
                         float* arKeyColor,
                         float* arTextColor)
{
    memcpy(m_arBackColor, arBackColor, 4 * sizeof(float));
    memcpy(m_arKeyColor,  arKeyColor,  4 * sizeof(float));
    memcpy(m_arTextColor, arTextColor, 4 * sizeof(float));
}

void Keyboard::GenerateVertexArrays()
{

}

