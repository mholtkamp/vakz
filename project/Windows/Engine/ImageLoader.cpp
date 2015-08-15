#include "ImageLoader.h"
#include "VFile.h"
#include "Log.h"

char ImageLoader::s_arFileBuffer[BMP_MAX_SIZE*BMP_MAX_SIZE*4 + 64];

unsigned char* ImageLoader::LoadBMP(const char* pFileName,
                                    int&        nWidth,
                                    int&        nHeight,
                                    short&      sBPP)
{
    int i = 0;
    unsigned char* pData = 0;
    unsigned char* pSrc  = 0;

    if (pFileName != 0)
    {
        ReadAsset(pFileName,
                  s_arFileBuffer,
                  BMP_MAX_SIZE * 
                  BMP_MAX_SIZE *
                  4 + 
                  BMP_HEADER_SIZE);

        // Grab the dimensions of texture
        nWidth  = *reinterpret_cast<int*>(&s_arFileBuffer[18]);
        nHeight = *reinterpret_cast<int*>(&s_arFileBuffer[22]);
        sBPP    = *reinterpret_cast<short*>(&s_arFileBuffer[28]);

        // Fill image data buffer based on format.
        if (sBPP == 1)
        {
            LogError("1 bit BMPs not supported yet.");
            return 0;
        }
        else if (sBPP == 8)
        {
            pData = new unsigned char[nWidth * nHeight];
            pSrc  = reinterpret_cast<unsigned char*>(&s_arFileBuffer[54]);

            for (i = 0; i < nWidth*nHeight; i++)
            {
                pData[i] = pSrc[i];
            }
            return pData;
        }
        else if (sBPP == 24)
        {
            pData = new unsigned char[nWidth * nHeight * 3];
            pSrc  = reinterpret_cast<unsigned char*>(&s_arFileBuffer[54]);

            for (i = 0; i < nWidth*nHeight*3; i += 3)
            {
                // Swap the B and R byte because windows bitmap format is in BGR
                pData[i]     = pSrc[i + 2];
                pData[i + 1] = pSrc[i + 1];
                pData[i + 2] = pSrc[i];                  
            }

            // Delete client-side image data. It is stored on GPU now.
            return pData;
        }
        else if (sBPP == 32)
        {
            LogError("32 bit BMPs not supported yet.");
            return 0;
        }
        
    }
    else
    {
        LogWarning("Failed to load null file name in Texture::LoadBMP.");
        return 0;
    }
}
