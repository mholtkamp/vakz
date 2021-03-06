#include "Game.h"
#include "Constants.h"
#include "Vakz.h"
#include "VInput.h"
#include "Resources.h"
#include <math.h>
#include "NetworkManager.h"
#include "Log.h"
#include "CardFactory.h"

Game::Game()
{
    Construct();
}

Game::Game(int nSide)
{
    Construct();
    m_nPlayerSide = nSide;
    m_pForest->SetSide(nSide);
    SetupCamera();
}

Game::~Game()
{

}

void Game::Construct()
{
    int i = 0;
    int j = 0;

    m_nPlayerSide = SIDE_1;

    SetupCamera();
    m_light.SetColor(1.0f, 1.0f, 1.0f);
    m_light.SetDirectionVector(0.0f, -1.0f, 0.0f);
    m_scene.AddActor(&m_light);

    // Set Game/Tiles for mages
    m_arMages[MAGE_1].SetGame(this);
    m_arMages[MAGE_2].SetGame(this);

    m_arMages[MAGE_1].SetSide(SIDE_1);
    m_arMages[MAGE_2].SetSide(SIDE_2);
    
    m_arMages[MAGE_1].Register(&m_scene);
    m_arMages[MAGE_2].Register(&m_scene);

    for (i = 0; i < GRID_HEIGHT; i++)
    {
        for (j = 0; j < GRID_WIDTH; j++)
        {
            m_arTiles[j][i].Register(&m_scene);
            m_arTiles[j][i].SetPosition(j, i);
            m_arTiles[j][i].SetGame(this);

            if (j < GRID_WIDTH / 2)
            {
                m_arTiles[j][i].m_nOwner = SIDE_1;
            }
            else
            {
                m_arTiles[j][i].m_nOwner = SIDE_2;
            }

            m_arTiles[j][i].UpdateTexture();
        }
    }

    memset(m_arHand, 0, sizeof(Card*) * HAND_SIZE);

    // Register HUD
    m_hud.Register(&m_scene);

    // Create Forest and register
    m_pForest = new Forest();
    m_pForest->Register(&m_scene);

    m_nTouchDownLeft    = 0;
    m_nJustTouchedLeft  = 0;
    m_nJustUpLeft       = 0;
    m_fTouchDownXLeft   = 0.0f;
    m_fTouchDownYLeft   = 0.0f;
    m_fTouchUpXLeft     = 0.0f;
    m_fTouchUpYLeft     = 0.0f;

    m_nTouchDownRight    = 0;
    m_nJustTouchedRight  = 0;

    m_pNetworkManager = 0;

    // Timers
    m_timerDraw.Start();
    m_timerMana.Start();
    m_timerMove.Start();

    // Initialize activation related members
    for (i = 0; i < MAX_ACTIVATIONS; i++)
    {
        // If an activation is non-zero, that means there is an activation in use
        m_arActivations[i] = 0;
 
        // Material will default to diffuse white,
        // Mesh pointer is set to null in constructor.
        m_arActMatters[i].SetMaterial(g_pDiffuseMaterial);

        // Add to scene
        m_scene.AddActor(&m_arActMatters[i]);
    }
}

Scene* Game::GetScene()
{
    return &m_scene;
}

