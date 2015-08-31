#include "Game.h"
#include "Constants.h"
#include "Vakz.h"
#include "VInput.h"
#include "Resources.h"
#include <math.h>

Game::Game()
{
    int i = 0;
    int j = 0;

    m_nPlayerSide = SIDE_1;

    SetupCamera();
    m_light.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    m_light.SetDirectionVector(0.0f, -1.0f, 0.0f);
    m_scene.AddLight(&m_light);

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

    m_nTouchDown    = 0;
    m_nJustTouched  = 0;
    m_nJustUp       = 0;
    m_fTouchDownX   = 0.0f;
    m_fTouchDownY   = 0.0f;
    m_fTouchUpX     = 0.0f;
    m_fTouchUpY     = 0.0f;

    // Set Game/Tiles for mages
    m_arMages[MAGE_1].SetGame(this);
    m_arMages[MAGE_2].SetGame(this);
}

Game::Game(int nSide) : Game()
{
    m_nPlayerSide = nSide;
    SetupCamera();
}

Game::~Game()
{

}

void Game::Update()
{
    float fDispX = 0.0f;
    float fDispY = 0.0f;

    int nDown = IsPointerDown();

    if (nDown == 0 &&
        m_nTouchDown != 0)
    {
        // Record the touch up location
        GetPointerPositionNormalized(m_fTouchUpX, m_fTouchUpY);
        m_nJustUp = 1;
    }
    else
    {
        m_nJustUp = 0;
    }

    if (m_nTouchDown == 0 &&
        nDown        != 0)
    {
        m_nJustTouched = 1;

        // Record the touch down location
        GetPointerPositionNormalized(m_fTouchDownX, m_fTouchDownY);
    }
    else
    {
        m_nJustTouched = 0;
    }
    m_nTouchDown = nDown;

    // Perform movement
    if (m_nJustUp)
    {
        fDispX = m_fTouchUpX - m_fTouchDownX;
        fDispY = m_fTouchUpY - m_fTouchDownY;

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
                m_arMages[m_nPlayerSide].Move(0, 1 + 2*m_nPlayerSide);
            }
            else if (fDispY > 0.0f)
            {
                m_arMages[m_nPlayerSide].Move(0, -1 + 2*m_nPlayerSide);
            }
        }
    }




    static float fRot = -40.0f;

    if(IsKeyDown(VKEY_Z))
    {
        fRot += 1.0f;
        m_camera.SetRotation(fRot, 0.0f, 0.0f);
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
        m_camera.SetPosition(3.5f, 7.0f, -7.5f);
        m_camera.SetRotation(-40.0f, 180.0f, 0.0f);
    }

    m_camera.SetProjectionType(Camera::CAMERA_PERSPECTIVE);
    m_scene.SetCamera(&m_camera);
}

void Game::RegisterScene()
{
    SetScene(&m_scene);
}