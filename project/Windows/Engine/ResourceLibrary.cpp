#include "ResourceLibrary.h"
#include "Log.h"
#include "Primitives.h"
#include "DefaultFont.h"

#include "Texture.h"
#include "StaticMesh.h"
#include "AnimatedMesh.h"
#include "SkeletalMesh.h"
#include "Sound.h"
#include "Font.h"

#include <string.h>

// Cube Primitive Data
void* ResourceLibrary::s_pCube = 0;

// Default Font Texture
void* ResourceLibrary::s_pDefaultFont = 0;


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

    memset(m_arTextures,       0, m_nMaxTextures       * sizeof(void*));
    memset(m_arStaticMeshes,   0, m_nMaxStaticMeshes   * sizeof(void*));
    memset(m_arAnimatedMeshes, 0, m_nMaxAnimatedMeshes * sizeof(void*));
    memset(m_arSkeletalMeshes, 0, m_nMaxSkeletalMeshes * sizeof(void*));
    memset(m_arSounds,         0, m_nMaxSounds         * sizeof(void*));

    m_arTextureStrings      = new char*[m_nMaxTextures];
    m_arStaticMeshStrings   = new char*[m_nMaxStaticMeshes];
    m_arAnimatedMeshStrings = new char*[m_nMaxAnimatedMeshes];
    m_arSkeletalMeshStrings = new char*[m_nMaxSkeletalMeshes];
    m_arSoundStrings        = new char*[m_nMaxSounds];

    memset(m_arTextureStrings,      0, m_nMaxTextures       * sizeof(char*));
    memset(m_arStaticMeshStrings,   0, m_nMaxStaticMeshes   * sizeof(char*));
    memset(m_arAnimatedMeshStrings, 0, m_nMaxAnimatedMeshes * sizeof(char*));
    memset(m_arSkeletalMeshStrings, 0, m_nMaxSkeletalMeshes * sizeof(char*));
    memset(m_arSoundStrings,        0, m_nMaxSounds         * sizeof(char*));
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
        m_arStaticMeshes = 0;
    }

    // Free animated meshes
    if (m_arAnimatedMeshes != 0)
    {
        for (i = 0; i < m_nMaxAnimatedMeshes; i++)
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
        m_arStaticMeshStrings = 0;
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

    // Delete Primitives
    if (s_pCube != 0)
    {
        delete reinterpret_cast<StaticMesh*>(s_pCube);
        s_pCube = 0;
    }
}

//*****************************************************************************
// LoadResource
//*****************************************************************************
int ResourceLibrary::LoadResource(int nResourceType,
                                  const char* pFile)
{
    Texture**      arTextures       = reinterpret_cast<Texture**>(m_arTextures);
    StaticMesh**   arStaticMeshes   = reinterpret_cast<StaticMesh**>(m_arStaticMeshes);
    AnimatedMesh** arAnimatedMeshes = reinterpret_cast<AnimatedMesh**>(m_arAnimatedMeshes);
    SkeletalMesh** arSkeletalMeshes = reinterpret_cast<SkeletalMesh**>(m_arSkeletalMeshes);
    Sound**        arSound          = reinterpret_cast<Sound**>(m_arSounds);

    int nReturn = 0;
    int nStrLen = 0;

    switch (nResourceType)
    {
    case RESOURCE_TEXTURE:
        if (m_nNumTextures < m_nMaxTextures)
        {
            // Load the resource
            arTextures[m_nNumTextures] = new Texture();
            arTextures[m_nNumTextures]->LoadBMP(pFile);

            // Save the string
            nStrLen = strlen(pFile);
            m_arTextureStrings[m_nNumTextures] = new char[nStrLen + 1];
            memcpy(m_arTextureStrings[m_nNumTextures], pFile, nStrLen);
            m_arTextureStrings[m_nNumTextures][nStrLen] = 0;

            // Increment counter and set return status
            m_nNumTextures++;
            nReturn = 1;
        }
        break;

    case RESOURCE_STATIC_MESH:
        if (m_nNumStaticMeshes < m_nMaxStaticMeshes)
        {
            // Load the resource
            arStaticMeshes[m_nNumStaticMeshes] = new StaticMesh();
            arStaticMeshes[m_nNumStaticMeshes]->Load(pFile);

            // Save the string
            nStrLen = strlen(pFile);
            m_arStaticMeshStrings[m_nNumStaticMeshes] = new char[nStrLen + 1];
            memcpy(m_arStaticMeshStrings[m_nNumStaticMeshes], pFile, nStrLen);
            m_arStaticMeshStrings[m_nNumStaticMeshes][nStrLen] = 0;

            // Increment counter and set return status
            m_nNumStaticMeshes++;
            nReturn = 1;
        }
        break;

    case RESOURCE_ANIMATED_MESH:
        if (m_nNumAnimatedMeshes < m_nMaxAnimatedMeshes)
        {
            // Load the resource
            arAnimatedMeshes[m_nNumAnimatedMeshes] = new AnimatedMesh();
            arAnimatedMeshes[m_nNumAnimatedMeshes]->Load(pFile);

            // Save the string
            nStrLen = strlen(pFile);
            m_arAnimatedMeshStrings[m_nNumAnimatedMeshes] = new char[nStrLen + 1];
            memcpy(m_arAnimatedMeshStrings[m_nNumAnimatedMeshes], pFile, nStrLen);
            m_arAnimatedMeshStrings[m_nNumAnimatedMeshes][nStrLen] = 0;

            // Increment counter and set return status
            m_nNumAnimatedMeshes++;
            nReturn = 1;
        }
        break;

    case RESOURCE_SKELETAL_MESH:
        LogWarning("Skeletal Mesh loading not supported in ResourceLibrary.");
        break;

    case RESOURCE_SOUND:
        LogWarning("Sound loading is not supported in ResourceLibrary yet.");
        break;

    default:
        break;
    }

    return nReturn;
}

