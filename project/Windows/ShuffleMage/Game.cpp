#include "Game.h"
#include "Constants.h"
#include "Vakz.h"
#include "VInput.h"

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

}

Game::Game(int nSide)
{
    Game();

    m_nPlayerSide = nSide;
    SetupCamera();
}

Game::~Game()
{

}

void Game::Update()
{
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
        m_camera.SetPosition(3.5f, 10.0f, 11.0f);
        m_camera.SetRotation(-40.0f, 0.0f, 0.0f);
    }
    else
    {
        m_camera.SetPosition(3.5f, 10.0f, -9.0f);
        m_camera.SetRotation(-40.0f, 180.0f, 0.0f);
    }

    m_camera.SetProjectionType(Camera::CAMERA_PERSPECTIVE);
    m_scene.SetCamera(&m_camera);
}

void Game::RegisterScene()
{
    SetScene(&m_scene);
}