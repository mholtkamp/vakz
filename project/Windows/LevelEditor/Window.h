#ifndef WINDOW_H
#define WINDOW_H

#include "Scene.h"

class Window
{
public:

    Window();
    ~Window();

    int IsVisible();

    void SetVisible(int nVisible);
    
private:

    int m_nVisible;
    Scene* m_pScene;
};

#endif