void Game::Update()
{
    int i = 0;
    int j = 0;

    float fX     = 0.0f;
    float fY     = 0.0f;
    float fDispX = 0.0f;
    float fDispY = 0.0f;
    
    float fDrawTime = 0.0f;
    float fManaTime = 0.0f;
    float fMoveTime = 0.0f;

    int nDownLeft   = 0;
    int nDownRight  = 0;
    int nLeftIndex  = 0;
    int nRightIndex = 0;

    // Update Draw and Mana timers
    m_timerDraw.Stop();
    m_timerMana.Stop();

    fDrawTime = m_timerDraw.Time();
    fManaTime = m_timerMana.Time();

    if (fDrawTime >= DRAW_TICK_TIME)
    {
        // Regen draw charge
        m_arMages[m_nPlayerSide].RegenDrawCharge();
        m_hud.SetDrawCharge(m_arMages[m_nPlayerSide].GetDrawCharge());
        m_timerDraw.Start();
    }
    
    if (fManaTime >= MANA_TICK_TIME)
    {
        // Regen mana charge
        m_arMages[m_nPlayerSide].RegenMana();
        m_hud.SetMana(m_arMages[m_nPlayerSide].GetMana());
        m_timerMana.Start();
    }

    // Figure out what pointers are down
    if(IsPointerDown(0) != 0)
    {
        GetPointerPositionNormalized(fX, fY, 0);
        if (fX < 0.0f)
        {
            nDownLeft = 1;
            nLeftIndex = 0;

            if (IsPointerDown(1) != 0)
            {
                if (fX > 0.0f)
                {
                    nDownRight = 1;
                    nRightIndex = 1;
                }
            }
        }
        else if (IsPointerDown(1) != 0)
        {
            GetPointerPositionNormalized(fX, fY, 1);
            if (fX < 0.0f)
            {
                nDownLeft = 1;
                nLeftIndex = 1;
            }
            else
            {
                nDownRight = 1;
                nDownLeft  = 0;
            }
        }
        else
        {
            nDownRight = 1;
            nDownLeft  = 0;
        }
    }
    else
    {
        nDownLeft  = 0;
        nDownRight = 0;
    }

    // Check if left touch was just released
    if (nDownLeft        == 0 &&
        m_nTouchDownLeft != 0)
    {
        // Record the touch up location
        GetPointerPositionNormalized(m_fTouchUpXLeft, m_fTouchUpYLeft, nLeftIndex);
        m_nJustUpLeft = 1;
    }
    else
    {
        m_nJustUpLeft = 0;
    }

    // Check if left touch was just pressed
    if (m_nTouchDownLeft == 0 &&
        nDownLeft        != 0)
    {
        m_nJustTouchedLeft = 1;

        // Record the touch down location
        GetPointerPositionNormalized(m_fTouchDownXLeft, m_fTouchDownYLeft, nLeftIndex);
    }
    else
    {
        m_nJustTouchedLeft = 0;
    }
    m_nTouchDownLeft = nDownLeft;

    // Perform movement
    m_timerMove.Stop();
    fMoveTime = m_timerMove.Time();

    if (m_nJustUpLeft                                       &&
        fMoveTime >= m_arMages[m_nPlayerSide].GetMoveTime() &&
        m_arMages[m_nPlayerSide].IsStunned() == 0)
    {
        fDispX = m_fTouchUpXLeft - m_fTouchDownXLeft;
        fDispY = m_fTouchUpYLeft - m_fTouchDownYLeft;

        // Check the direction of gesture
        if (fabs(fDispX) > fabs(fDispY))
        {
            // Most displacement happened in X direction
            if (fDispX < 0.0f)
            {
                m_arMages[m_nPlayerSide].Move( -1 + 2*m_nPlayerSide, 0);
                m_timerMove.Start();
            }
            else if (fDispX > 0.0f)
            {
                m_arMages[m_nPlayerSide].Move( 1 - 2*m_nPlayerSide, 0);
                m_timerMove.Start();
            }
        }
        else
        {
            // Most displacement happened in Y direction, so move player in Z direction.
            if (fDispY < 0.0f)
            {
                m_arMages[m_nPlayerSide].Move(0, 1 - 2*m_nPlayerSide);
                m_timerMove.Start();
            }
            else if (fDispY > 0.0f)
            {
                m_arMages[m_nPlayerSide].Move(0, -1 + 2*m_nPlayerSide);
                m_timerMove.Start();
            }
        }
    }

    // Check if right touch is just down.
    if (nDownRight        != 0 &&
        m_nTouchDownRight == 0)
    {
        m_nJustTouchedRight = 1;
    }
    else
    {
        m_nJustTouchedRight = 0;
    }
    m_nTouchDownRight = nDownRight;

    if (m_nJustTouchedRight != 0)
    {
        GetPointerPositionNormalized(fX,fY, nRightIndex);

        if (m_hud.IsCastPressed(fX, fY)          != 0 &&
            m_arMages[m_nPlayerSide].IsStunned() == 0)
        {
            if (m_arHand[0] != 0)
            {
                // Only cast card if there is enough mana.
                if (m_arHand[0]->GetManaCost() < m_arMages[m_nPlayerSide].GetMana())
                {
                    m_arMages[m_nPlayerSide].Drain(m_arHand[0]->GetManaCost());
                    m_hud.SetMana(m_arMages[m_nPlayerSide].GetMana());
                    m_arHand[0]->Cast(this, m_nPlayerSide);
                    
                    m_arMages[m_nPlayerSide].PlayCastAnimation();

                    // Send Card message to server
                    m_msgCard.Clear();
                    m_msgCard.m_nCard   = m_arHand[0]->GetID();
                    m_msgCard.m_nCaster = m_nPlayerSide;
                    reinterpret_cast<NetworkManager*>(m_pNetworkManager)->Send(m_msgCard);

                    RemoveCardFromHand(0);
                }
            }
        }
        else if (m_hud.IsRotatePressed(fX, fY) != 0)
        {
            RotateHandLeft();
        }
    }

    // Update all activations
    for (i = 0; i < MAX_ACTIVATIONS; i++)
    {
        if (m_arActivations[i] != 0)
        {
            m_arActivations[i]->Update();

            if (m_arActivations[i]->m_nExpired != 0)
            {
                m_arActivations[i]->OnDestroy();
                delete m_arActivations[i];
                m_arActivations[i] = 0;
            }
        }
    }

    // Update all tiles
    for (i = 0; i < GRID_WIDTH; i++)
    {
        for (j = 0; j < GRID_HEIGHT; j++)
        {
            m_arTiles[i][j].Update();
        }
    }
}

