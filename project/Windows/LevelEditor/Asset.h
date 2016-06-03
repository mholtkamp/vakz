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

struct Asset
{
    char m_arName[ASSET_NAME_BUFFER_SIZE];

    Asset()
    {
        memset(m_arName, 0, ASSET_NAME_BUFFER_SIZE);
    }

    
};

struct MeshAsset : public Asset
{
    StaticMesh* m_pMesh;
    Material* m_pMaterial;
    List m_lColliders;

    MeshAsset()
    {
        m_pMesh     = 0;
        m_pMaterial = 0;
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