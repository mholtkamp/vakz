#include "Outliner.h"
#include "VInput.h"
#include "PointLight.h"

#include <stdio.h>

#define ADD_BUTTON_WIDTH 0.103f
#define ADD_BUTTON_HEIGHT 0.06f
#define ADD_HORI_SPACE 0.01f

#define BUTTON_SCALE_X 0.4f
#define BUTTON_SCALE_Y 0.68f

#define ACTOR_BUTTON_WIDTH 0.265f
#define ACTOR_BUTTON_HEIGHT 0.05f

#define BUTTON_OFF_Y 0.008f

#define TEXT_COLOR s_arColor2

Outliner::Outliner()
{
    m_rect.m_fX = 1.0f - WINDOW_WIDTH;
    m_rect.m_fY = 0.13f;
    m_rect.m_fWidth = WINDOW_WIDTH;
    m_rect.m_fHeight = 0.87f;

    m_pPrevHover = 0;
    m_nDispOffset = 0;
    
    m_pSelectedActor = 0;
    m_nSelectedActor = 0;

    m_nMatterID = 0;
    m_nLightID = 0;
    m_nParticleID = 0;
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
    Window::RegisterScene(pScene);

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

    m_btAddMatter.SetTextScale(0.32f, BUTTON_SCALE_Y);
    m_btAddLight.SetTextScale(0.37f, BUTTON_SCALE_Y);
    m_btAddParticle.SetTextScale(0.25f, BUTTON_SCALE_Y);

    for (int i = 0; i < NUM_ACTOR_BUTTONS; i++)
    {
        ColorButtonOff(m_arActorButtons[i]);
        //m_arActorButtons[i].SetTextScale(BUTTON_SCALE_X, BUTTON_SCALE_Y);
        m_arActorButtons[i].SetTextString("Actor");
        m_arActorButtons[i].SetTextOffsetY(0.0055f);
    }
    
    m_tSearch.SetText("Search:");
    m_tSearch.SetScale(BUTTON_SCALE_X, BUTTON_SCALE_Y);
    m_tSearch.SetColor(TEXT_COLOR);

    SetTextFieldColors(m_tfSearch);
    m_tfSearch.SetMaxSize(16);
    m_tfSearch.SetTextScale(0.3f, BUTTON_SCALE_Y);
    m_tfSearch.SetClearOnSelect(0);
}

void Outliner::UpdateView()
{
    Window::UpdateView();

    float fX = m_rect.m_fX + m_fPadding;
    float fY = m_rect.m_fY + m_rect.m_fHeight - HEADER_HEIGHT - 0.1f;

    m_btAddMatter.SetRect(fX,
                          fY,
                          ADD_BUTTON_WIDTH,
                          ADD_BUTTON_HEIGHT);

    m_btAddLight.SetRect(fX + ADD_BUTTON_WIDTH + ADD_HORI_SPACE,
                         fY,
                         ADD_BUTTON_WIDTH,
                         ADD_BUTTON_HEIGHT);

    m_btAddParticle.SetRect(fX + ADD_BUTTON_WIDTH*2 + ADD_HORI_SPACE*2,
                            fY,
                            ADD_BUTTON_WIDTH,
                            ADD_BUTTON_HEIGHT);

    // Actor Buttons
    fX = m_rect.m_fX + m_fPadding + 0.03f;
    fY -= m_fSpacing;

    ListNode* pNode = m_lActors.Get(m_nDispOffset);

    for (int i = 0; i < NUM_ACTOR_BUTTONS; i++)
    {
        m_arActorButtons[i].SetRect(fX,
                                    fY,
                                    ACTOR_BUTTON_WIDTH,
                                    ACTOR_BUTTON_HEIGHT);

        if (pNode != 0)
        {
            if (strlen(m_tfSearch.GetText()) > 0)
            {
                // There is a search term entered, so filter
                // for items containing term
                Actor* pActor = reinterpret_cast<Actor*>(pNode->m_pData);
                if (strstr(pActor->GetName(), m_tfSearch.GetText()))
                {
                    m_arActorButtons[i].SetVisible(1);
                    m_arActorButtons[i].SetTextString(pActor->GetName());

                    pNode = pNode->m_pNext;
                }
                else
                {
                    // Search string not found in name so....
                    // Iterate the actor node and decrement the counter
                    pNode = pNode->m_pNext;
                    i--;
                    continue;
                }
            }
            else
            {
                // No search term entered
                m_arActorButtons[i].SetVisible(1);
                m_arActorButtons[i].SetTextString(reinterpret_cast<Actor*>(pNode->m_pData)->GetName());

                pNode = pNode->m_pNext;
            }

        }
        else
        {
            m_arActorButtons[i].SetVisible(0);
        }
        
        fY -= m_fSpacing;
    }

    // Search Field
    fX = m_rect.m_fX + m_fPadding;

    m_tSearch.SetPosition(fX, fY);
    
    m_tfSearch.SetRect(fX + 0.12f,
                       fY - BUTTON_OFF_Y,
                       0.2f,
                       ADD_BUTTON_HEIGHT);
}

