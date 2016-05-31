#ifndef ACTOR_PROPERTIES_H
#define ACTOR_PROPERTIES_H

#include "Window.h"

class ActorProperties : public Window
{
public:
    ActorProperties();
    ~ActorProperties();

    void HandleInput();

    void UpdateView();

    void RegisterScene(Scene* pScene);

private:

};

#endif