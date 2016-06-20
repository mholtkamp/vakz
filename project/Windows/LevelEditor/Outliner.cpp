#include "Outliner.h"

#define ADD_BUTTON_WIDTH 0.1f
#define ADD_BUTTON_HEIGHT 0.06f

#define BUTTON_SCALE_X 0.4f
#define BUTTON_SCALE_Y 0.68f

#define TEXT_COLOR s_arColor2

Outliner::Outliner()
{
    m_rect.m_fX = 1.0f - WINDOW_WIDTH;
    m_rect.m_fY = 0.13f;
    m_rect.m_fWidth = WINDOW_WIDTH;
    m_rect.m_fHeight = 0.87f;

    m_pPrevHover = 0;
    
    m_pSelectedActor = 0;
    m_nSelectedActor = 0;
}

Outliner::~Outliner()
{
    ListNode* pNode = m_lActors.GetHead();

    while (pNode != 0)
    {
        delete reinterpret_cast<Actor*>(pNode->m_pData);
        pNode->m_pData = 0;

        pNode = pNode->m_pNext;
    }
}

void Outliner::RegisterScene(Scene* pScene)
{
    m_btAddLight.AddToScene(*pScene);
    m_btAddMatter.AddToScene(*pScene);
    m_btAddParticle.AddToScene(*pScene);

    for (int i = 0; i < NUM_ACTOR_BUTTONS; i++)
    {
        m_arActorButtons[i].AddToScene(*pScene);
    }

    pScene->AddGlyph(&m_tSearch);
    m_tfSearch.AddToScene(*pScene);
}

void Outliner::InitializeView()
{
    Window::InitializeView();

    m_textHeader.SetText("Outliner");

    ColorButtonOff(m_btAddMatter);
    ColorButtonOff(m_btAddLight);
    ColorButtonOff(m_btAddParticle);
    
    m_btAddMatter.SetTextString("+Matter");
    m_btAddLight.SetTextString("+Light");
    m_btAddParticle.SetTextString("+Particle");

    m_btAddMatter.SetTextScale(BUTTON_SCALE_X, BUTTON_SCALE_Y);
    m_btAddLight.SetTextScale(BUTTON_SCALE_X, BUTTON_SCALE_Y);
    m_btAddParticle.SetTextScale(BUTTON_SCALE_X, BUTTON_SCALE_Y);

    for (int i = 0; i < NUM_ACTOR_BUTTONS; i++)
    {
        ColorButtonOff(m_arActorButtons[i]);
        //m_arActorButtons[i].SetTextScale(BUTTON_SCALE_X, BUTTON_SCALE_Y);
        m_arActorButtons[i].SetTextString("Actor");
    }
    
    m_tSearch.SetText("Search:");
    m_tSearch.SetScale(BUTTON_SCALE_X, BUTTON_SCALE_Y);
    m_tSearch.SetColor(TEXT_COLOR);

    SetTextFieldColors(m_tfSearch);

}
