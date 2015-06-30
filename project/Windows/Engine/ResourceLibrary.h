#ifndef RESOURCE_LIBRARY_H
#define RESOURCE_LIBRARY_H

enum Resources
{
    TEXTURE_RESOURCE       = 0,
    STATIC_MESH_RESOURCE   = 1,
    ANIMATED_MESH_RESOURCE = 2,
    SKELETAL_MESH_RESOURCE = 3,
    SOUND_RESOURCE         = 4
};

enum PrimitiveResource
{
    PRIMITIVE_CUBE    = 0,
    PRIMITIVE_SPHERE  = 1,
    PRIMITIVE_PYRAMID = 2,
    PRIMITIVE_QUAD    = 3,
    PRIMITIVE_TORUS   = 4
};
class ResourceLibrary
{

public:

    //## **********************************************************************
    //## Constructor
    //## **********************************************************************
    ResourceLibrary(int nMaxTextures       = RL_DEFAULT_MAX_TEXTURES,
                    int nMaxStaticMeshes   = RL_DEFAULT_MAX_STATIC_MESHES,
                    int nMaxAnimatedMeshes = RL_DEFAULT_MAX_ANIMATED_MESHES,
                    int nMaxSkeletalMeshes = RL_DEFAULT_MAX_SKELETAL_MESHES,
                    int nMaxSounds         = RL_DEFAULT_MAX_SOUNDS);

    //## **********************************************************************
    //## Destructor
    //## **********************************************************************
    ~ResourceLibrary();

    //## **********************************************************************
    //## LoadResource
    //##
    //## Creates and loads a resource from a given filename.
    //##
    //## Input:
    //##   nResourceType - type of resource (matches Resources enum values).
    //##   pFile         - string containing filename.
    //##
    //## Returns:
    //##   int - '1' if resource was loaded successfully.
    //##       - '0' otherwise.
    //## **********************************************************************
    int LoadResource(int nResourceType,
                     const char* pFile);

    //## **********************************************************************
    //## GetResource
    //##
    //## Returns a pointer to a resource that has already been loaded from a
    //## previous call to LoadResource().
    //##
    //## Input:
    //##   nResourceType - type of resource (matches Resources enum values).
    //##   pFile         - string containing filename.
    //##
    //## Returns:
    //##   void* - pointer to resource. '0x0' if resource could not be found.
    //## **********************************************************************
    void* GetResource(int nResourceType,
                      const char* pFile);

    //## **********************************************************************
    //## GetPrimitive
    //##
    //## Returns a pointer to a StaticMesh primitive. Primitives are static
    //## meshes with basic geomtry that are commonly used throughout many 
    //## 3D games/applications. Primitives include meshes such as: Cube,
    //## Sphere, Pyramid, Torus, etc.
    //## 
    //## Primitives are loaded by default during construction of the Resource
    //## library and do not count towards the maximum number of static meshes.
    //##
    //## Input:
    //##   nPrimitiveType - type of primitive to return. See PrimitiveResource
    //##                    enum at tope of file.
    //##
    //## Returns:
    //##   void* - pointer to primitive. 0x0 if invalid primitive type is 
    //##           supplied by calling function.
    //## **********************************************************************
    void* GetPrimitive(int nPrimitiveType);

    enum ResourceLibraryEnum
    {
        RL_DEFAULT_MAX_TEXTURES        = 1024,
        RL_DEFAULT_MAX_STATIC_MESHES   = 1024,
        RL_DEFAULT_MAX_ANIMATED_MESHES = 1024,
        RL_DEFAULT_MAX_SKELETAL_MESHES = 1024,
        RL_DEFAULT_MAX_SOUNDS          = 1024
    };

private:

    //## Maximums
    int m_nMaxTextures;
    int m_nMaxStaticMeshes;
    int m_nMaxAnimatedMeshes;
    int m_nMaxSkeletalMeshes;
    int m_nMaxSounds;

    //## Current counts
    int m_nNumTextures;
    int m_nNumStaticMeshes;
    int m_nNumAnimatedMeshes;
    int m_nNumSkeletalMeshes;
    int m_nNumSounds;

    //## Arrays holding pointers to resources
    void** m_arTextures;
    void** m_arStaticMeshes;
    void** m_arAnimatedMeshes;
    void** m_arSkeletalMeshes;
    void** m_arSounds;

    //## Arrays to hold filenames
    char** m_arTextureStrings;
    char** m_arStaticMeshStrings;
    char** m_arAnimatedMeshStrings;
    char** m_arSkeletalMeshStrings;
    char** m_arSoundStrings;

    //## Static Mesh primitives
    static void* s_pCube;
};
#endif
