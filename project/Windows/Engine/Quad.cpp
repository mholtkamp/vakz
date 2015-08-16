#include "Quad.h"
#include "VGL.h"
#include "Settings.h"
#include "Log.h"
#include <stdio.h>

//*****************************************************************************
// Constructor
//*****************************************************************************
Quad::Quad()
{
    m_pTexture  = 0;
    m_fWidth    = 1.0f;
    m_fHeight   = 1.0f;
    m_fTileS    = 1.0f;
    m_fTileT    = 1.0f;

    GeneratePositionArray();
    GenerateTexCoordArray();

    //## Border enable
    m_nBorderEnable = 0;

    //## Border color
    m_arBorderColor[0] = 0.0f;
    m_arBorderColor[1] = 0.0f;
    m_arBorderColor[2] = 0.0f;
    m_arBorderColor[3] = 1.0f;

    //## Border line width
    m_fBorderWidth = 1.0f;
}

//*****************************************************************************
// Destructor
//*****************************************************************************
Quad::~Quad()
{

}

//*****************************************************************************
// Render
//*****************************************************************************
void Quad::Render()
{
    int hProgram  = GetShaderProgram(QUAD_PROGRAM);
    int hPosition = -1;
    int hTexCoord = -1;
    int hType     = -1;
    int hColor    = -1;
    int hTexture  = -1;

    if (m_nVisible != 0)
    {
        // Unbind VBO (using client-side arrays)
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Switch to the Quad program
        glUseProgram(hProgram);

        glActiveTexture(GL_TEXTURE0);

        // Retreive variable locations
        hPosition = glGetAttribLocation(hProgram, "aPosition");
        hTexCoord = glGetAttribLocation(hProgram, "aTexCoord");
        hType     = glGetUniformLocation(hProgram, "uType");
        hColor    = glGetUniformLocation(hProgram, "uColor");
        hTexture  = glGetUniformLocation(hProgram, "uTexture");

        // Set up vertex arrays
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

        // Set uniforms
        glUniform1i(hType, (m_pTexture == 0) ? 0 : 1);
        glUniform4fv(hColor, 1, m_arColor);
        glUniform1i(hTexture, 0);  // Set texture unit to 0.

        // Bind texture
        if (m_pTexture != 0)
        {
            m_pTexture->Bind();
        }

        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        // If the border is enabled, draw a border.
        if (m_nBorderEnable != 0)
        {
            // Disable any texture features as the border
            // cannot be rendered with a texture... or at least
            // it shouldn't be.
            glUniform1i(hType, 0);
            glBindTexture(GL_TEXTURE_2D, 0);
            glUniform4fv(hColor, 1, m_arBorderColor);
            glLineWidth(m_fBorderWidth);

            glDrawArrays(GL_LINE_LOOP, 0, 4);

            // Reset the line width
            glLineWidth(1.0f);
        }
    }
}

//*****************************************************************************
// SetPosition
//*****************************************************************************
void Quad::SetPosition(float fX,
                       float fY)
{
    m_fX = fX;
    m_fY = fY;

    GeneratePositionArray();
}

//*****************************************************************************
// SetWidth
//*****************************************************************************
void Quad::SetWidth(float fWidth)
{
    m_fWidth = fWidth;

    GeneratePositionArray();
}

//*****************************************************************************
// SetHeight
//*****************************************************************************
void Quad::SetHeight(float fHeight)
{
    m_fHeight = fHeight;

    GeneratePositionArray();
}

//*****************************************************************************
// SetDimensions
//*****************************************************************************
void Quad::SetDimensions(float fWidth,
                         float fHeight)
{
    m_fWidth  = fWidth;
    m_fHeight = fHeight;

    GeneratePositionArray();
}

//*****************************************************************************
// SetBox
//*****************************************************************************
void Quad::SetBox(float fX,
                  float fY,
                  float fWidth,
                  float fHeight)
{
    m_fX      = fX;
    m_fY      = fY;
    m_fWidth  = fWidth;
    m_fHeight = fHeight;

    GeneratePositionArray();
}

//*****************************************************************************
// SetTiling
//*****************************************************************************
void Quad::SetTiling(float fTileS,
                     float fTileT)
{
    m_fTileS = fTileS;
    m_fTileT = fTileT;

    GenerateTexCoordArray();
}

//*****************************************************************************
// GeneratePositionArray
//*****************************************************************************
void Quad::GeneratePositionArray()
{
    // Bottom left corner
    m_arPosition[0] = m_fX;
    m_arPosition[1] = m_fY;

    // Top left corner
    m_arPosition[2] = m_fX;
    m_arPosition[3] = m_fY + m_fHeight;

    // Top right corner
    m_arPosition[4] = m_fX + m_fWidth;
    m_arPosition[5] = m_fY + m_fHeight;

    // Bottom right corner
    m_arPosition[6] = m_fX + m_fWidth;
    m_arPosition[7] = m_fY;
}

//*****************************************************************************
// GenerateTexCoordArray
//*****************************************************************************
void Quad::GenerateTexCoordArray()
{
    // Bottom left corner
    m_arTexCoord[0] = 0.0f;
    m_arTexCoord[1] = 0.0f;
    
    // Top left corner
    m_arTexCoord[2] = 0.0f;
    m_arTexCoord[3] = m_fTileT;

    // Top right corner
    m_arTexCoord[4] = m_fTileS;
    m_arTexCoord[5] = m_fTileT;

    // Bottom right corner
    m_arTexCoord[6] = m_fTileS;
    m_arTexCoord[7] = 0.0f;
}

void Quad::SetTexture(Texture* pTexture)
{
    m_pTexture = pTexture;
}

void Quad::SetBorderColor(float fRed,
                          float fGreen,
                          float fBlue,
                          float fAlpha)
{
    m_arBorderColor[0] = fRed;
    m_arBorderColor[1] = fGreen;
    m_arBorderColor[2] = fBlue;
    m_arBorderColor[3] = fAlpha;
}

void Quad::SetBorderWidth (float fWidth)
{
    m_fBorderWidth = fWidth;
}

void Quad::EnableBorder(int nEnable)
{
    m_nBorderEnable = nEnable;
}
