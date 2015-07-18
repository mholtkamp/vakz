#ifndef VFILE_H
#define VFILE_H

//## **********************************************************************
//## ReadAsset
//##
//## Opens a file and stores it in the given buffer.
//##
//## Input:
//##   pFileName - name of file to open.
//##   nMaxSize  - maximum number of bytes to read.
//##
//## Output:
//##   arBuffer - buffer to store data read from file.
//## **********************************************************************
void ReadAsset(const char* pFileName,
               char*       arBuffer,
               int         nMaxSize);

//## **********************************************************************
//## GetAssetSize
//##
//## Opens a file and returns the file size in bytes.
//##
//## Input:
//##   pFileName - path to file
//## **********************************************************************
int GetAssetSize(const char* pFileName);

//## **********************************************************************
//## SetAssetManager
//##
//## Sets the asset manager used for accessing files. Android requires
//## an asset manager to read assets packaged in the installed APK.
//##
//## Input:
//##   pAssetManager - pointer to asset manager.
//## **********************************************************************
void SetAssetManager(void* pAssetManager);

#endif