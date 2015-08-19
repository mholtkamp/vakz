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
    //## LoadArray
    //## 
    //## Generates a texture and uploads texture data from the given array to
    //## graphics memory.
    //##
    //## Input:
    //##   pData   - array of pixel data for the texutre.
    //##   nFormat - format of the provided data (RGB/RGBA/ALPHA). See
    //##             "TextureEnum" below for applicable values.
    //##   nWidth  - width of texture.
    //##   nHeight - height of texture.
    //## **********************************************************************
    void LoadArray(const unsigned char* pData,
                   int                  nFormat,
                   int                  nWidth,
                   int                  nHeight);

    //## **********************************************************************
    //## Bind
    //##
    //## Calls glBindTexture() using this texture's handle.
    //## **********************************************************************
    void Bind();

    //## **********************************************************************
    //## SetFiltering
    //##
    //## Sets the texture filtering for this texture to either linear or 
    //## nearest. The texture is bound by this function.
    //##
    //## Input:
    //##    nFilterType - NEAREST or LINEAR (refer to TextureEnum)
    //## **********************************************************************
    void SetFiltering(int nFilterType);

    enum TextureEnum
    {
        TEXTURE_MAX_SIZE        = 2048,
        TEXTURE_BMP_HEADER_SIZE = 54,

        TEXTURE_RGB   = 0,
        TEXTURE_RGBA  = 1,
        TEXTURE_ALPHA = 2,

        NEAREST = 0,
        LINEAR  = 1
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
};

#endif
