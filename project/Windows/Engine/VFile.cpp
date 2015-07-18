#include "VFile.h"
#include "Log.h"
#include <string.h>
#if defined(WINDOWS)
#include <stdio.h>
#elif defined(ANDROID)
#include <android/asset_manager.h>
static AAssetManager* s_pAssetManager = 0;
#endif

//*****************************************************************************
// ReadAsset
//*****************************************************************************
void ReadAsset(const char* pFileName,
               char*       arBuffer,
               int         nMaxSize)
{
#if defined (WINDOWS)
    FILE* pFile       = 0;
    int   nFileSize   = 0;

    // Open the requested file.
    pFile = fopen(pFileName, "rb");

    // Check if file was found
    if (pFile == 0)
    {
        LogError("Asset could not be opened.");
        return;
    }

    // Check the file size.
    fseek(pFile, 0, SEEK_END);
    nFileSize = ftell(pFile);
    fseek(pFile, 0, SEEK_SET);

    // Check if file is too big
    if (nFileSize > nMaxSize)
    {
        LogError("File is too large.");
        fclose(pFile);
        return;
    }

    // Clear the buffer that will hold contents of obj file
    memset(arBuffer, 0, nFileSize + 1);

    // Read from file and store in character buffer.
    fread(arBuffer, nFileSize, 1, pFile);

    // Close file, it is no longer needed.
    fclose(pFile);

#elif defined (ANDROID)
    int   nFileSize   = 0;
    AAsset* pAsset    = 0;

    pAsset = AAssetManager_open((AAssetManager*) s_pAssetManager, pFileName, AASSET_MODE_BUFFER);

    if (!pAsset)
    {
        LogError("Could not oppen asset in MeshLoader::GetCounts().");
        return;
    }

    nFileSize = AAsset_getLength(pAsset);
    if(nFileSize > nMaxSize)
    {
        LogError("File is too big.");
        AAsset_close(pAsset);
        return;
    }

    // Clear the buffer that will hold contents of obj file
    memset(arBuffer, 0, nFileSize + 1);

    AAsset_read(pAsset, arBuffer, nFileSize);
    AAsset_close(pAsset);
#endif
}

//*****************************************************************************
// GetAssetSize
//*****************************************************************************
int GetAssetSize(const char* pFileName)
{
#if defined (WINDOWS)
    FILE* pFile       = 0;
    int   nFileSize   = 0;

    // Open the requested file.
    pFile = fopen(pFileName, "rb");

    // Check if file was found
    if (pFile == 0)
    {
        LogError("Asset could not be opened.");
        return 0;
    }

    // Check the file size.
    fseek(pFile, 0, SEEK_END);
    nFileSize = ftell(pFile);
    fseek(pFile, 0, SEEK_SET);

    // Close file, it is no longer needed.
    fclose(pFile);

    return nFileSize;

#elif defined (ANDROID)
    int   nFileSize   = 0;
    AAsset* pAsset    = 0;

    pAsset = AAssetManager_open((AAssetManager*) s_pAssetManager, pFileName, AASSET_MODE_BUFFER);

    if (!pAsset)
    {
        LogError("Could not oppen asset.");
        return 0;
    }

    // Check the size of the file.
    nFileSize = AAsset_getLength(pAsset);

    // Close file now that file size is known.
    AAsset_close(pAsset);

    return nFileSize;
#endif
}

//*****************************************************************************
// SetAssetManager
//*****************************************************************************
void SetAssetManager(void* pAssetManager)
{
#if defined (ANDROID)
    s_pAssetManager = reinterpret_cast<AAssetManager*>(pAssetManager);
#endif
}
