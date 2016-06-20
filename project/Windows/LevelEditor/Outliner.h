#ifndef OUTLINER_H
#define OUTLINER_H

#include "Window.h"

#define NUM_ACTOR_BUTTONS 7

class Outliner : public Window
{
public:

    Outliner();
    ~Outliner();

    void HandleInput();

    void InitializeView();

    void UpdateView();

    void RegisterScene(Scene* pScene);

private:

    // View Objects
    Button m_btAddMatter;
    Button m_btAddLight;
    Button m_btAddParticle;

    Button m_arActorButtons[NUM_ACTOR_BUTTONS];

    Text m_tSearch;
    TextField m_tfSearch;

    // Data
    Button* m_pPrevHover;

    Actor* m_pSelectedActor;
    int m_nSelectedActor;

    // List
    List m_lActors;
};

#endif