void Game::RemoveCardFromHand(int nIndex)
{
    int i = 0;

    if (m_arHand[nIndex] != 0)
    {
        for (i = 0; i < HAND_SIZE - 1; i++)
        {
            m_arHand[i] = m_arHand[i+1];
        }

        m_arHand[HAND_SIZE - 1] = 0;
        m_hud.SetHandTextures(m_arHand);
    }
}

void Game::SetupCamera()
{
    if (m_nPlayerSide == SIDE_1)
    {

        m_camera.SetPosition(3.75f, 6.8f, 7.1f);
        m_camera.SetRotation(-45.0f, 0.0f, 0.0f);

        //m_camera.SetPosition(3.8f, 7.8f, 6.5f);
        //m_camera.SetRotation(-50.0f, 0.0f, 0.0f);

        //m_camera.SetPosition(3.8f, 6.5f, 8.5f);
        //m_camera.SetRotation(-40.0f, 0.0f, 0.0f);
    }
    else
    {
        m_camera.SetPosition(3.75f, 6.80f, -5.1f);
        m_camera.SetRotation(-45.0f, 180.0f, 0.0f);
    }

    m_camera.SetProjectionType(Camera::CAMERA_PERSPECTIVE);
    m_scene.SetCamera(&m_camera);
}

void Game::RegisterScene()
{
    SetScene(&m_scene);
}

void Game::SendPosition(int nPlayer, int nX, int nZ)
{
    if (m_pNetworkManager != 0)
    {
        m_msgPosition.m_nPlayer = nPlayer;
        m_msgPosition.m_nX      = nX;
        m_msgPosition.m_nZ      = nZ;

        reinterpret_cast<NetworkManager*>(m_pNetworkManager)->Send(m_msgPosition);
    }
}

void Game::UpdatePosition(int nPlayer,
                          int nX,
                          int nZ)
{
    if (nPlayer >= MAGE_1 &&
        nPlayer <= MAGE_2)
    {
        m_arMages[nPlayer].UpdatePosition(nX, nZ);
    }
}

