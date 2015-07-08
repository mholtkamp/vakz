#ifndef TEXTURE_H
#define TEXTURE_H

class Texture
{

public:
    
    Texture();

    ~Texture();

    void LoadBMP(const char* pFileName);

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

    unsigned int m_hTexture;
    
    int m_nWidth;

    int m_nHeight;

    int m_nFormat;

    static unsigned char s_arFileBuffer[TEXTURE_MAX_SIZE*TEXTURE_MAX_SIZE*4 + 64];
};

#endif
