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
};


#endif
