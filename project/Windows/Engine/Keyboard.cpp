#include "Keyboard.h"
#include "VGL.h"
#include "Log.h"
#include "ResourceLibrary.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#define DEFAULT_CHAR_WIDTH   0.05f
#define DEFAULT_CHAR_HEIGHT  0.05f
#define CHAR_UNIT            0.125f
#define TEXCOORD_BUFFER      0.004f
#define KEY_WIDTH            0.12f
#define KEY_HEIGHT           0.12f

static float s_arQuadPosition[8] = {-1.0f, -1.0f,
                                    -1.0f,  0.0f,
                                     1.0f,  0.0f,
                                     1.0f, -1.0f};

Keyboard::Keyboard()
{
    memset(m_arChars, 0, NUM_KEYS);
    memset(m_arPosition,
           0,
           NUM_KEYS              *
           TRIANGLES_PER_KEY     *
           VERTICES_PER_TRIANGLE * 
           FLOATS_PER_VERTEX     *
           sizeof(float));
    memset(m_arTexCoord,
           0,
           NUM_KEYS              *
           TRIANGLES_PER_KEY     * 
           VERTICES_PER_TRIANGLE *
           FLOATS_PER_VERTEX     *
           sizeof(float));
    
    // Default Colors
    m_arBackColor[0] = 0.2f;
    m_arBackColor[1] = 0.2f;
    m_arBackColor[2] = 0.2f;
    m_arBackColor[3] = 1.0f;

    m_arKeyColor[0] = 0.5f;
    m_arKeyColor[1] = 0.5f;
    m_arKeyColor[2] = 0.5f;
    m_arKeyColor[3] = 1.0f;

    m_arTextColor[0] = 0.0f;
    m_arTextColor[1] = 0.0f;
    m_arTextColor[2] = 0.0f;
    m_arTextColor[3] = 1.0f;

    m_pFont = reinterpret_cast<Font*>(ResourceLibrary::GetDefaultFont());

    GenerateRects();
    GenerateVertexArrays();
}

Keyboard::~Keyboard()
{

}

