#include "AssetProperties.h"

#define TF_SCALE_X 0.4f
#define TF_SCALE_Y 0.6f

#define T_SCALE_X 0.5f
#define T_SCALE_Y 0.65f

#define BT_SCALE_X 0.5f
#define BT_SCALE_Y 0.65f

#define TEXT_COLOR s_arColor2

AssetProperties::AssetProperties()
{
    m_rect.m_fX = -1.0f;
    m_rect.m_fY = -1.0f;
    m_rect.m_fWidth = WINDOW_WIDTH;
    m_rect.m_fHeight = 1.13f;
}

AssetProperties::~AssetProperties()
{

}

void AssetProperties::HandleInput()
{

}

void AssetProperties::InitializeView()
{
    Window::InitializeView();

    m_textHeader.SetText("Asset Properties");

    // MESH

    // Material Section
    m_tMaterialType.SetText("Material:");
    m_tMaterialType.SetColor(TEXT_COLOR);
    m_tMaterialType.SetScale(T_SCALE_X, T_SCALE_Y);

    ColorButtonOff(m_btMaterialType);
    m_btMaterialType.SetTextScale(BT_SCALE_X, BT_SCALE_Y);

    m_tColor.SetText("Color");
    m_tColor.SetColor(TEXT_COLOR);
    m_tColor.SetScale(T_SCALE_X, T_SCALE_Y);

    m_tRimColor.SetText("Rim Color");
    m_tRimColor.SetColor(TEXT_COLOR);
    m_tRimColor.SetScale(T_SCALE_X, T_SCALE_Y);

    m_tRimStyle.SetText("Rim Style");
    m_tRimStyle.SetColor(TEXT_COLOR);
    m_tRimStyle.SetScale(T_SCALE_X, T_SCALE_Y);

    ColorButtonOff(m_btRimStyle);
    m_btRimStyle.SetTextScale(BT_SCALE_X, BT_SCALE_Y);

    m_tRimSize.SetText("Rim Size");
    m_tRimSize.SetColor(TEXT_COLOR);
    m_tRimSize.SetScale(T_SCALE_X, T_SCALE_Y);

    SetTextFieldColors(m_tfRimSize);
    m_tfRimSize.SetTextScale(TF_SCALE_X, TF_SCALE_Y);

    m_tIntervals.SetText("Intervals");
    m_tIntervals.SetColor(TEXT_COLOR);
    m_tIntervals.SetScale(T_SCALE_X, T_SCALE_Y);

    SetTextFieldColors(m_tfIntervals);
    m_tfIntervals.SetTextScale(TF_SCALE_X, TF_SCALE_Y);

    // Default Texture Section
    m_tDefaultTexture.SetText("Texture:");
    m_tDefaultTexture.SetColor(TEXT_COLOR);
    m_tDefaultTexture.SetScale(T_SCALE_X, T_SCALE_Y);

    ColorButtonOff(m_btDefaultTexture);
    m_btDefaultTexture.SetTextScale(BT_SCALE_X, BT_SCALE_Y);

    ColorButtonOff(m_btAssignDefaultTexture);
    m_btAssignDefaultTexture.SetTextScale(BT_SCALE_X, BT_SCALE_Y);

    // Collider Section
    m_tCollider.SetText("Collider:");
    m_tCollider.SetColor(TEXT_COLOR);
    m_tCollider.SetScale(T_SCALE_X, T_SCALE_Y);

    ColorButtonOff(m_btCollider);
    m_btCollider.SetTextScale(BT_SCALE_X, BT_SCALE_Y);

    ColorButtonOff(m_btAddCollider);
    m_btAddCollider.SetTextScale(BT_SCALE_X, BT_SCALE_Y);
    m_btAddCollider.SetTextString("+");

    ColorButtonOff(m_btRemoveCollider);
    m_btRemoveCollider.SetTextScale(BT_SCALE_X, BT_SCALE_Y);
    m_btRemoveCollider.SetTextString("-");

    m_tColliderCenter.SetText("Center");
    m_tColliderCenter.SetColor(TEXT_COLOR);
    m_tColliderCenter.SetScale(T_SCALE_X, T_SCALE_Y);

    m_tColliderExtents.SetText("Extents");
    m_tColliderExtents.SetColor(TEXT_COLOR);
    m_tColliderExtents.SetScale(T_SCALE_X, T_SCALE_Y);
    
    m_tColliderRotation.SetText("Rotation");
    m_tColliderRotation.SetColor(TEXT_COLOR);
    m_tColliderRotation.SetScale(T_SCALE_X, T_SCALE_Y);

    m_tColliderColor.SetText("Color");
    m_tColliderColor.SetColor(TEXT_COLOR);
    m_tColliderColor.SetScale(T_SCALE_X, T_SCALE_Y);

    // TEXTURE

    m_tTextureFilter.SetText("Filter:");
    m_tTextureFilter.SetColor(TEXT_COLOR);
    m_tTextureFilter.SetScale(T_SCALE_X, T_SCALE_Y);
    
    ColorButtonOff(m_btTextureFilter);
    m_btTextureFilter.SetTextScale(BT_SCALE_X, BT_SCALE_Y);

    // Set textfield-array data
    for (int i = 0; i < 3; i++)
    {
        SetTextFieldColors(m_tfColliderCenter[i]);
        m_tfColliderCenter[i].SetTextScale(TF_SCALE_X, TF_SCALE_Y);

        SetTextFieldColors(m_tfColliderExtents[i]);
        m_tfColliderExtents[i].SetTextScale(TF_SCALE_X, TF_SCALE_Y);

        SetTextFieldColors(m_tfColliderRotation[i]);
        m_tfColliderRotation[i].SetTextScale(TF_SCALE_X, TF_SCALE_Y);
    }

    for (int i = 0; i < 4; i++)
    {
        SetTextFieldColors(m_tfColor[i]);
        m_tfColor[i].SetTextScale(TF_SCALE_X, TF_SCALE_Y);

        SetTextFieldColors(m_tfRimColor[i]);
        m_tfRimColor[i].SetTextScale(TF_SCALE_X, TF_SCALE_Y);

        SetTextFieldColors(m_tfColliderColor[i]);
        m_tfColliderColor[i].SetTextScale(TF_SCALE_X, TF_SCALE_Y);
    }
}

void AssetProperties::UpdateView()
{
    Window::UpdateView();
}

void AssetProperties::RegisterScene(Scene* pScene)
{
    if (pScene == 0)
    {
        return;
    }

    Window::RegisterScene(pScene);

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