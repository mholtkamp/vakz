#ifndef ASSET_BANK_H
#define ASSET_BANK_H

#include "Window.h"
#include "Button.h"
#include "TextField.h"

#define BANK_VISIBLE_ITEMS 7

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
};

#endif