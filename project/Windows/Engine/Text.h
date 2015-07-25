#ifndef TEXT_H
#define TEXT_H

#include "Glyph.h"
#include "Font.h"

class Text : public Glyph
{

public:

    Text();

    ~Text();

    void Render();

    void SetScale(float fX,
                  float fY);

    void SetText(const char* pText);

    void SetBufferSize(int nSize);

    enum TextEnum
    {
        DEFAULT_BUFFER_SIZE = 128,

        TRIANGLES_PER_CHARACTER = 2,
        VERTICES_PER_TRIANGLE   = 3,
        FLOATS_PER_TRIANGLE     = 12
    };

private:

    void GenerateVertexArray(float** pArray,
                             int     nTextLength);

    int m_nBufferSize;

    char* m_pText;

    int m_nTextLength;

    float m_fScaleX;

    float m_fScaleY;

    Font* m_pFont;

    unsigned int m_hVBO;
};

#endif
