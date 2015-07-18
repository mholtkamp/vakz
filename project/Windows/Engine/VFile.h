#ifndef VFILE_H
#define VFILE_H

void ReadAsset(const char* pFileName,
               char*       arBuffer,
               int         nMaxSize);

int GetAssetSize(const char* pFileName);

void SetAssetManager(void* pAssetManager);

#endif