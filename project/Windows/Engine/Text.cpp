#include "Text.h"
#include "ResourceLibrary.h"
#include "VGL.h"
#include <string.h>

#define DEFAULT_CHAR_WIDTH  0.05f
#define DEFAULT_CHAR_HEIGHT 0.05f
#define CHAR_UNIT           0.125f

Text::Text()
{
    m_nBufferSize  = DEFAULT_BUFFER_SIZE;
    m_pText        = new char[m_nBufferSize];
    m_nTextLength  = 0;
    m_fScaleX      = 1.0f;
    m_fScaleY      = 1.0f;

    m_pFont = reinterpret_cast<Font*>(ResourceLibrary::GetDefaultFont());

    memset(m_pText, 0, m_nBufferSize);
}

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

void Text::Render()
{
    unsigned int hProg = GetShaderProgram(ProgramEnum::TEXT_PROGRAM);
    int hColor    = -1;
    int hPosition = -1;
    int hTexCoord = -1;

    glUseProgram(hProg);
    hColor    = glGetUniformLocation(hProg, "uColor");
    hPosition = glGetAttribLocation(hProg, "aPosition");
    hTexCoord = glGetAttribLocation(hProg, "aTexCoord");

    m_pFont->SetRenderState(hProg);
    glUniform4fv(hColor, 1, m_arColor);

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

void Text::SetScale(float fX,
                    float fY)
{
    m_fScaleX = fX;
    m_fScaleY = fY;
}

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

void Text::GenerateVertexArray(float** pArray,
                               int     nTextLength)
{
    int i               = 0;
    float* pVertexArray = *pArray;
    float fCharWidth    = m_fScaleX  * DEFAULT_CHAR_WIDTH;
    float fCharHeight   = m_fScaleY  * DEFAULT_CHAR_HEIGHT;

    for (i = 0; i < nTextLength; i++)
    {
        unsigned int index; 
        float xIndex;
        float yIndex;
        char target = (char) toupper(m_pText[i]);

        if(target == '\0')         // Breakout on null terminating char
            break;
        else if(target >= ' ' &&   // Valid character range
                target <= 'Z')
        {
            index = target - 0x20;
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
        pVertexArray[i*24 + 2] = CHAR_UNIT * xIndex;
        pVertexArray[i*24 + 3] = 1.0f - CHAR_UNIT * (yIndex+1.0f);

        pVertexArray[i*24 + 6] = CHAR_UNIT * xIndex;
        pVertexArray[i*24 + 7] = 1.0f - CHAR_UNIT * (yIndex); 

        pVertexArray[i*24 + 10] = CHAR_UNIT * (xIndex+1.0f);
        pVertexArray[i*24 + 11] = 1.0f - CHAR_UNIT * (yIndex); 

        pVertexArray[i*24 + 14] = CHAR_UNIT * xIndex;
        pVertexArray[i*24 + 15] = 1.0f - CHAR_UNIT * (yIndex+1.0f);

        pVertexArray[i*24 + 18] = CHAR_UNIT * (xIndex+1.0f);
        pVertexArray[i*24 + 19] = 1.0f - CHAR_UNIT * (yIndex); 

        pVertexArray[i*24 + 22] = CHAR_UNIT * (xIndex+1.0f);
        pVertexArray[i*24 + 23] = 1.0f - CHAR_UNIT * (yIndex+1.0f);


        // Position
        pVertexArray[i*24 + 0] = m_fX + i*fCharWidth;
        pVertexArray[i*24 + 1] = m_fY;

        pVertexArray[i*24 + 4] = m_fX + i*fCharWidth;
        pVertexArray[i*24 + 5] = m_fY + fCharHeight;

        pVertexArray[i*24 + 8] = m_fX + fCharWidth + i*fCharWidth;
        pVertexArray[i*24 + 9] = m_fY + fCharHeight;

        pVertexArray[i*24 + 12] = m_fX + i*fCharWidth;
        pVertexArray[i*24 + 13] = m_fY;

        pVertexArray[i*24 + 16] = m_fX + fCharWidth + i*fCharWidth;
        pVertexArray[i*24 + 17] = m_fY + fCharHeight;

        pVertexArray[i*24 + 20] = m_fX + fCharWidth + i*fCharWidth;
        pVertexArray[i*24 + 21] = m_fY;
    }
}