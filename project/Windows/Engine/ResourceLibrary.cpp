#include "ResourceLibrary.h"
#include "Log.h"
#include "Primitives.h"

#include "Texture.h"
#include "StaticMesh.h"
#include "AnimatedMesh.h"
#include "SkeletalMesh.h"
#include "Sound.h"

// Cube Primitive Data
void* ResourceLibrary::s_pCube = 0;


//*****************************************************************************
// Constructor
//*****************************************************************************
ResourceLibrary::ResourceLibrary(int nMaxTextures,
                                 int nMaxStaticMeshes,
                                 int nMaxAnimatedMeshes,
                                 int nMaxSkeletalMeshes,
                                 int nMaxSounds)
{
    m_nMaxTextures       = nMaxTextures;
    m_nMaxStaticMeshes   = nMaxStaticMeshes;
    m_nMaxAnimatedMeshes = nMaxAnimatedMeshes;
    m_nMaxSkeletalMeshes = nMaxSkeletalMeshes;
    m_nMaxSounds         = nMaxSounds;

    m_nNumTextures       = 0;
    m_nNumStaticMeshes   = 0;
    m_nNumAnimatedMeshes = 0;
    m_nNumSkeletalMeshes = 0;
    m_nNumSounds         = 0;

    m_arTextures       = new void*[m_nMaxTextures];
    m_arStaticMeshes   = new void*[m_nMaxStaticMeshes];
    m_arAnimatedMeshes = new void*[m_nMaxAnimatedMeshes];
    m_arSkeletalMeshes = new void*[m_nMaxSkeletalMeshes];
    m_arSounds         = new void*[m_nMaxSounds];

    m_arTextureStrings      = new char*[m_nMaxTextures];
    m_arStaticMeshStrings   = new char*[m_nMaxStaticMeshes];
    m_arAnimatedMeshStrings = new char*[m_nMaxAnimatedMeshes];
    m_arSkeletalMeshStrings = new char*[m_nMaxSkeletalMeshes];
    m_arSoundStrings        = new char*[m_nMaxSounds];

    // Cube primitive
    if (s_pCube != 0)
    {
        s_pCube = new StaticMesh();
        reinterpret_cast<StaticMesh*>(s_pCube)->SetVertexCount(s_nCubeVertexCount);
        reinterpret_cast<StaticMesh*>(s_pCube)->SetPositionArray(s_arCubePosition);
        reinterpret_cast<StaticMesh*>(s_pCube)->SetTexCoordArray(0);
        reinterpret_cast<StaticMesh*>(s_pCube)->SetNormalArray(s_arCubeNormal);
    }
}

