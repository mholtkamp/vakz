#ifndef ASSET_PROPERTIES_H
#define ASSET_PROPERTIES_H

#include "Window.h"

class AssetProperties : public Window
{
public:
    AssetProperties();
    ~AssetProperties();

    void HandleInput();

    void UpdateView();

    void RegisterScene(Scene* pScene);

private:

    // Mesh Properties Views
    Text m_tMaterialType;
    Button m_btMaterialType;
    Text m_tColor;
    TextField m_tfColor[4];
    Text m_tRimColor;
    TextField m_tfRimColor[4];
    Text m_tRimStyle;
    TextField m_btRimStyle;
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

};

#endif