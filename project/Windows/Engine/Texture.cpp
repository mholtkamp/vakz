#include "Texture.h"
#include "VGL.h"
#include "Log.h"
#include <stdio.h>

unsigned char Texture::s_arFileBuffer[TEXTURE_MAX_SIZE*TEXTURE_MAX_SIZE*4 + 64];

//*****************************************************************************
// Constructor
//*****************************************************************************
Texture::Texture()
{
    m_hTexture = 0;
    m_nWidth   = 0;
    m_nHeight  = 0;
    m_nFormat  = 0;
}

//*****************************************************************************
// Destructor
//*****************************************************************************
Texture::~Texture()
{
    
}

//*****************************************************************************
// LoadBMP
//*****************************************************************************
void Texture::LoadBMP(const char* pFileName)
{
    int            i         = 0;
    int            nFileSize = 0;
    FILE*          pFile     = 0;
    unsigned char* pData     = 0;
    unsigned char* pSrc      = 0;
    short          sBPP      = 0;

    if (pFileName != 0)
    {
        // Open the file for reading.
        pFile = fopen(pFileName, "r");

        // Check if file was opened
        if (pFile == 0)
        {
            LogError("Failed to open file in Texture::LoadBMP");
            return;
        }

        // Check the size of the file
        fseek(pFile, 0, SEEK_END);
        nFileSize = ftell(pFile);
        fseek(pFile, 0, SEEK_SET);

        // If file is too big, report error
        if (nFileSize > TEXTURE_MAX_SIZE * 
                        TEXTURE_MAX_SIZE *
                        4 + 
                        TEXTURE_BMP_HEADER_SIZE)
        {
            LogError("Texture Bitmap is larger than supported 2048x2048xRGBA.\n");
            fclose(pFile);
            return;
        }

        // Read the file to RAM
        fread(s_arFileBuffer, nFileSize, 1, pFile);

        // Close the file as it is no longer needed.
        fclose(pFile);

        // Grab the dimensions of texture
        m_nWidth  = *reinterpret_cast<int*>(&s_arFileBuffer[18]);
        m_nHeight = *reinterpret_cast<int*>(&s_arFileBuffer[22]);
        sBPP      = *reinterpret_cast<short*>(&s_arFileBuffer[28]);

        // Fill image data buffer based on format.
        if (sBPP == 1)
        {
            m_nFormat = TEXTURE_BMP_ALPHA;
            LogError("1 bit BMPs not supported yet.");
            return;
        }
        else if (sBPP == 24)
        {
            m_nFormat = TEXTURE_BMP_RGB;
            pData = new unsigned char[m_nWidth * m_nHeight * 3];
            pSrc  = &s_arFileBuffer[54];

            for (i = 0; i < m_nWidth*m_nHeight*3; i += 3)
            {
                // Swap the B and R byte because windows 
                pData[i]     = pSrc[i + 2];
                pData[i + 1] = pSrc[i + 1];
                pData[i + 2] = pSrc[i];                  
            }

            // Generate and bind as current texture
            glGenTextures(1, &m_hTexture);
            glBindTexture(GL_TEXTURE_2D, m_hTexture);

            // Set default texture paramters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // Allocate graphics memory and upload texture
            glTexImage2D(GL_TEXTURE_2D,
                         0,
                         GL_RGB,
                         m_nWidth,
                         m_nHeight,
                         0,
                         GL_RGB,
                         GL_UNSIGNED_BYTE,
                         pData);

            // Delete client-side image data. It is stored on GPU now.
            delete [] pData;
            pData = 0;
        }
        else if (sBPP == 32)
        {
            m_nFormat = TEXTURE_BMP_RGBA;
            LogError("32 bit BMPs not supported yet.");
            return;
        }
        
    }
    else
    {
        LogWarning("Failed to load null file name in Texture::LoadBMP.");
    }
}

//*****************************************************************************
// Bind
//*****************************************************************************
void Texture::Bind()
{
    glBindTexture(GL_TEXTURE_2D, m_hTexture);
}
