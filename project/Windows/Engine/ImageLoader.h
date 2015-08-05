#ifndef IMAGE_LOADER_H
#define IMAGE_LOADER_H

class ImageLoader
{

public:

    static unsigned char* LoadBMP(const char* pFileName,
                                  int&        nWidth,
                                  int&        nHeight,
                                  short&      sBPP);

    enum ImageLoaderEnum
    {
        BMP_MAX_SIZE        = 2048,
        BMP_HEADER_SIZE     = 54,

        BMP_RGB   = 0,
        BMP_RGBA  = 1,
        BMP_ALPHA = 2,
    };


private:

    //## Buffer to hold contents of a file
    //## The extra 64 bytes is extra room for the header.
    static char s_arFileBuffer[BMP_MAX_SIZE*BMP_MAX_SIZE*4 + 64];

};


#endif