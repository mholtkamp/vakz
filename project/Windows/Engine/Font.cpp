#include "Font.h"
#include "Log.h"
#include "VGL.h"

Font::Font()
{
    m_nType    = LEGACY_TYPE;
}

Font::~Font()
{

}

void Font::LoadBMP(const char* pFileName)
{
    m_texture.LoadBMP(pFileName);
}

void Font::LoadArray(const unsigned char* pData,
                     int                  nType)
{
    // Legacy textures are 128x128 alpha textures
    // that only support numbers, capital letters, and
    // a few symbols.
    if (nType == LEGACY_TYPE)
    {
        m_texture.LoadArray(pData,
                             Texture::TEXTURE_ALPHA,
                             LEGACY_WIDTH,
                             LEGACY_HEIGHT);
    }
    else
    {
        LogWarning("Unrecognized font type in Font::LoadArray().");
    }
}

void Font::SetRenderState(unsigned int hProg)
{
    int hTexture = glGetUniformLocation(hProg, "uTexture");
    glUniform1i(hTexture, 0);
    m_texture.Bind();
}


void Font::SetFiltering(int nFilterType)
{
    m_texture.SetFiltering(nFilterType);
}