void Game::UpdateHealth(int nPlayer,
                        int nHealth)
{
    if (nPlayer >= MAGE_1 &&
        nPlayer <= MAGE_2)
    {
        m_arMages[nPlayer].SetHealth(nHealth);
        
        if (nPlayer == m_nPlayerSide)
        {
            m_hud.SetHealth(nHealth);
        }
        else
        {
            m_hud.SetEnemyHealth(nHealth);
        }
    }
}

void Game::UpdateStatus(int nPlayer,
                        int nStatus,
                        int nAfflicted)
{
    if (nPlayer < 0 &&
        nPlayer >= NUM_MAGES)
    {
        return;
    }

    switch(nStatus)
    {
    case STATUS_FLOATING:
        m_arMages[nPlayer].SetFloating(nAfflicted);
        break;
    case STATUS_POISONED:
        m_arMages[nPlayer].SetPoisoned(nAfflicted);
        break;
    case STATUS_STUNNED:
        m_arMages[nPlayer].SetStunned(nAfflicted);
        break;
    case STATUS_CONFUSED:
        m_arMages[nPlayer].SetConfused(nAfflicted);
        break;
    case STATUS_INVISIBLE:
        m_arMages[nPlayer].SetInvisible(nAfflicted);
        break;
    case STATUS_INVINCIBLE:
        m_arMages[nPlayer].SetInvincible(nAfflicted);
        break;
    case STATUS_ROOTED:
        m_arMages[nPlayer].SetRooted(nAfflicted);
        break;
    default:
        LogDebug("Unknown player status received.");
        break;
    }
}

void Game::UseCard(int nCard,
                   int nCaster)
{
    Card* pCard = InstantiateCard(nCard);

    if (pCard != 0)
    {
        pCard->Cast(this, nCaster);
        m_arMages[nCaster].PlayCastAnimation();
        delete pCard;
        pCard = 0;
    }
    else
    {
        LogError("Game::UseCard, could not instantiate card from ID.");
    }
}

void Game::AddCardsToHand(int* arCards)
{
    int i         = 0;
    int nCardSlot = 0;

    for (i = 0; i < HAND_SIZE; i++)
    {
        if(m_arHand[i] == 0)
        {
            nCardSlot = i;
            break;
        }
    }

    if (i == HAND_SIZE)
    {
        LogError("Could not add cards to hand because hand is full.");
        return;
    }

    for (i = 0; i < HAND_SIZE; i++)
    {
        if(arCards[i] > 0)
        {
            if (nCardSlot >= HAND_SIZE)
            {
                LogError("Could not add card to hand because hand is full.");
                break;
            }

            m_arHand[nCardSlot] = InstantiateCard(arCards[i]);
            nCardSlot++;
        }
    }

    m_hud.SetHandTextures(m_arHand);
}

void Game::RotateHandLeft()
{
    int i       = 0;
    Card* pTemp = 0;

    if (m_arHand[0] == 0)
    {
        return;
    }

    pTemp = m_arHand[0];

    for (i = 0; i < HAND_SIZE - 1; i++)
    {
        if (m_arHand[i+1] != 0)
        {
            m_arHand[i] = m_arHand[i+1];
        }
        else
        {
            m_arHand[i] = pTemp;
            break;
        }
    }

    if (i == HAND_SIZE - 1)
    {
        // Hand was full of cards
        m_arHand[HAND_SIZE - 1] = pTemp;
    }

    m_hud.SetHandTextures(m_arHand);
}

void Game::RotateHandRight()
{

}

Mage* Game::GetMage(int nIndex)
{
    if (nIndex >= SIDE_1 &&
        nIndex <= SIDE_2)
    {
        return &(m_arMages[nIndex]);
    }
    else
    {
        return 0;
    }
}

void Game::UpdateTile(int nX,
                      int nZ,
                      int nOwner,
                      int nType)
{
    if(nX >= 0          &&
       nX <  GRID_WIDTH &&
       nZ >= 0          &&
       nZ <  GRID_HEIGHT)
    {
        m_arTiles[nX][nZ].SetOwner(nOwner);
        m_arTiles[nX][nZ].SetType(nType);
    }
}
