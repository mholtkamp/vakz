#include "Keyboard.h"
#include "VGL.h"
#include "Log.h"
#include "VInput.h"
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

#define TEXT_OFFSET 0.015f

static float s_arQuadPosition[8] = {-1.0f, -1.0f,
                                    -1.0f,  0.0f,
                                     1.0f,  0.0f,
                                     1.0f, -1.0f};

//*****************************************************************************
// Constructor
//*****************************************************************************
Keyboard::Keyboard()
{
    memset(m_arChars, 0, NUM_KEYS);

    m_hPositionKeyVBO  = 0;
    m_hPositionTextVBO = 0;
    m_hTexCoordTextVBO = 0;
    
    // Default Colors
    m_arBackColor[0] = 0.15f;
    m_arBackColor[1] = 0.1f;
    m_arBackColor[2] = 0.1f;
    m_arBackColor[3] = 1.0f;

    m_arKeyColor[0] = 0.7f;
    m_arKeyColor[1] = 0.7f;
    m_arKeyColor[2] = 0.7f;
    m_arKeyColor[3] = 1.0f;

    m_arTextColor[0] = 0.0f;
    m_arTextColor[1] = 0.0f;
    m_arTextColor[2] = 0.0f;
    m_arTextColor[3] = 1.0f;

    m_pFont = reinterpret_cast<Font*>(ResourceLibrary::GetDefaultFont());

    GenerateRects();
    GenerateVertexArrays();
}

//*****************************************************************************
// Destructor
//*****************************************************************************
Keyboard::~Keyboard()
{

}

//*****************************************************************************
// Render
//*****************************************************************************
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
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glUniform4fv(hColor, 1, m_arBackColor);
    glUniform1i(hType, 0);

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

    glBindBuffer(GL_ARRAY_BUFFER, m_hPositionKeyVBO);
    glEnableVertexAttribArray(hPosition);
    glVertexAttribPointer(hPosition,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          0);

    glDrawArrays(GL_TRIANGLES, 0, NUM_KEYS * 6);

    // Now draw the key letters
    m_pFont->SetRenderState(hProg);
    m_pFont->SetFiltering(Texture::NEAREST);

    glUniform4fv(hColor, 1, m_arTextColor);
    glUniform1i(hType, 1);

    glBindBuffer(GL_ARRAY_BUFFER, m_hPositionTextVBO);
    glEnableVertexAttribArray(hPosition);
    
    glVertexAttribPointer(hPosition,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          0);

    glBindBuffer(GL_ARRAY_BUFFER, m_hTexCoordTextVBO);
    glEnableVertexAttribArray(hTexCoord);
    glVertexAttribPointer(hTexCoord,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          0);

    glDrawArrays(GL_TRIANGLES, 0, NUM_KEYS * 6);
    m_pFont->SetFiltering(Texture::LINEAR);

    // Render texts for special keys
    m_txSpace.Render();
    m_txBack.Render();
    m_txEnter.Render();
}

//*****************************************************************************
// Update
//*****************************************************************************
void Keyboard::Update()
{
    int i    = 0;
    float fX = 0.0f;
    float fY = 0.0f;

    ClearAllKeys();
    
    // Check first pointer
    if (IsPointerDownRaw(0))
    {
        GetPointerPositionNormalized(fX, fY, 0);

        for (i = 0; i < NUM_NORMAL_KEYS; i++)
        {
            if (m_arRects[i].Contains(fX, fY))
            {
                // Set the key as down
                SetKey(CharToKey(m_arChars[i]));
            }
        }

        if (m_arRects[INDEX_SPACE_KEY].Contains(fX, fY))
        {
            SetKey(VKEY_SPACE);
        }

        if (m_arRects[INDEX_BACK_KEY].Contains(fX, fY))
        {
            SetKey(VKEY_BACKSPACE);
        }

        if (m_arRects[INDEX_ENTER_KEY].Contains(fX, fY))
        {
            SetKey(VKEY_ENTER);
        }

    }

    // Check second pointer
    if (IsTouchDownRaw(1))
    {
        GetPointerPositionNormalized(fX, fY, 1);

        for (i = 0; i < NUM_NORMAL_KEYS; i++)
        {
            if (m_arRects[i].Contains(fX, fY))
            {
                // Set the key as down
                SetKey(CharToKey(m_arChars[i]));
            }
        }

        if (m_arRects[INDEX_SPACE_KEY].Contains(fX, fY))
        {
            SetKey(VKEY_SPACE);
        }

        if (m_arRects[INDEX_BACK_KEY].Contains(fX, fY))
        {
            SetKey(VKEY_BACKSPACE);
        }

        if (m_arRects[INDEX_ENTER_KEY].Contains(fX, fY))
        {
            SetKey(VKEY_ENTER);
        }
    }
}

