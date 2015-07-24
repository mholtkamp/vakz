#include "Text.h"
#include "ResourceLibrary.h"
#include "VGL.h"
#include <string.h>

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
}

void Text::Render()
{
    //int hProg = GetShaderProgram(ProgramEnum::TEXT_PROGRAM);
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

}