void Keyboard::Render()
{
    unsigned int hProg = GetShaderProgram(QUAD_PROGRAM);
    int hColor    = -1;
    int hType     = -1;
    int hPosition = -1;
    int hTexCoord = -1;

    glUseProgram(hProg);
    hColor = glGetUniformLocation(hProg, "uColor");
    hType  = glGetUniformLocation(hProg, "uType");
    hPosition = glGetAttribLocation(hProg, "aPosition");
    hTexCoord = glGetAttribLocation(hProg, "aTexCoord");

    // Draw the background color quad first.
    glBindTexture(GL_TEXTURE_2D, 0);

    glUniform4fv(hColor, 1, m_arBackColor);
    glUniform1i(hType, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glEnableVertexAttribArray(hPosition);
    glVertexAttribPointer(hPosition,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          s_arQuadPosition);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    // Now draw the key colored quads.
    glUniform4fv(hColor, 1, m_arKeyColor);
    glUniform1i(hType, 0);

    glEnableVertexAttribArray(hPosition);
    glVertexAttribPointer(hPosition,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          m_arPosition);

    glDrawArrays(GL_TRIANGLES, 0, NUM_KEYS * 6);

    // Now draw the key letters
    m_pFont->SetRenderState(hProg);
    m_pFont->SetFiltering(Texture::NEAREST);

    glUniform4fv(hColor, 1, m_arTextColor);
    glUniform1i(hType, 1);

    glEnableVertexAttribArray(hPosition);
    glEnableVertexAttribArray(hTexCoord);
    glVertexAttribPointer(hPosition,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          m_arPosition);
    glVertexAttribPointer(hTexCoord,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          m_arTexCoord);

    glDrawArrays(GL_TRIANGLES, 0, NUM_KEYS * 6);
    m_pFont->SetFiltering(Texture::LINEAR);
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

void Keyboard::GenerateRects()
{
    int i = 0;

    char* szRow1 = "1234567890";
    char* szRow2 = "QWERTYUIOP";
    char* szRow3 = "ASDFGHJKL?";
    char* szRow4 = "ZXCVBNM,./";

    int nLenRow1 = strlen(szRow1);
    int nLenRow2 = strlen(szRow2);
    int nLenRow3 = strlen(szRow3);
    int nLenRow4 = strlen(szRow4);

    // Row 1
    for (i = 0; i < nLenRow1; i++)
    {
        m_arRects[i].m_fX = -0.8f + 0.15f*i;
        m_arRects[i].m_fY = -0.2f;
        m_arRects[i].m_fWidth  = KEY_WIDTH;
        m_arRects[i].m_fHeight = KEY_HEIGHT;

        m_arChars[i] = szRow1[i];
    }

    // Row 2
    for (i = 0; i < nLenRow2; i++)
    {
        m_arRects[i + nLenRow1].m_fX = -0.8f + 0.15f*i;
        m_arRects[i + nLenRow1].m_fY = -0.38f;
        m_arRects[i + nLenRow1].m_fWidth  = KEY_WIDTH;
        m_arRects[i + nLenRow1].m_fHeight = KEY_HEIGHT;

        m_arChars[i + nLenRow1] = szRow2[i];
    }

    // Row 3
    for (i = 0; i < nLenRow3; i++)
    {
        m_arRects[i + nLenRow1 + nLenRow2].m_fX = -0.8f + 0.15f*i;
        m_arRects[i + nLenRow1 + nLenRow2].m_fY = -0.56f;
        m_arRects[i + nLenRow1 + nLenRow2].m_fWidth  = KEY_WIDTH;
        m_arRects[i + nLenRow1 + nLenRow2].m_fHeight = KEY_HEIGHT;

        m_arChars[i + nLenRow1  + nLenRow2] = szRow3[i];
    }

    // Row 4
    for (i = 0; i < nLenRow4; i++)
    {
        m_arRects[i + nLenRow1 + nLenRow2 + nLenRow3].m_fX = -0.8f + 0.15f*i;
        m_arRects[i + nLenRow1 + nLenRow2 + nLenRow3].m_fY = -0.74f;
        m_arRects[i + nLenRow1 + nLenRow2 + nLenRow3].m_fWidth  = KEY_WIDTH;
        m_arRects[i + nLenRow1 + nLenRow2 + nLenRow3].m_fHeight = KEY_HEIGHT;

        m_arChars[i + nLenRow1  + nLenRow2 + nLenRow3] = szRow4[i];
    }
}

void Keyboard::GenerateVertexArrays()
{
    int i               = 0;
    float fCharWidth    = DEFAULT_CHAR_WIDTH;
    float fCharHeight   = DEFAULT_CHAR_HEIGHT;

    for (i = 0; i < NUM_KEYS; i++)
    {
        unsigned int index; 
        float xIndex;
        float yIndex;
        char target = (char) toupper(m_arChars[i]);

        if(target == '\0')         // Breakout on null terminating char
            break;
        else if(target >= ' ' &&   // Valid character range
                target <= 'Z')
        {
            index = target - 0x20;
        }
        else if (target == '\n')
        {
            index = ' ' - 0x20;
        }
        else
            // Do not render the rest, dip out.
            // (this might need to be changed later)
            break;

        yIndex = (float) (index/8);
        xIndex = (float) (index%8);

        // Each character will be drawn with two triangles like so
        //    2--3  5
        //    | /  /|
        //    |/  / |
        //    1  4--6

        // Texcoords
        m_arTexCoord[i*12 + 0] = CHAR_UNIT * xIndex;
        m_arTexCoord[i*12 + 1] = 1.0f - CHAR_UNIT * (yIndex+1.0f);

        m_arTexCoord[i*12 + 2] = CHAR_UNIT * xIndex;
        m_arTexCoord[i*12 + 3] = 1.0f - CHAR_UNIT * (yIndex) - TEXCOORD_BUFFER; 

        m_arTexCoord[i*12 + 4] = CHAR_UNIT * (xIndex+1.0f);
        m_arTexCoord[i*12 + 5] = 1.0f - CHAR_UNIT * (yIndex) - TEXCOORD_BUFFER; 

        m_arTexCoord[i*12 + 6] = CHAR_UNIT * xIndex;
        m_arTexCoord[i*12 + 7] = 1.0f - CHAR_UNIT * (yIndex+1.0f);

        m_arTexCoord[i*12 + 8] = CHAR_UNIT * (xIndex+1.0f);
        m_arTexCoord[i*12 + 9] = 1.0f - CHAR_UNIT * (yIndex) - TEXCOORD_BUFFER; 

        m_arTexCoord[i*12 + 10] = CHAR_UNIT * (xIndex+1.0f);
        m_arTexCoord[i*12 + 11] = 1.0f - CHAR_UNIT * (yIndex+1.0f);

        // Position
        m_arPosition[i*12 + 0] = m_arRects[i].m_fX;
        m_arPosition[i*12 + 1] = m_arRects[i].m_fY;

        m_arPosition[i*12 + 2] = m_arRects[i].m_fX;
        m_arPosition[i*12 + 3] = m_arRects[i].m_fY + m_arRects[i].m_fHeight;

        m_arPosition[i*12 + 4] = m_arRects[i].m_fX + m_arRects[i].m_fWidth;
        m_arPosition[i*12 + 5] = m_arRects[i].m_fY + m_arRects[i].m_fHeight;

        m_arPosition[i*12 + 6] = m_arRects[i].m_fX;
        m_arPosition[i*12 + 7] = m_arRects[i].m_fY;

        m_arPosition[i*12 + 8] = m_arRects[i].m_fX + m_arRects[i].m_fWidth;
        m_arPosition[i*12 + 9] = m_arRects[i].m_fY + m_arRects[i].m_fHeight;

        m_arPosition[i*12 + 10] = m_arRects[i].m_fX + m_arRects[i].m_fWidth;
        m_arPosition[i*12 + 11] = m_arRects[i].m_fY;
    }
}
