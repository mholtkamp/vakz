#ifndef TEXT_H
#define TEXT_H

#include "Glyph.h"
#include "Font.h"

class Text : public Glyph
{

public:

    //## **********************************************************************
    //## Constructor
    //## **********************************************************************
    Text();

    //## **********************************************************************
    //## Destructor
    //## **********************************************************************
    ~Text();

    //## **********************************************************************
    //## Render
    //##
    //## Renders the text string using the characters assigned with the 
    //## SetText() function. 
    //## **********************************************************************
    void Render();

    //## **********************************************************************
    //## SetScale
    //## 
    //## Sets the scale of the text. Default X/Y scaling are both equal to 1.
    //## 
    //## Input:
    //##   fX - x scaling factor.
    //##   fY - y scaling factor.
    //## **********************************************************************
    void SetScale(float fX,
                  float fY);

    //## **********************************************************************
    //## SetLineSpacing
    //##
    //## Sets the line spacing for the entire Text object. Line spacing 
    //## determines the distance between consecutive lines separated by 
    //## '\n' characters when rendering the text string. The value of the line
    //## spacing should be a fraction, where 1.0f indicates 1 full character 
    //## space between lines. 0.5f would be half a character, 2.0f would be
    //## two characters, etc.
    //##
    //## Input:
    //##   fLineSpacing - line spacing amount (fraction)
    //## **********************************************************************
    void SetLineSpacing(float fLineSpacing);

    //## **********************************************************************
    //## SetText
    //##
    //## Sets the Text object's text string. The text string contains the 
    //## characters that will be drawn to the screen.
    //##
    //## Input:
    //##   pText - null terminated string
    //## **********************************************************************
    void SetText(const char* pText);

    enum TextEnum
    {
        DEFAULT_BUFFER_SIZE = 128,

        TRIANGLES_PER_CHARACTER = 2,
        VERTICES_PER_TRIANGLE   = 3,
        FLOATS_PER_TRIANGLE     = 12
    };

private:

    //## Generates positions and texcoords
    void GenerateVertexArray(float** pArray,
                             int     nTextLength);

    //## Size of text string buffer in bytes
    int m_nBufferSize;

    //## Text string
    char* m_pText;

    //## Length of the text segment
    int m_nTextLength;

    //## X Scaling factor
    float m_fScaleX;

    //## Y Scaling factor
    float m_fScaleY;

    //## Variable to store spacing between lines
    float m_fLineSpacing;

    //## Pointer to font to use when rendering.
    Font* m_pFont;

    //## Handle to vertex buffer object.
    unsigned int m_hVBO;
};

#endif
