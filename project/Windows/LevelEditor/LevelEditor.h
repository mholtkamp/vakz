#ifndef LEVEL_EDITOR_H
#define LEVEL_EDITOR_H

#include "Scene.h"
#include "AssetBank.h"

class LevelEditor
{
public:

    LevelEditor();
    ~LevelEditor();
    void Initialize();
    void HandleInput();

    Scene m_scene;
    AssetBank m_winAssetBank;
};

#endif