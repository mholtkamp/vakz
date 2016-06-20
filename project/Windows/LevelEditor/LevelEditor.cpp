#include "LevelEditor.h"

LevelEditor::LevelEditor()
{

}

LevelEditor::~LevelEditor()
{

}

void LevelEditor::Initialize()
{
    m_winAssetBank.SetEditor(this);
    m_winAssetBank.RegisterScene(&m_scene);
    m_winAssetBank.Initialize();

    m_winAssetProperties.SetEditor(this);
    m_winAssetProperties.RegisterScene(&m_scene);
    m_winAssetProperties.Initialize();

    m_winOutliner.SetEditor(this);
    m_winOutliner.RegisterScene(&m_scene);
    m_winOutliner.Initialize();
}

void LevelEditor::HandleInput()
{
    m_winAssetBank.HandleInput();
    m_winAssetProperties.HandleInput();
    m_winOutliner.HandleInput();
}

Asset* LevelEditor::GetSelectedAsset()
{
    return m_winAssetBank.GetSelectedAsset();
}

void LevelEditor::SetDetailedAsset(Asset* pAsset)
{
    m_winAssetProperties.SetDetailedAsset(pAsset);
}
