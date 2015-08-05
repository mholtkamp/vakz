#include "Texture.h"
#include "VGL.h"
#include "VFile.h"
#include "ImageLoader.h"
#include "Log.h"
#include <stdio.h>
#if defined (ANDROID)
#include <android/asset_manager.h>
#endif

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

    
    pData = ImageLoader::LoadBMP(pFileName,
                                 m_nWidth,
                                 m_nHeight,
                                 sBPP);

    // Fill image data buffer based on format.
    if (sBPP == 1)
    {
        m_nFormat = TEXTURE_ALPHA;
        LogError("Alpha not supported yet.");
        return;
    }
    else if (sBPP == 24)
    {
        m_nFormat = TEXTURE_RGB;

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
        m_nFormat = TEXTURE_RGBA;
        LogError("32 bit BMPs not supported yet.");
        return;
    }
    else
    {
        LogWarning("Failed to load null file name in Texture::LoadBMP.");
    }
}

void Texture::LoadArray(const unsigned char* pData,
                        int                  nFormat,
                        int                  nWidth,
                        int                  nHeight)
{
    m_nFormat = nFormat;
    m_nWidth  = nWidth;
    m_nHeight = nHeight;

    // Generate and bind as current texture
    glGenTextures(1, &m_hTexture);
    glBindTexture(GL_TEXTURE_2D, m_hTexture);

    // Set default texture paramters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Allocate graphics memory and upload texture
    if (m_nFormat == TEXTURE_ALPHA)
    {
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_ALPHA,
                     m_nWidth,
                     m_nHeight,
                     0,
                     GL_ALPHA,
                     GL_UNSIGNED_BYTE,
                     pData);
    }
    else if (m_nFormat == TEXTURE_RGB)
    {
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGB,
                     m_nWidth,
                     m_nHeight,
                     0,
                     GL_RGB,
                     GL_UNSIGNED_BYTE,
                     pData);
    }
    else if (m_nFormat == TEXTURE_RGBA)
    {
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGBA,
                     m_nWidth,
                     m_nHeight,
                     0,
                     GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     pData);
    }
}

//*****************************************************************************
// Bind
//*****************************************************************************
void Texture::Bind()
{
    glBindTexture(GL_TEXTURE_2D, m_hTexture);
}
