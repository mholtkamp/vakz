#ifndef LEVEL_EDITOR_H
#define LEVEL_EDITOR_H

#include "Scene.h"
#include "Asset.h"
#include "AssetBank.h"
#include "AssetProperties.h"
#include "Outliner.h"

class LevelEditor
{
public:

    LevelEditor();
    ~LevelEditor();
    void Initialize();
    void HandleInput();

    Asset* GetSelectedAsset();
    void SetDetailedAsset(Asset* pAsset);

    Scene m_scene;
    AssetBank m_winAssetBank;
    AssetProperties m_winAssetProperties;
    Outliner m_winOutliner;
};

#endif