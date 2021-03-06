#include "Text.h"
#include "ResourceLibrary.h"
#include "VGL.h"
#include <string.h>
#include <ctype.h>

#define DEFAULT_LINE_SPACING 0.2f
#define DEFAULT_CHAR_SPACING -0.2f
#define DEFAULT_CHAR_WIDTH   0.05f
#define DEFAULT_CHAR_HEIGHT  0.05f
#define CHAR_UNIT_WIDTH      0.125f
#define CHAR_UNIT_HEIGHT     0.0625f
#define TEXCOORD_BUFFER      0.004f

//*****************************************************************************
// Constructor
//*****************************************************************************
Text::Text()
{
    // Overwrite default color
    m_arColor[0] = 0.0f;
    m_arColor[1] = 0.0f;
    m_arColor[2] = 0.0f;
    m_arColor[3] = 1.0f;

    m_nBufferSize  = DEFAULT_BUFFER_SIZE;
    m_pText        = new char[m_nBufferSize];
    m_nTextLength  = 0;
    m_fScaleX      = 1.0f;
    m_fScaleY      = 1.0f;
    m_fLineSpacing = DEFAULT_LINE_SPACING;
    m_fCharSpacing = DEFAULT_CHAR_SPACING;

    m_pFont = reinterpret_cast<Font*>(ResourceLibrary::GetDefaultFont());

    memset(m_pText, 0, m_nBufferSize);

    m_hVBO = 0;
}

//*****************************************************************************
// Destructor
//*****************************************************************************
Text::~Text()
{
    if (m_pText != 0)
    {
        delete [] m_pText;
        m_pText = 0;
    }

    if (m_hVBO != 0)
    {
        glDeleteBuffers(1, &m_hVBO);
        m_hVBO = 0;
    }
}

//*****************************************************************************
// Render
//*****************************************************************************
void Text::Render()
{
    unsigned int hProg = GetShaderProgram(TEXT_PROGRAM);
    int hColor    = -1;
    int hPosition = -1;
    int hTexCoord = -1;
    int hOrigin   = -1;
    int hScale    = -1;

    if (m_nVisible != 0)
    {
        glUseProgram(hProg);
        hColor    = glGetUniformLocation(hProg, "uColor");
        hOrigin   = glGetUniformLocation(hProg, "uOrigin");
        hScale    = glGetUniformLocation(hProg, "uScale");
        hPosition = glGetAttribLocation(hProg, "aPosition");
        hTexCoord = glGetAttribLocation(hProg, "aTexCoord");

        m_pFont->SetRenderState(hProg);
        glUniform4fv(hColor, 1, m_arColor);
        glUniform2f(hOrigin, m_fX, m_fY);
        glUniform2f(hScale, m_fScaleX, m_fScaleY);

        glBindBuffer(GL_ARRAY_BUFFER, m_hVBO);

        glEnableVertexAttribArray(hPosition);
        glEnableVertexAttribArray(hTexCoord);

        glVertexAttribPointer(hPosition,
                              2,
                              GL_FLOAT,
                              GL_FALSE,
                              (4 * sizeof(float)),
                              0);
        glVertexAttribPointer(hTexCoord,
                              2,
                              GL_FLOAT,
                              GL_FALSE,
                              (4 * sizeof(float)),
                              (void*) (2 * sizeof(float)));

        glDrawArrays(GL_TRIANGLES,
                     0,
                     TRIANGLES_PER_CHARACTER   * 
                        VERTICES_PER_TRIANGLE  *
                            m_nTextLength);
    }
}

//*****************************************************************************
// SetScale
//*****************************************************************************
void Text::SetScale(float fX,
                    float fY)
{
    m_fScaleX = fX;
    m_fScaleY = fY;
}

//*****************************************************************************
// SetLineSpacing
//*****************************************************************************
void Text::SetLineSpacing(float fLineSpacing)
{
    m_fLineSpacing = fLineSpacing;
}

//*****************************************************************************
// SetText
//*****************************************************************************
void Text::SetText(const char* pText)
{
    float* pVertexArray = 0;
    int nTextLength     = strlen(pText);

    // If the buffer is not big enough, allocate
    // a bigger buffer to hold the entire string 
    // including a null terminating character.
    if (nTextLength + 1 >= m_nBufferSize)
    {
        delete [] m_pText;
        m_pText = new char[nTextLength + 1];
        m_nBufferSize = nTextLength + 1;
    }

    // Clear the text buffer
    memset(m_pText, 0, m_nBufferSize);

    // Copy the new text string to the new buffer
    strcpy(m_pText, pText);

    // Generate the vertex array containing position and
    // texture coordinates needed for drawing. Each character
    // is drawn using two separate triangles.
    pVertexArray = new float[nTextLength * 
                             TRIANGLES_PER_CHARACTER *
                             FLOATS_PER_TRIANGLE *
                             sizeof(float)];

    GenerateVertexArray(&pVertexArray,
                         nTextLength);


    // Check if a VBO has already been created.
    // The VBO is created here instead instead of in the
    // constructor in case a Text object is created before
    // an opengl context has been created and made current.
    if (m_hVBO == 0)
    {
        glGenBuffers(1, &m_hVBO);
    }

    // Load the new VBO with the vertex data.
    glBindBuffer(GL_ARRAY_BUFFER, m_hVBO);
    glBufferData(GL_ARRAY_BUFFER,
                 nTextLength                 * 
                     TRIANGLES_PER_CHARACTER *
                         FLOATS_PER_TRIANGLE *
                             sizeof(float),
                 pVertexArray,
                 GL_DYNAMIC_DRAW);

    // After vertex data is sent to GPU, delete system memory.
    delete [] pVertexArray;

    // Set the new text length
    m_nTextLength = nTextLength;
}

