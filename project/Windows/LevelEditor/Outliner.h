#ifndef OUTLINER_H
#define OUTLINER_H

#include "Window.h"

class Outliner : public Window
{
public:
    Outliner();
    ~Outliner();

    void HandleInput();

    void UpdateView();

    void RegisterScene(Scene* pScene);

private:

};

#endif