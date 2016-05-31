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

};

#endif