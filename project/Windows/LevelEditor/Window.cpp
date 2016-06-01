#include "Window.h"

#define DEFAULT_WINDOW_WIDTH 0.5f

#define DEFAULT_TEXT_SCALE_X 0.5f
#define DEFAULT_TEXT_SCALE_Y 0.8f

#define VERTICAL_SPACE 0.05f

Window::Window()
{
    m_rect.m_fX      = 0.0f;
    m_rect.m_fY      = 0.0f;
    m_rect.m_fWidth  = 1.0f;
    m_rect.m_fHeight = 1.0f;

    m_fPadding = 0.0f;

    m_nVisible = 1;
    m_pScene   = 0;
}

Window::~Window()
{

}

int Window::IsVisible()
{
    return m_nVisible;
}

void Window::SetVisible(int nVisible)
{
    m_nVisible = nVisible;
}

void Window::SetPadding(float fPadding)
{
    m_fPadding = fPadding;
}

void Window::InitializeView()
{
    m_textHeader.SetScale(DEFAULT_TEXT_SCALE_X,
                          DEFAULT_TEXT_SCALE_Y);
    m_textHeader.SetColor(s_arColor1[0],
                          s_arColor1[1],
                          s_arColor1[2],
                          s_arColor1[3]);

    m_quadBackground.SetColor(s_arColor4[0],
                              s_arColor4[1],
                              s_arColor4[2],
                              s_arColor4[3]);

    m_quadHeader.SetColor(s_arColor3[0],
                          s_arColor3[1],
                          s_arColor3[2],
                          s_arColor3[3]);
}
void Window::RegisterScene(Scene* pScene)
{
    m_pScene = pScene;
    m_pScene->AddGlyph(&m_quadBackground);
    m_pScene->AddGlyph(&m_quadHeader);
    m_pScene->AddGlyph(&m_textHeader);
}
