#ifndef LEVEL_EDITOR_H
#define LEVEL_EDITOR_H

#include "Scene.h"
#include "AssetBank.h"
#include "AssetProperties.h"

class LevelEditor
{
public:

    LevelEditor();
    ~LevelEditor();
    void Initialize();
    void HandleInput();

    Scene m_scene;
    AssetBank m_winAssetBank;
    AssetProperties m_winAssetProperties;
};

#endif