#ifndef IMAGE_LOADER_H
#define IMAGE_LOADER_H

class ImageLoader
{

public:

    //## **********************************************************************
    //## LoadBMP
    //##
    //## Loads a bitmap and returns a pointer to the array of pixel data.
    //## The unsigned char array that is returned is allocated on the heap,
    //## so it is the responsibility of the calling function to delete this 
    //## memory after it is finished using it, or else this function will
    //## leak memory.
    //##
    //## Input:
    //##   pFileName - name of file on disk to load.
    //##
    //## Output:
    //##   nWidth  - width of bitmap
    //##   nHeight - height of bitmap
    //##   sBPP    - bits per pixel
    //##
    //## Returns:
    //##   unsigned char* - pointer to array of pixel data.
    //## **********************************************************************
    static unsigned char* LoadBMP(const char* pFileName,
                                  int         nColorKey,
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