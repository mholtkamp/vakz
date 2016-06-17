#ifndef ASSET_PROPERTIES_H
#define ASSET_PROPERTIES_H

#include "Window.h"
#include "Asset.h"

class AssetProperties : public Window
{
public:
    AssetProperties();
    ~AssetProperties();

    void HandleInput();

    void InitializeView();

    void UpdateView();

    void HideViews();

    void UpdateView_Mesh();
    void UpdateView_Texture();
    void UpdateView_Sound();

    void HandleInput_Mesh();
    void HandleInput_Texture();
    void HandleInput_Sound();

    void RegisterScene(Scene* pScene);

    void SetDetailedAsset(Asset* pAsset);

private:

    // General Property Views
    Text m_tName1;
    Text m_tName2;

    // Mesh Properties Views
    Text m_tMaterialType;
    Button m_btMaterialType;
    Text m_tColor;
    TextField m_tfColor[4];
    Text m_tRimColor;
    TextField m_tfRimColor[4];
    Text m_tRimStyle;
    Button m_btRimStyle;
    Text m_tRimSize;
    TextField m_tfRimSize;
    Text m_tIntervals;
    TextField m_tfIntervals;
    
    Text m_tDefaultTexture;
    Button m_btDefaultTexture;
    Button m_btAssignDefaultTexture;

    Text m_tCollider;
    Button m_btCollider;
    Button m_btAddCollider;
    Button m_btRemoveCollider;

    Text m_tColliderCenter;
    TextField m_tfColliderCenter[3];
    Text m_tColliderExtents;
    TextField m_tfColliderExtents[3];
    Text m_tColliderRotation;
    TextField m_tfColliderRotation[3];
    Text m_tColliderColor;
    TextField m_tfColliderColor[4];

    // Texture Properties Views
    Text m_tTextureFilter;
    Button m_btTextureFilter;

    // Data
    Asset* m_pAsset;
    Button* m_pPrevHover;
    int m_nCurCollider;
};

#endif