//*****************************************************************************
// GetResource
//*****************************************************************************
void* ResourceLibrary::GetResource(int nResourceType,
                                   const char* pFile)
{
    int i = 0;
    char** arStrings = 0;
    int nCount = 0;
    void** arRes = 0;

    switch (nResourceType)
    {
    case RESOURCE_TEXTURE:
        arStrings = m_arTextureStrings;
        nCount = m_nNumTextures;
        arRes = m_arTextures;
        break;
    case RESOURCE_STATIC_MESH:
        arStrings = m_arStaticMeshStrings;
        nCount = m_nNumStaticMeshes;
        arRes = m_arStaticMeshes;
        break;
    case RESOURCE_ANIMATED_MESH:
        arStrings = m_arAnimatedMeshStrings;
        nCount = m_nNumAnimatedMeshes;
        arRes = m_arAnimatedMeshes;
        break;
    case RESOURCE_SKELETAL_MESH:
        arStrings = m_arSkeletalMeshStrings;
        nCount = m_nNumSkeletalMeshes;
        arRes = m_arSkeletalMeshes;
        break;
    case RESOURCE_SOUND:
        arStrings = m_arSoundStrings;
        nCount = m_nNumSounds;
        arRes = m_arSounds;
        break;
    default:
        break;
    }

    if (arStrings != 0)
    {
        for (i = 0; i < nCount; i++)
        {
            if (strcmp(pFile, arStrings[i]) == 0)
            {
                return arRes[i];
            }
        }
        
        // No matching string was found.
        LogWarning("ResourceLibrary could not find requested resource.");
        return 0;
    }
    else
    {
        return 0;
    }
}

//*****************************************************************************
// GetPrimitive
//*****************************************************************************
void* ResourceLibrary::GetPrimitive(int nPrimitiveType)
{
    if (nPrimitiveType == PRIMITIVE_CUBE)
    {
        // Cube primitive
        if (s_pCube == 0)
        {
            s_pCube = new StaticMesh();
            reinterpret_cast<StaticMesh*>(s_pCube)->SetVertexCount(s_nCubeVertexCount);
            reinterpret_cast<StaticMesh*>(s_pCube)->SetPositionArray(s_arCubePosition);
            reinterpret_cast<StaticMesh*>(s_pCube)->SetTexCoordArray(0);
            reinterpret_cast<StaticMesh*>(s_pCube)->SetNormalArray(s_arCubeNormal);
        }
        return s_pCube;
    }
    else
    {
        LogWarning("Requested primitive is not implemented yet.");
        return 0;
    }
}

//*****************************************************************************
// GetDefaultFontTexture
//*****************************************************************************
void* ResourceLibrary::GetDefaultFont()
{
    // Check if the texture exists yet. If not, create it.
    if (s_pDefaultFont == 0)
    {
        s_pDefaultFont = new Font();
        reinterpret_cast<Font*>(s_pDefaultFont)->
            LoadArray(pDefaultFontArray, 
                      Font::LEGACY_TYPE);
    }

    return s_pDefaultFont;
}