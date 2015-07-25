#ifndef FONT_H
#define FONT_H

#include "Texture.h"

class Font
{

public:

    Font();

    ~Font();

    void LoadBMP(const char* pFileName);

    void LoadArray(const unsigned char*  pData,
                   int                   nType);

    void SetRenderState(unsigned int hProg);

    enum FontEnum
    {
        LEGACY_TYPE   = 0,
        LEGACY_WIDTH  = 128,
        LEGACY_HEIGHT = 128
    };

private:

    int m_nType;

    Texture m_pTexture;
};

#endif
