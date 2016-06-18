#ifndef ASSET_H
#define ASSET_H

#include "StaticMesh.h"
#include "Material.h"
#include "DiffuseMaterial.h"
#include "Texture.h"
#include "Collider.h"
#include "List.h"
#include <string.h>

#define ASSET_NAME_BUFFER_SIZE 16

enum AssetType
{
    ASSET_NONE = 0,
    ASSET_MESH = 1,
    ASSET_TEXTURE = 2,
    ASSET_SOUND = 3
};

struct Asset
{
    char m_arName[ASSET_NAME_BUFFER_SIZE];
    char* m_pPath;
    int m_nType;

    Asset()
    {
        memset(m_arName, 0, ASSET_NAME_BUFFER_SIZE);
        m_pPath = 0;
        m_nType = ASSET_NONE;
    }

    virtual ~Asset()
    {
        if (m_pPath != 0)
        {
            delete [] m_pPath;
            m_pPath = 0;
        }
    }

    void SetNameFromFile(char* pStr)
    {
        char* pPeriod = 0;
        char* pFile = strrchr(pStr, '\\');

        // Clear the name in case there is a previous name
        memset(m_arName, 0, ASSET_NAME_BUFFER_SIZE);

        // Find the file name, not path
        if (pFile == 0)
        {
            pFile = strrchr(pStr, '/');
        }

        if (pFile != 0)
        {
            strncpy(m_arName, pFile + 1, ASSET_NAME_BUFFER_SIZE);
        }
        else
        {
            strncpy(m_arName, pStr, ASSET_NAME_BUFFER_SIZE);
        }
        
        m_arName[ASSET_NAME_BUFFER_SIZE - 1] = 0;

        // Find the first period
        pPeriod = strchr(m_arName, '.');

        if (pPeriod != 0)
        {
            *pPeriod = 0;
        }
    }

    void SetFilePath(char* pPath)
    {
        // First check if there is already a file path
        if (m_pPath != 0)
        {
            // delete this memory
            delete [] m_pPath;
            m_pPath = 0;
        }
        
        int nLen = strlen(pPath);

        m_pPath = new char[nLen + 1];
        strcpy(m_pPath, pPath);
        m_pPath[nLen] = 0;
    }

    int GetType()
    {
        return m_nType;
    }
};

struct MeshAsset : public Asset
{
    StaticMesh* m_pMesh;
    Material* m_pMaterial;
    List m_lColliders;
    Asset* m_pDefaultTextureAsset;

    MeshAsset()
    {
        m_pMesh     = 0;
        m_pMaterial = 0;
        m_nType = ASSET_MESH;
        m_pDefaultTextureAsset = 0;
    }

    ~MeshAsset()
    {
        if(m_pMesh != 0)
        {
            delete m_pMesh;
            m_pMesh = 0;
        }

        if (m_pMaterial != 0)
        {
            delete m_pMaterial;
            m_pMaterial = 0;
        }

        ListNode* pNode = m_lColliders.GetHead();
        while (pNode != 0)
        {
            if (pNode->m_pData != 0)
            {
                delete reinterpret_cast<Collider*>(pNode->m_pData);
                pNode->m_pData = 0;
            }

            pNode = pNode->m_pNext;
        }
    }
};

struct TextureAsset : public Asset
{
    Texture* m_pTexture;

    TextureAsset()
    {
        m_pTexture = 0;
        m_nType = ASSET_TEXTURE;
    }

    ~TextureAsset()
    {
        if (m_pTexture != 0)
        {
            delete m_pTexture;
            m_pTexture = 0;
        }
    }
};

#endif