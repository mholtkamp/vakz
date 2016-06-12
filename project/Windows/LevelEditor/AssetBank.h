#ifndef ASSET_BANK_H
#define ASSET_BANK_H

#include "Window.h"
#include "Button.h"
#include "TextField.h"
#include "Asset.h"

#define BANK_VISIBLE_ITEMS 7

#define DISP_MESHES ASSET_MESH
#define DISP_TEXTURES ASSET_TEXTURE
#define DISP_SOUNDS ASSET_SOUND

class AssetBank : public Window
{
public:
    AssetBank();
    ~AssetBank();

    void HandleInput();

    void UpdateView();

    void InitializeView();

    void RegisterScene(Scene* pScene);

private:

    void ImportAsset();

    void LoadMesh(char* pStr);
    void LoadTexture(char* pStr);
    void LoadSound(char* pStr);

    // View objects
    Button m_btToggleMesh;
    Button m_btToggleTexture;
    Button m_btToggleSound;

    Button m_arBankButtons[BANK_VISIBLE_ITEMS];

    Button m_btAddAsset;
    Button m_btDeleteAsset;
    Button m_btCreateMatter;

    // Data
    int m_nShowMeshes;
    int m_nShowTextures;
    int m_nShowSounds;
    Button* m_pPrevHover;
    List m_lMeshes;
    List m_lTextures;
    List m_lSounds;

    int m_nDispAsset;
    int m_nDispOffset;
    Asset* m_pSelectedAsset;
    int m_nSelectedAsset;
};

#endif