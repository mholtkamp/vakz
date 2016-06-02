#include "LevelEditor.h"

LevelEditor::LevelEditor()
{

}

LevelEditor::~LevelEditor()
{

}

void LevelEditor::Initialize()
{
    m_winAssetBank.RegisterScene(&m_scene);
    m_winAssetBank.Initialize();
}

void LevelEditor::HandleInput()
{
    m_winAssetBank.HandleInput();
}
