#include "Texture.h"
#include "VGL.h"
#include "VFile.h"
#include "Log.h"
#include <stdio.h>
#if defined (ANDROID)
#include <android/asset_manager.h>
#endif

char Texture::s_arFileBuffer[TEXTURE_MAX_SIZE*TEXTURE_MAX_SIZE*4 + 64];

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
        ReadAsset(pFileName,
                  s_arFileBuffer,
                  TEXTURE_MAX_SIZE * 
                  TEXTURE_MAX_SIZE *
                  4 + 
                  TEXTURE_BMP_HEADER_SIZE);

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
            pSrc  = reinterpret_cast<unsigned char*>(&s_arFileBuffer[54]);

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
