#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "Rect.h"
#include "Text.h"

class Keyboard
{
public:

    //## **********************************************************************
    //## Constructor
    //## **********************************************************************
    Keyboard();

    //## **********************************************************************
    //## Destructor
    //## **********************************************************************
    ~Keyboard();

    //## **********************************************************************
    //## Render
    //## 
    //## Renders the keyboard to the screen. The keyboard takes up the bottom
    //## half of the screen.
    //## **********************************************************************
    void Render();

    //## **********************************************************************
    //## Update
    //##
    //## Checks what keys are being pressed and sets them in VInput for the
    //## user to query.
    //## **********************************************************************
    void Update();

    //## **********************************************************************
    //## SetColors
    //## 
    //## Sets the colors that are used to render the keyboard. The inputs 
    //## should be provided as size 4 float arrays. The floating point values
    //## represent the red, green, blue, and alpha components of the color.
    //##
    //## Input:
    //##   arBackColor - the color of the background quad that covers half
    //##                 of the screen.
    //##   arKeyColor  - the color of the rectangles that represent the keys.
    //##   arTextColor - color of the letters printed on top of the key rects.
    //## **********************************************************************
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

    //## Generates the bounds of the rectangles that represent the keys
    void GenerateRects();

    //## Generates position/texcoord vertex data for keys
    void GenerateVertexArrays();

    //## Array of rectangles to test if key is being pressed.
    Rect m_arRects[NUM_KEYS];

    //## Array of characters that each key represent. Same indices
    //## used as the Rect array.
    char m_arChars[NUM_KEYS];

    //## Vertex buffer object handles
    unsigned int m_hPositionKeyVBO;
    unsigned int m_hPositionTextVBO;
    unsigned int m_hTexCoordTextVBO;

    //## Colors
    float m_arBackColor[4];
    float m_arKeyColor[4];
    float m_arTextColor[4];

    //## Pointer to font to use for letters on keys
    Font* m_pFont;

    //## Text objects for special keys
    Text m_txSpace;
    Text m_txBack;
    Text m_txEnter;
};

#endif
