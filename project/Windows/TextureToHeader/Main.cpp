#include <stdio.h>
#include <string.h>
#include <Windows.h>


#define IN_FILE_NAME "VakzFont.bmp"
#define OUT_FILE_NAME "ConvertedFont.h"

#define BUFFER_SIZE 2048*2048*4 + 64
#define THRESHOLD 1

static char s_arFileBuffer[BUFFER_SIZE] = {0};
static char s_arOutputBuffer[BUFFER_SIZE] = {0};

void ReadAsset(const char* pFileName,
               char*       arBuffer,
               int         nMaxSize)

{

    FILE* pFile       = 0;
    int   nFileSize   = 0;

    // Open the requested file.
    pFile = fopen(pFileName, "rb");

    // Check if file was found
    if (pFile == 0)
    {
        printf("Asset could not be opened.\n");
        return;
    }

    // Check the file size.
    fseek(pFile, 0, SEEK_END);
    nFileSize = ftell(pFile);
    fseek(pFile, 0, SEEK_SET);

    // Check if file is too big
    if (nFileSize > nMaxSize)
    {
        printf("File is too large.\n");
        fclose(pFile);
        return;
    }

    // Clear the buffer that will hold contents of obj file
    memset(arBuffer, 0, nFileSize + 1);

    // Read from file and store in character buffer.
    fread(arBuffer, nFileSize, 1, pFile);

    // Close file, it is no longer needed.
    fclose(pFile);
}

int main()
{
    int i = 0;
    int j = 0;
    int nSize = 0;
    char* pOut = 0;
    unsigned char* pSrc  = 0;
    
    int nWidth  = 0;
    int nHeight = 0;
    short sBPP  = 0;
    int nLine = 0;

    ReadAsset(IN_FILE_NAME,
              s_arFileBuffer,
              2048 * 
              2048 *
              4 + 
              54);

    // Grab the dimensions of texture
    nWidth  = *reinterpret_cast<int*>(&s_arFileBuffer[18]);
    nHeight = *reinterpret_cast<int*>(&s_arFileBuffer[22]);
    sBPP    = *reinterpret_cast<short*>(&s_arFileBuffer[28]);

    // Fill image data buffer based on format.
    if (sBPP == 24)
    {
        pSrc  = reinterpret_cast<unsigned char*>(&s_arFileBuffer[54]);
        pOut  = s_arOutputBuffer;

        j = 0;
        for (i = 0; i < nWidth*nHeight*3; i += 3)
        {
            if (pSrc[i + 2] > THRESHOLD ||
                pSrc[i + 1] > THRESHOLD ||
                pSrc[i + 0] > THRESHOLD)
            {
                pOut[0] = '0';
                pOut[1] = 'x';
                pOut[2] = 'f';
                pOut[3] = 'f';
                pOut[4] = ',';
            }
            else
            {
                pOut[0] = '0';
                pOut[1] = 'x';
                pOut[2] = '0';
                pOut[3] = '0';
                pOut[4] = ',';
            }

            pOut += 5;
            nSize += 5;

            nLine++;

            if (nLine >= 16)
            {
                pOut[0] = '\n';
                pOut++;
                nSize++;
                nLine = 0;
            }
        }

        // Append a null terminator at the end of the string.
        pOut[0] = 0;
        nSize++;

        FILE* pFile       = 0;

        // Open the requested file.
        pFile = fopen(OUT_FILE_NAME, "w");
        
        if (pFile != 0)
        {
            fwrite(s_arOutputBuffer, nSize, 1, pFile);
            fclose(pFile);
        }
        else
        {
            printf("Couldn't open file for writing output.\n");
        }
        
    }
    else
    {
        printf("Unsupported BPP: %d\n", static_cast<int>(sBPP));
    }


}
