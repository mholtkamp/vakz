#ifndef MESH_LOADER_H
#define MESH_LOADER_H

class MeshLoader
{

public :

    enum MeshLoaderEnum
    {
        MESH_LOADER_MAX_FILE_SIZE = 1048576,
        MESH_LOADER__ANIMATION_NAME_SIZE_WARNING = 32
    };

    //## **********************************************************************
    //## LoadOBJ
    //##
    //## Loads an wavefront .obj file from disk and constructs an interleaved
    //## vertex array. The vertex array array gets uploaded to the GPU into
    //## a vertex buffer object.
    //##
    //## Input:
    //##   pFileName - path to obj file to load.
    //##
    //## Output:
    //##   nFaces - number of faces in mesh.
    //## 
    //## Returns:
    //##   unsigned int - handle to the newly created vertex buffer object.
    //## **********************************************************************
    static unsigned int LoadOBJ(const char*   pFileName,
                                unsigned int& nFaces);

    static void LoadAMF(const char*     pFileName,
                        char***         pAnimationNames,
                        int*            pAnimationCount,
                        int*            pFramesPerSecond,
                        int**           pKeyFrameCounts,
                        int***          pKeyFrameStarts,
                        unsigned int*** pFaces,
                        unsigned int*** pVBO);

private:

    //## Scans file for number of vertices/UVs/normals/faces
    //## Needed to figure out how much heap memory to allocate.
    static void GetCounts(const char* pFileName,
                                 int& nNumVerts,
                                 int& nNumUVs,
                                 int& nNumNormals,
	                             int& nNumFaces);

    //## Generates an interleaved vertex buffer in system memory.
    static void GenerateVertexBuffer(int    nNumFaces,
                                     float* pVertices,
                                     float* pUVs,
                                     float* pNormals,
                                     int*   pFaces,
                                     float* pVB);

    //## Buffer to hold file contents.
    static char s_arFileBuffer[MESH_LOADER_MAX_FILE_SIZE];
};


#endif