//*****************************************************************************
// Destructor
//*****************************************************************************
ResourceLibrary::~ResourceLibrary()
{
    // The ResourceLibrary is responsible for deleting all data it generates.
    int i = 0;

    // Free textures
    if (m_arTextures != 0)
    {
        for (i = 0; i < m_nMaxTextures; i++)
        {
            if (m_arTextures[i] != 0)
            {
                delete reinterpret_cast<Texture*>(m_arTextures[i]);
                m_arTextures[i] = 0;
            }
        }

        delete [] m_arTextures;
        m_arTextures = 0;
    }

    // Free static meshes
    if (m_arStaticMeshes != 0)
    {
        for (i = 0; i < m_nMaxStaticMeshes; i++)
        {
            if (m_arStaticMeshes[i] != 0)
            {
                delete reinterpret_cast<StaticMesh*>(m_arStaticMeshes[i]);
                m_arStaticMeshes[i] = 0;
            }
        }

        delete [] m_arStaticMeshes;
        m_arStaticMeshes[i] = 0;
    }

    // Free animated meshes
    if (m_arAnimatedMeshes != 0)
    {
        for (i = 0; i < m_nMaxAnimatedMeshes; i ++)
        {
            if (m_arAnimatedMeshes[i] != 0)
            {
                delete reinterpret_cast<AnimatedMesh*>(m_arAnimatedMeshes[i]);
                m_arAnimatedMeshes[i] = 0;
            }
        }

        delete [] m_arAnimatedMeshes;
        m_arAnimatedMeshes = 0;
    }

    // Free skeletal meshes
    if (m_arSkeletalMeshes != 0)
    {
        for (i = 0; i < m_nMaxSkeletalMeshes; i++)
        {
            if (m_arSkeletalMeshes[i] != 0)
            {
                delete reinterpret_cast<SkeletalMesh*>(m_arSkeletalMeshes[i]);
                m_arSkeletalMeshes[i] = 0;
            }
        }

        delete [] m_arSkeletalMeshes;
        m_arSkeletalMeshes = 0;
    }

    // Free sounds
    if (m_arSounds != 0)
    {
        for (i = 0; i < m_nMaxSounds; i++)
        {
            if (m_arSounds[i] != 0)
            {
                delete reinterpret_cast<Sound*>(m_arSounds[i]);
                m_arSounds[i] = 0;
            }
        }

        delete [] m_arSounds;
        m_arSounds = 0;
    }

    // Free texture strings
    if (m_arTextureStrings != 0)
    {
        for (i = 0; i < m_nMaxTextures; i++)
        {
            if (m_arTextureStrings[i] != 0)
            {
                delete [] m_arTextureStrings[i];
                m_arTextureStrings[i] = 0;
            }
        }

        delete [] m_arTextureStrings;
        m_arTextureStrings = 0;
    }

    // Free static mesh strings
    if (m_arStaticMeshStrings != 0)
    {
        for (i = 0; i < m_nMaxStaticMeshes; i++)
        {
            if (m_arStaticMeshStrings[i] != 0)
            {
                delete reinterpret_cast<StaticMesh*>(m_arStaticMeshStrings[i]);
                m_arStaticMeshStrings[i] = 0;
            }
        }

        delete [] m_arStaticMeshStrings;
        m_arStaticMeshStrings[i] = 0;
    }

    // Free animated mesh strings
    if (m_arAnimatedMeshStrings != 0)
    {
        for (i = 0; i < m_nMaxAnimatedMeshes; i ++)
        {
            if (m_arAnimatedMeshStrings[i] != 0)
            {
                delete reinterpret_cast<AnimatedMesh*>(m_arAnimatedMeshStrings[i]);
                m_arAnimatedMeshStrings[i] = 0;
            }
        }

        delete [] m_arAnimatedMeshStrings;
        m_arAnimatedMeshStrings = 0;
    }

    // Free skeletal mesh strings
    if (m_arSkeletalMeshStrings != 0)
    {
        for (i = 0; i < m_nMaxSkeletalMeshes; i++)
        {
            if (m_arSkeletalMeshStrings[i] != 0)
            {
                delete reinterpret_cast<SkeletalMesh*>(m_arSkeletalMeshStrings[i]);
                m_arSkeletalMeshStrings[i] = 0;
            }
        }

        delete [] m_arSkeletalMeshStrings;
        m_arSkeletalMeshStrings = 0;
    }

    // Free sound strings
    if (m_arSoundStrings != 0)
    {
        for (i = 0; i < m_nMaxSounds; i++)
        {
            if (m_arSoundStrings[i] != 0)
            {
                delete reinterpret_cast<Sound*>(m_arSoundStrings[i]);
                m_arSoundStrings[i] = 0;
            }
        }

        delete [] m_arSoundStrings;
        m_arSoundStrings = 0;
    }
}

//*****************************************************************************
// LoadResource
//*****************************************************************************
int ResourceLibrary::LoadResource(int nResourceType,
                                  const char* pFile)
{
    LogWarning("LoadResource not implemented yet.");
    return 0;
}

//*****************************************************************************
// GetResource
//*****************************************************************************
void* ResourceLibrary::GetResource(int nResourceType,
                                   const char* pFile)
{
    LogWarning("GetResource not implemented yet.");
    return 0;
}

//*****************************************************************************
// GetPrimitive
//*****************************************************************************
void* ResourceLibrary::GetPrimitive(int nPrimitiveType)
{
    if (nPrimitiveType == PRIMITIVE_CUBE)
    {
        return s_pCube;
    }
    else
    {
        LogWarning("Requested primitive is not implemented yet.");
        return 0;
    }
}