void Outliner::HandleInput()
{
    if (m_nVisible = 0)
    {
        // Do not handle input
        return;
    }

    float fX = 0.0f;
    float fY = 0.0f;
    GetPointerPositionNormalized(fX, fY);

    // Clear previous hover button
    if (m_pPrevHover != 0)
    {
        ColorButtonOff(*m_pPrevHover);
    }

    // Highlight buttons if pointer is down on them
    if (IsPointerDown())
    {
        Button* pDown = 0;

        if (m_btAddMatter.IsTouched())
        {
            ColorButtonDown(m_btAddMatter);
            pDown = &m_btAddMatter;
        }
        else if (m_btAddLight.IsTouched())
        {
            ColorButtonDown(m_btAddLight);
            pDown = &m_btAddLight;
        }
        else if (m_btAddParticle.IsTouched())
        {
            ColorButtonDown(m_btAddParticle);
            pDown = &m_btAddParticle;
        }
        else
        {
            for (int i = 0; i < NUM_ACTOR_BUTTONS; i++)
            {
                if (m_arActorButtons[i].IsTouched())
                {
                    ColorButtonDown(m_arActorButtons[i]);
                    pDown = &m_arActorButtons[i];
                }
            }
        }

        m_pPrevHover = pDown;
    }
    else
    {
        Button* pHover = 0;

        if (m_btAddMatter.IsPointerHovering())
        {
            ColorButtonHover(m_btAddMatter);
            pHover = &m_btAddMatter;
        }
        else if (m_btAddLight.IsPointerHovering())
        {
            ColorButtonHover(m_btAddLight);
            pHover = &m_btAddLight;
        }
        else if (m_btAddParticle.IsPointerHovering())
        {
            ColorButtonHover(m_btAddParticle);
            pHover = &m_btAddParticle;
        }
        else
        {
            for (int i = 0; i < NUM_ACTOR_BUTTONS; i++)
            {
                if (m_arActorButtons[i].IsPointerHovering())
                {
                    ColorButtonHover(m_arActorButtons[i]);
                    pHover = &m_arActorButtons[i];
                }
            }
        }

        m_pPrevHover = pHover;
    }

    // Do important stuff
    if (IsPointerJustUp())
    {
        char arName[ACTOR_NAME_BUFFER_SIZE] = {0};
        if (m_btAddMatter.IsPointerHovering())
        {
            // Create a new matter actor
            Matter* pNewMatter = new Matter();
            m_lActors.Add(pNewMatter);

            sprintf(arName, "Matter%d", m_nMatterID);
            arName[ACTOR_NAME_BUFFER_SIZE - 1] = 0;
            m_nMatterID++;

            pNewMatter->SetName(arName);
            UpdateView();
        }
        else if (m_btAddLight.IsPointerHovering())
        {
            // Create a new point light actor
            PointLight* pNewLight = new PointLight();
            m_lActors.Add(pNewLight);

            sprintf(arName, "Light%d", m_nLightID);
            arName[ACTOR_NAME_BUFFER_SIZE - 1] = 0;
            m_nLightID++;

            pNewLight->SetName(arName);
            UpdateView();
        }
        else if (m_btAddParticle.IsPointerHovering())
        {
            // Create a new particle system actor
            ParticleSystem* pNewParticle = new ParticleSystem();
            m_lActors.Add(pNewParticle);

            sprintf(arName, "Particle%d", m_nParticleID);
            arName[ACTOR_NAME_BUFFER_SIZE - 1] = 0;
            m_nParticleID++;

            pNewParticle->SetName(arName);
            UpdateView();
        }
        else
        {
            for (int i = 0; i < NUM_ACTOR_BUTTONS; i++)
            {
                if (m_arActorButtons[i].IsVisible() &&
                    m_arActorButtons[i].IsPointerHovering())
                {
                    Actor* pSelActor = 0;
                    ListNode* pNode = m_lActors.Get(i + m_nDispOffset);

                    if (pNode != 0)
                    {
                        pSelActor = reinterpret_cast<Actor*>(pNode->m_pData);
                    }
                    else
                    {
                        // Node doesn't exist, should never reach this point
                        break;
                    }

                    if (pSelActor == m_pSelectedActor)
                    {
                        if (m_pEditor != 0)
                        {
                            //reinterpret_cast<LevelEditor*>(m_pEditor)->SetInspectedActor(m_pSelectedActor);
                        }
                    }
                    else
                    {
                        // Undown the previous selected actor
                        if (m_nSelectedActor - m_nDispOffset >= 0 &&
                            m_nSelectedActor - m_nDispOffset < NUM_ACTOR_BUTTONS)
                            ColorButtonOff(m_arActorButtons[m_nSelectedActor - m_nDispOffset]);

                        // Store address and 
                        m_pSelectedActor = pSelActor;
                        m_nSelectedActor = i + m_nDispOffset;
                    }

                    UpdateView();
                }
            }
        }

        if (m_tfSearch.IsSelected())
        {
            UpdateView();
        }
    }
    if (IsKeyJustUp(VKEY_ENTER))
    {
        if (m_tfSearch.IsSelected())
        {
            m_tfSearch.SetSelect(0);
            UpdateView();
        }
    }
    
    // Mouse Wheel
    if (m_rect.Contains(fX, fY) &&
        GetScrollWheelDelta() != 0)
    {
        // Color the previously selected button off
        if (m_nSelectedActor - m_nDispOffset >= 0 &&
            m_nSelectedActor - m_nDispOffset < NUM_ACTOR_BUTTONS)
            ColorButtonOff(m_arActorButtons[m_nSelectedActor - m_nDispOffset]);

        m_nDispOffset += -1 * GetScrollWheelDelta();

        if (m_nDispOffset >= m_lActors.Count())
            m_nDispOffset = m_lActors.Count() - 1;
        if (m_nDispOffset < 0)
            m_nDispOffset = 0;

        UpdateView();
    }

    m_tfSearch.Update(IsPointerJustUp(), fX, fY);

    // Always hightlight selected actor
    if (m_pSelectedActor != 0 &&
        m_nSelectedActor - m_nDispOffset >= 0 &&
        m_nSelectedActor - m_nDispOffset <  NUM_ACTOR_BUTTONS)
        ColorButtonDown(m_arActorButtons[m_nSelectedActor - m_nDispOffset]);
}
