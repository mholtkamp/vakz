#include "AssetProperties.h"

AssetProperties::AssetProperties()
{

}

AssetProperties::~AssetProperties()
{

}

void AssetProperties::HandleInput()
{

}

void AssetProperties::InitializeView()
{
    //// Mesh Properties Views
    //m_tMaterialType;
    //m_btMaterialType;
    //m_tColor;
    //m_tfColor[4];
    //m_tRimColor;
    //m_tfRimColor[4];
    //m_tRimStyle;
    //m_btRimStyle;
    //m_tRimSize;
    //m_tfRimSize;
    //m_tIntervals;
    //m_tfIntervals;
    //
    //m_tDefaultTexture;
    //m_btDefaultTexture;
    //m_btAssignDefaultTexture;

    //m_tCollider;
    //m_btCollider;
    //m_btAddCollider;
    //m_btRemoveCollider;

    //m_tColliderCenter;
    //m_tfColliderCenter[3];
    //m_tColliderExtents;
    //m_tfColliderExtents[3];
    //m_tColliderRotation;
    //m_tfColliderRotation[3];
    //m_tColliderColor;
    //m_tfColliderColor[4];

    //// Texture Properties Views
    //m_tTextureFilter;
    //m_btTextureFilter;
}

void AssetProperties::UpdateView()
{

}

void AssetProperties::RegisterScene(Scene* pScene)
{
    if (pScene == 0)
    {
        return;
    }

    // Mesh Properties Views
    pScene->AddGlyph(&m_tMaterialType);
    m_btMaterialType.AddToScene(*pScene);
    pScene->AddGlyph(&m_tColor);
    m_tfColor[0].AddToScene(*pScene);
    m_tfColor[1].AddToScene(*pScene);
    m_tfColor[2].AddToScene(*pScene);
    m_tfColor[3].AddToScene(*pScene);
    pScene->AddGlyph(&m_tRimColor);
    m_tfRimColor[0].AddToScene(*pScene);
    m_tfRimColor[1].AddToScene(*pScene);
    m_tfRimColor[2].AddToScene(*pScene);
    m_tfRimColor[3].AddToScene(*pScene);
    pScene->AddGlyph(&m_tRimStyle);
    m_btRimStyle.AddToScene(*pScene);
    pScene->AddGlyph(&m_tRimSize);
    m_tfRimSize.AddToScene(*pScene);
    pScene->AddGlyph(&m_tIntervals);
    m_tfIntervals.AddToScene(*pScene);
    
    pScene->AddGlyph(&m_tDefaultTexture);
    m_btDefaultTexture.AddToScene(*pScene);
    m_btAssignDefaultTexture.AddToScene(*pScene);

    pScene->AddGlyph(&m_tCollider);
    m_btCollider.AddToScene(*pScene);
    m_btAddCollider.AddToScene(*pScene);
    m_btRemoveCollider.AddToScene(*pScene);

    pScene->AddGlyph(&m_tColliderCenter);
    m_tfColliderCenter[0].AddToScene(*pScene);
    m_tfColliderCenter[1].AddToScene(*pScene);
    m_tfColliderCenter[2].AddToScene(*pScene);
    pScene->AddGlyph(&m_tColliderExtents);
    m_tfColliderExtents[0].AddToScene(*pScene);
    m_tfColliderExtents[1].AddToScene(*pScene);
    m_tfColliderExtents[2].AddToScene(*pScene);
    pScene->AddGlyph(&m_tColliderRotation);
    m_tfColliderRotation[0].AddToScene(*pScene);
    m_tfColliderRotation[1].AddToScene(*pScene);
    m_tfColliderRotation[2].AddToScene(*pScene);
    pScene->AddGlyph(&m_tColliderColor);
    m_tfColliderColor[0].AddToScene(*pScene);
    m_tfColliderColor[1].AddToScene(*pScene);
    m_tfColliderColor[2].AddToScene(*pScene);
    m_tfColliderColor[3].AddToScene(*pScene);

    // Texture Properties Views
    pScene->AddGlyph(&m_tTextureFilter);
    m_btTextureFilter.AddToScene(*pScene);
}