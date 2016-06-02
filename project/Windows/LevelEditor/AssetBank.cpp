#include "AssetBank.h"

AssetBank::AssetBank()
{

}

AssetBank::~AssetBank()
{

}

void AssetBank::HandleInput()
{

}

void AssetBank::UpdateView()
{

}

void AssetBank::InitializeView()
{
    Window::InitializeView();

    m_textHeader.SetText("Asset Bank");

    // Toggle Mesh Button
    m_btToggleMesh.SetBackgroundColor(s_arColor3[0],
                                      s_arColor3[1],
                                      s_arColor3[2],
                                      s_arColor3[3]);
    m_btToggleMesh.SetTextColor(s_arColor5[0],
                                s_arColor5[1],
                                s_arColor5[2],
                                s_arColor5[3]);
    m_btToggleMesh.EnableBorder(0);

    //

}

void AssetBank::RegisterScene(Scene* pScene)
{
    Window::RegisterScene(pScene);
}
