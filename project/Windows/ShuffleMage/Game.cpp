#include "Game.h"
#include "Constants.h"
#include "Vakz.h"
#include "VInput.h"
#include "Resources.h"
#include <math.h>
#include "NetworkManager.h"
#include "Log.h"

Game::Game()
{
    Construct();
}

Game::Game(int nSide)
{
    Construct();
    m_nPlayerSide = nSide;
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
    m_light.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    m_light.SetDirectionVector(0.0f, -1.0f, 0.0f);
    m_scene.AddLight(&m_light);

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

            if (j < GRID_WIDTH / 2)
            {
                m_arTiles[j][i].SetOwner(SIDE_1);
            }
            else
            {
                m_arTiles[j][i].SetOwner(SIDE_2);
            }
        }
    }

    // Register HUD
    m_hud.Register(&m_scene);

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
}

void Game::Update()
{
    float fX     = 0.0f;
    float fY     = 0.0f;
    float fDispX = 0.0f;
    float fDispY = 0.0f;
    
    int nDownLeft   = 0;
    int nDownRight  = 0;
    int nLeftIndex  = 0;
    int nRightIndex = 0;

    // Figure out what pointers are down
    if(IsPointerDown(0) != 0)
    {
        GetPointerPositionNormalized(fX, fY, 0);
        if (fX < 0.0f)
        {
            nDownLeft = 1;
            nLeftIndex = 0;
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
    if (m_nJustUpLeft)
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
            }
            else if (fDispX > 0.0f)
            {
                m_arMages[m_nPlayerSide].Move( 1 - 2*m_nPlayerSide, 0);
            }
        }
        else
        {
            // Most displacement happened in Y direction, so move player in Z direction.
            if (fDispY < 0.0f)
            {
                m_arMages[m_nPlayerSide].Move(0, 1 - 2*m_nPlayerSide);
            }
            else if (fDispY > 0.0f)
            {
                m_arMages[m_nPlayerSide].Move(0, -1 + 2*m_nPlayerSide);
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

        if (m_hud.IsCastPressed(fX, fY) != 0)
        {
            // TODO: Add cast logic
            LogDebug("Cast pressed!");
        }
        else if (m_hud.IsRotatePressed(fX, fY) != 0)
        {
            // TODO: Add rotate logic
            LogDebug("Rotate pressed!");
        }
    }
}

void Game::SetupCamera()
{
    if (m_nPlayerSide == SIDE_1)
    {
        m_camera.SetPosition(3.8f, 6.5f, 8.5f);
        m_camera.SetRotation(-40.0f, 0.0f, 0.0f);
    }
    else
    {
        m_camera.SetPosition(3.8f, 6.5f, -6.5f);
        m_camera.SetRotation(-40.0f, 180.0f, 0.0f);
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
