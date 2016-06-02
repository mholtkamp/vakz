#include "AssetBank.h"
#include "Log.h"

AssetBank::AssetBank()
{
    m_rect.m_fX = -1.0f;
    m_rect.m_fY =  0.0f;
    m_rect.m_fWidth = WINDOW_WIDTH;
    m_rect.m_fHeight = 1.0f;

    InitializeView();
    UpdateView();
}

AssetBank::~AssetBank()
{

}

void AssetBank::HandleInput()
{

}

void AssetBank::UpdateView()
{
    Window::UpdateView();
}

void AssetBank::InitializeView()
{
    Window::InitializeView();

    m_textHeader.SetText("Asset Bank");

    // Setup Buttons
    ColorButtonOff(m_btToggleMesh);
    ColorButtonOff(m_btToggleTexture);
    ColorButtonOff(m_btToggleSound);
    ColorButtonOff(m_btAddAsset);
    ColorButtonOff(m_btDeleteAsset);
    ColorButtonOff(m_btCreateMatter);

    for (int i = 0; i < BANK_VISIBLE_ITEMS; i++)
    {
        ColorButtonOff(m_arBankButtons[i]);
    }
}

void AssetBank::RegisterScene(Scene* pScene)
{
    if (pScene == 0)
        return;

    Window::RegisterScene(pScene);
        
    m_btToggleMesh.AddToScene(*pScene);
    m_btToggleTexture.AddToScene(*pScene);
    m_btToggleSound.AddToScene(*pScene);
    m_btAddAsset.AddToScene(*pScene);
    m_btDeleteAsset.AddToScene(*pScene);
    m_btCreateMatter.AddToScene(*pScene);

    for (int i = 0; i < BANK_VISIBLE_ITEMS; i++)
    {
        m_arBankButtons[BANK_VISIBLE_ITEMS].AddToScene(*pScene);
    }
}
