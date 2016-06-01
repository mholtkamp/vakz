#ifndef ASSET_BANK_H
#define ASSET_BANK_H

#include "Window.h"

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

    

};

#endif