//*****************************************************************************
// SetColors
//*****************************************************************************
void Keyboard::SetColors(float* arBackColor,
                         float* arKeyColor,
                         float* arTextColor)
{
    memcpy(m_arBackColor, arBackColor, 4 * sizeof(float));
    memcpy(m_arKeyColor,  arKeyColor,  4 * sizeof(float));
    memcpy(m_arTextColor, arTextColor, 4 * sizeof(float));
}

//*****************************************************************************
// GenerateRects
//*****************************************************************************
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

    
    // Special keys
    i = nLenRow1 + nLenRow2 + nLenRow3 + nLenRow4;

    // Space
    m_arRects[i].m_fX      = -0.25f;
    m_arRects[i].m_fY      = -0.96f;
    m_arRects[i].m_fWidth  = 0.5f;
    m_arRects[i].m_fHeight = 0.16f;
    m_arChars[i] = ' ';
    m_txSpace.SetPosition(m_arRects[i].m_fX + 0.11f,
                          m_arRects[i].m_fY + 0.05f);
    m_txSpace.SetText("Space");
    i++;

    // Backspace
    m_arRects[i].m_fX      = 0.73f;
    m_arRects[i].m_fY      = -0.24f;
    m_arRects[i].m_fWidth  = 0.24f;
    m_arRects[i].m_fHeight = 0.16f;
    m_arChars[i] = ' ';
    m_txBack.SetPosition(m_arRects[i].m_fX + 0.03f,
                         m_arRects[i].m_fY + 0.05f);
    m_txBack.SetText("Back");
    i++;

    // Enter
    m_arRects[i].m_fX      = 0.73f;
    m_arRects[i].m_fY      = -0.565f;
    m_arRects[i].m_fWidth  = 0.24f;
    m_arRects[i].m_fHeight = 0.16f;
    m_arChars[i] = ' ';
    m_txEnter.SetPosition(m_arRects[i].m_fX + 0.03f,
                          m_arRects[i].m_fY + 0.05f);
    m_txEnter.SetScale(0.75f, 1.0f);
    m_txEnter.SetText("Enter");
    i++;
}

