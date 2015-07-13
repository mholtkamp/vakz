#ifndef TEXTURE_H
#define TEXTURE_H

class Texture
{

public:
    
    //## **********************************************************************
    //## Constructor
    //## **********************************************************************
    Texture();

    //## **********************************************************************
    //## Destructor
    //## **********************************************************************
    ~Texture();

    //## **********************************************************************
    //## LoadBMP
    //## 
    //## Reads a .bmp image file, generates the pixel array, creates an OpenGL
    //## texture, and uploads the pixel array to the GPU. This function should
    //## only be called when there is a valid graphics context. Only works with
    //## 1, 24, and 32 bits-per-pixel bitmaps.
    //##
    //## Input:
    //##   pFileName - path to .bmp file to load.
    //## **********************************************************************
    void LoadBMP(const char* pFileName);

    //## **********************************************************************
    //## Bind
    //##
    //## Calls glBindTexture() using this texture's handle.
    //## **********************************************************************
    void Bind();

    enum TextureEnum
    {
        TEXTURE_MAX_SIZE        = 2048,
        TEXTURE_BMP_HEADER_SIZE = 54,

        TEXTURE_BMP_RGB   = 0,
        TEXTURE_BMP_RGBA  = 1,
        TEXTURE_BMP_ALPHA = 2,
    };

private:

    //## Handle to OpenGL texture
    unsigned int m_hTexture;
    
    //## Width of the texture.
    int m_nWidth;

    //## Height of the texture.
    int m_nHeight;

    //## Format of texture (ie. ALPHA, RGB, RGBA).
    int m_nFormat;

    //## buffer to hold contents of .bmp file.
    static unsigned char s_arFileBuffer[TEXTURE_MAX_SIZE*TEXTURE_MAX_SIZE*4 + 64];
};

#endif
