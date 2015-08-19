#ifndef MESH_LOADER_H
#define MESH_LOADER_H

class MeshLoader
{

public :

    enum MeshLoaderEnum
    {
        MESH_LOADER_MAX_FILE_SIZE = 1048576,
        MESH_LOADER_ANIMATION_NAME_SIZE_WARNING = 32
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
    //##   nFaces     - number of faces in mesh.
    //## 
    //## Returns:
    //##   unsigned int - handle to the newly created vertex buffer object.
    //## **********************************************************************
    static unsigned int LoadOBJ(const char*   pFileName,
                                unsigned int& nFaces);

    //## **********************************************************************
    //## LoadOBJGeometry
    //##
    //## Loads an wavefront .obj file from disk and constructs a position
    //## vertex array. The vertex array array is stored in system RAM, and not
    //## uploaded to VRAM. The purpose of this is to load meshes for collision
    //## purposes.
    //##
    //## Input:
    //##   pFileName - path to obj file to load.
    //##
    //## Output:
    //##   nFaces     - number of faces in mesh.
    //## 
    //## Returns:
    //##   float* - pointer to the vertex buffer in main memory.
    //## **********************************************************************
    static float* LoadOBJGeometry(const char* pFileName,
                                  unsigned int& nFaces);

    //## **********************************************************************
    //## LoadAMF
    //##
    //## Loads a .amf animated mesh file.
    //##
    //## Input:
    //##   pFileName - path to amf file to load.
    //##
    //## Output:
    //##   pAnimationNames - pointer to a 2D char array. Allocated in function.
    //##   pAnimationCount - pointer to int to store the number of animations.
    //##   pFramesPerSecond - pointer to int to store the framerate.
    //##   pKeyFrameCounts - pointer to array of keyframes per animation. 
    //##                     Allocated within function.
    //##   pKeyFrameStates - pointer to 2D array of start frames for each 
    //##                     keyframe for each animation. Allocated in function.
    //##   pFaces          - pointer to an array of face counts per keyframe per
    //##                     animation. Allocated in this function.
    //##   pVBO            - pointer to VBO handle array for each keyframe per
    //##                     animation. Allocated in this function.
    //## **********************************************************************
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

    //## Generates a vertex buffer that stores position only.
    static void GenerateVertexGeometryBuffer(int    nNumFaces,
                                             float* pVertices,
                                             int*   pFaces,
                                             float* pVertexBuffer);

    //## Buffer to hold file contents.
    static char s_arFileBuffer[MESH_LOADER_MAX_FILE_SIZE];
};


#endif
