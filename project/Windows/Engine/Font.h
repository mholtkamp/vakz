#ifndef FONT_H
#define FONT_H

#include "Texture.h"

class Font
{

public:

    //## **********************************************************************
    //## Constructor
    //## **********************************************************************
    Font();

    //## **********************************************************************
    //## Destructor
    //## **********************************************************************
    ~Font();

    //## **********************************************************************
    //## LoadBMP
    //##
    //## Loads a font from a bitmap file.
    //## 
    //## Input:
    //##   pFileName - name of bitmap file on disk to load.
    //## **********************************************************************
    void LoadBMP(const char* pFileName);

    //## **********************************************************************
    //## LoadArray
    //## 
    //## Loads an array (already in memory) to video RAM.
    //##
    //## Input:
    //##   pData - pointer to array to load.
    //##   nType - type of array to load. (LEGACY_TYPE)
    //## **********************************************************************
    void LoadArray(const unsigned char*  pData,
                   int                   nType);

    //## **********************************************************************
    //## SetRenderState
    //## 
    //## Sets up the OpenGL to properly use the font. This should be called
    //## before doing any rendering with a font.
    //## 
    //## Input:
    //##   hProg - handle to shader program being used.
    //## **********************************************************************
    void SetRenderState(unsigned int hProg);

    //## **********************************************************************
    //## SetFiltering
    //## 
    //## Sets the filtering of the texture. Linear filtering provides smoother
    //## text, Nearest filtering creates a pixelated look.
    //## **********************************************************************
    void SetFiltering(int nFilterType);

    enum FontEnum
    {
        LEGACY_TYPE   = 0,
        LEGACY_WIDTH  = 256,
        LEGACY_HEIGHT = 512
    };

private:

    //## Type of font. Currently only LEGACY_TYPE is availble.
    int m_nType;

    //## Font texture to use when drawing glyphs on the screen.
    Texture m_texture;
};

#endif