//*****************************************************************************
// GenerateVertexArrays
//*****************************************************************************
void Keyboard::GenerateVertexArrays()
{
    int i                 = 0;
    float fCharWidth      = DEFAULT_CHAR_WIDTH;
    float fCharHeight     = DEFAULT_CHAR_HEIGHT;
    float* arPositionKey  = new float[NUM_KEYS * TRIANGLES_PER_KEY * VERTICES_PER_TRIANGLE * FLOATS_PER_VERTEX];
    float* arPositionText = new float[NUM_KEYS * TRIANGLES_PER_KEY * VERTICES_PER_TRIANGLE * FLOATS_PER_VERTEX];
    float* arTexCoordText = new float[NUM_KEYS * TRIANGLES_PER_KEY * VERTICES_PER_TRIANGLE * FLOATS_PER_VERTEX];

    memset(arPositionKey,
           0,
           NUM_KEYS              *
           TRIANGLES_PER_KEY     *
           VERTICES_PER_TRIANGLE * 
           FLOATS_PER_VERTEX     *
           sizeof(float));
    memset(arPositionText,
           0,
           NUM_KEYS              *
           TRIANGLES_PER_KEY     *
           VERTICES_PER_TRIANGLE * 
           FLOATS_PER_VERTEX     *
           sizeof(float));
    memset(arTexCoordText,
           0,
           NUM_KEYS              *
           TRIANGLES_PER_KEY     * 
           VERTICES_PER_TRIANGLE *
           FLOATS_PER_VERTEX     *
           sizeof(float));

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
        arTexCoordText[i*12 + 0] = CHAR_UNIT * xIndex;
        arTexCoordText[i*12 + 1] = 1.0f - CHAR_UNIT * (yIndex+1.0f);

        arTexCoordText[i*12 + 2] = CHAR_UNIT * xIndex;
        arTexCoordText[i*12 + 3] = 1.0f - CHAR_UNIT * (yIndex) - TEXCOORD_BUFFER; 

        arTexCoordText[i*12 + 4] = CHAR_UNIT * (xIndex+1.0f);
        arTexCoordText[i*12 + 5] = 1.0f - CHAR_UNIT * (yIndex) - TEXCOORD_BUFFER; 

        arTexCoordText[i*12 + 6] = CHAR_UNIT * xIndex;
        arTexCoordText[i*12 + 7] = 1.0f - CHAR_UNIT * (yIndex+1.0f);

        arTexCoordText[i*12 + 8] = CHAR_UNIT * (xIndex+1.0f);
        arTexCoordText[i*12 + 9] = 1.0f - CHAR_UNIT * (yIndex) - TEXCOORD_BUFFER; 

        arTexCoordText[i*12 + 10] = CHAR_UNIT * (xIndex+1.0f);
        arTexCoordText[i*12 + 11] = 1.0f - CHAR_UNIT * (yIndex+1.0f);

        // Position
        arPositionKey[i*12 + 0] = m_arRects[i].m_fX;
        arPositionKey[i*12 + 1] = m_arRects[i].m_fY;
       
        arPositionKey[i*12 + 2] = m_arRects[i].m_fX;
        arPositionKey[i*12 + 3] = m_arRects[i].m_fY + m_arRects[i].m_fHeight;

        arPositionKey[i*12 + 4] = m_arRects[i].m_fX + m_arRects[i].m_fWidth;
        arPositionKey[i*12 + 5] = m_arRects[i].m_fY + m_arRects[i].m_fHeight;

        arPositionKey[i*12 + 6] = m_arRects[i].m_fX;
        arPositionKey[i*12 + 7] = m_arRects[i].m_fY;

        arPositionKey[i*12 + 8] = m_arRects[i].m_fX + m_arRects[i].m_fWidth;
        arPositionKey[i*12 + 9] = m_arRects[i].m_fY + m_arRects[i].m_fHeight;

        arPositionKey[i*12 + 10] = m_arRects[i].m_fX + m_arRects[i].m_fWidth;
        arPositionKey[i*12 + 11] = m_arRects[i].m_fY;

        // Text Positions
        arPositionText[i*12 + 0] = m_arRects[i].m_fX + TEXT_OFFSET;
        arPositionText[i*12 + 1] = m_arRects[i].m_fY + TEXT_OFFSET;
       
        arPositionText[i*12 + 2] = m_arRects[i].m_fX + TEXT_OFFSET;
        arPositionText[i*12 + 3] = m_arRects[i].m_fY + m_arRects[i].m_fHeight - TEXT_OFFSET;

        arPositionText[i*12 + 4] = m_arRects[i].m_fX + m_arRects[i].m_fWidth - TEXT_OFFSET;
        arPositionText[i*12 + 5] = m_arRects[i].m_fY + m_arRects[i].m_fHeight - TEXT_OFFSET;

        arPositionText[i*12 + 6] = m_arRects[i].m_fX + TEXT_OFFSET;
        arPositionText[i*12 + 7] = m_arRects[i].m_fY + TEXT_OFFSET;

        arPositionText[i*12 + 8] = m_arRects[i].m_fX + m_arRects[i].m_fWidth - TEXT_OFFSET;
        arPositionText[i*12 + 9] = m_arRects[i].m_fY + m_arRects[i].m_fHeight - TEXT_OFFSET;

        arPositionText[i*12 + 10] = m_arRects[i].m_fX + m_arRects[i].m_fWidth - TEXT_OFFSET;
        arPositionText[i*12 + 11] = m_arRects[i].m_fY + TEXT_OFFSET;
    }

    // Create the vertex buffers
    glGenBuffers(1, &m_hPositionKeyVBO);
    glGenBuffers(1, &m_hPositionTextVBO);
    glGenBuffers(1, &m_hTexCoordTextVBO);

    // Fill the new vertex buffer objects
    glBindBuffer(GL_ARRAY_BUFFER, m_hPositionKeyVBO);
    glBufferData(GL_ARRAY_BUFFER,
                 NUM_KEYS * TRIANGLES_PER_KEY * VERTICES_PER_TRIANGLE * FLOATS_PER_VERTEX * sizeof(float),
                 arPositionKey,
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_hPositionTextVBO);
    glBufferData(GL_ARRAY_BUFFER,
                 NUM_KEYS * TRIANGLES_PER_KEY * VERTICES_PER_TRIANGLE * FLOATS_PER_VERTEX * sizeof(float),
                 arPositionText,
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_hTexCoordTextVBO);
    glBufferData(GL_ARRAY_BUFFER,
                 NUM_KEYS * TRIANGLES_PER_KEY * VERTICES_PER_TRIANGLE * FLOATS_PER_VERTEX * sizeof(float),
                 arTexCoordText,
                 GL_STATIC_DRAW);

    // Now destroy the arrays allocated on heap as they are now copied to VRAM
    delete [] arPositionKey;
    delete [] arPositionText;
    delete [] arTexCoordText;
}
