#ifndef MESH_LOADER_H
#define MESH_LOADER_H

class MeshLoader
{

public :

    enum MeshLoaderEnum
    {
        MESH_LOADER_MAX_FILE_SIZE = 1048576
    };

    static unsigned int LoadOBJ(const char*   filename,
                                unsigned int& faceNum);

private:

    static void GetCounts(const char* pFileName,
                                 int& nNumVerts,
                                 int& nNumUVs,
                                 int& nNumNormals,
	                             int& nNumFaces);

    static void GenerateVertexBuffer(int    nNumFaces,
                                     float* pVertices,
                                     float* pUVs,
                                     float* pNormals,
                                     int*   pFaces,
                                     float* pVB);

    static char s_arFileBuffer[MESH_LOADER_MAX_FILE_SIZE];
};


#endif