//*****************************************************************************
// GenerateVertexArray
//*****************************************************************************
void Text::GenerateVertexArray(float** pArray,
                               int     nTextLength)
{
    int i                = 0;
    float* pVertexArray  = *pArray;
    float  fCharWidth    = DEFAULT_CHAR_WIDTH;
    float  fCharHeight   = DEFAULT_CHAR_HEIGHT;
    float    fCharOffX     = 0.0f;
    float  fCharOffY     = 0.0f;

    for (i = 0; i < nTextLength; i++)
    {
        unsigned int index; 
        float xIndex;
        float yIndex;
        char target = m_pText[i];
        float  fLowerDrop    = 0.0f;

        if (target == '\0')         // Breakout on null terminating char
            break;

        if (target == 'g' ||
            target == 'j' ||
            target == 'p' ||
            target == 'q' ||
            target == 'y')
        {
            fLowerDrop = fCharHeight * 0.25f;
        }
        
        index = target;

        yIndex = (float) (index/8);
        xIndex = (float) (index%16);

        // Each character will be drawn with two triangles like so
        //    2--3  5
        //    | /  /|
        //    |/  / |
        //    1  4--6

        // Texcoords
        pVertexArray[i*24 + 2] = CHAR_UNIT_WIDTH * xIndex;
        pVertexArray[i*24 + 3] = 1.0f - CHAR_UNIT_HEIGHT * (yIndex+1.0f);

        pVertexArray[i*24 + 6] = CHAR_UNIT_WIDTH * xIndex;
        pVertexArray[i*24 + 7] = 1.0f - CHAR_UNIT_HEIGHT * (yIndex) - TEXCOORD_BUFFER; 

        pVertexArray[i*24 + 10] = CHAR_UNIT_WIDTH * (xIndex+1.0f);
        pVertexArray[i*24 + 11] = 1.0f - CHAR_UNIT_HEIGHT * (yIndex) - TEXCOORD_BUFFER; 

        pVertexArray[i*24 + 14] = CHAR_UNIT_WIDTH * xIndex;
        pVertexArray[i*24 + 15] = 1.0f - CHAR_UNIT_HEIGHT * (yIndex+1.0f);

        pVertexArray[i*24 + 18] = CHAR_UNIT_WIDTH * (xIndex+1.0f);
        pVertexArray[i*24 + 19] = 1.0f - CHAR_UNIT_HEIGHT * (yIndex) - TEXCOORD_BUFFER; 

        pVertexArray[i*24 + 22] = CHAR_UNIT_WIDTH * (xIndex+1.0f);
        pVertexArray[i*24 + 23] = 1.0f - CHAR_UNIT_HEIGHT * (yIndex+1.0f);

        // Position
        pVertexArray[i*24 + 0] = fCharOffX;
        pVertexArray[i*24 + 1] = fCharOffY - fLowerDrop;

        pVertexArray[i*24 + 4] = fCharOffX;
        pVertexArray[i*24 + 5] = fCharHeight + fCharOffY - fLowerDrop;

        pVertexArray[i*24 + 8] = fCharWidth + fCharOffX;
        pVertexArray[i*24 + 9] = fCharHeight + fCharOffY - fLowerDrop;

        pVertexArray[i*24 + 12] = fCharOffX;
        pVertexArray[i*24 + 13] = fCharOffY - fLowerDrop;

        pVertexArray[i*24 + 16] = fCharWidth + fCharOffX;
        pVertexArray[i*24 + 17] = fCharHeight + fCharOffY - fLowerDrop;

        pVertexArray[i*24 + 20] = fCharWidth + fCharOffX;
        pVertexArray[i*24 + 21] = fCharOffY - fLowerDrop;

        if (target == '\n')
        {
            fCharOffX = 0.0f;
            fCharOffY -= fCharHeight + m_fLineSpacing*fCharHeight;
        }
        else
        {
            fCharOffX += fCharWidth + m_fCharSpacing*fCharWidth;
        }
    }
}

void Text::SetFont(Font* pFont)
{
    m_pFont = pFont;
}

void Text::SetCharSpacing(float fCharSpacing)
{
    m_fCharSpacing = fCharSpacing;
}
