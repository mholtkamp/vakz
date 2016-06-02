#include "Window.h"
#include "Button.h"

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

    m_fPadding = 0.01f;
    m_fSpacing = 0.08f;

    m_nVisible = 1;
    m_pScene   = 0;
}

Window::~Window()
{

}

void Window::Initialize()
{
    InitializeView();
    UpdateView();
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

void Window::SetSpacing(float fSpacing)
{
    m_fSpacing = fSpacing;
}

void Window::InitializeView()
{
    m_textHeader.SetScale(DEFAULT_TEXT_SCALE_X,
                          DEFAULT_TEXT_SCALE_Y);
    m_textHeader.SetColor(s_arColor5[0],
                          s_arColor5[1],
                          s_arColor5[2],
                          s_arColor5[3]);

    m_quadBackground.SetColor(s_arColor5[0],
                              s_arColor5[1],
                              s_arColor5[2],
                              s_arColor5[3]);

    m_quadHeader.SetColor(s_arColor3[0],
                          s_arColor3[1],
                          s_arColor3[2],
                          s_arColor3[3]);
}

void Window::UpdateView()
{
    m_quadHeader.SetBox(m_rect.m_fX, 
                        m_rect.m_fY + m_rect.m_fHeight - HEADER_HEIGHT,
                        m_rect.m_fWidth,
                        HEADER_HEIGHT);

    m_quadBackground.SetBox(m_rect.m_fX,
                            m_rect.m_fY,
                            m_rect.m_fWidth,
                            m_rect.m_fHeight);

    m_textHeader.SetPosition(m_rect.m_fX + 0.015, 
                             m_rect.m_fY + m_rect.m_fHeight - HEADER_HEIGHT + 0.016);
}

void Window::RegisterScene(Scene* pScene)
{
    m_pScene = pScene;
    m_pScene->AddGlyph(&m_quadBackground);
    m_pScene->AddGlyph(&m_quadHeader);
    m_pScene->AddGlyph(&m_textHeader);
}

void Window::ColorButtonDown(Button& button)
{
    button.SetBackgroundColor(s_arColor3[0],
                              s_arColor3[1],
                              s_arColor3[2],
                              s_arColor3[3]);

    button.SetTextColor(s_arColor5[0],
                        s_arColor5[1],
                        s_arColor5[2],
                        s_arColor5[3]);

    button.EnableBorder(0);
}

void Window::ColorButtonHover(Button& button)
{
    button.SetBackgroundColor(s_arColor4[0],
                              s_arColor4[1],
                              s_arColor4[2],
                              s_arColor4[3]);

    button.SetTextColor(s_arColor2[0],
                        s_arColor2[1],
                        s_arColor2[2],
                        s_arColor2[3]);

    button.EnableBorder(1);

    button.SetBorderColor(s_arColor3[0],
                          s_arColor3[1],
                          s_arColor3[2],
                          s_arColor3[3]);
}

void Window::ColorButtonOff(Button& button)
{

    button.SetBackgroundColor(s_arColor4[0],
                              s_arColor4[1],
                              s_arColor4[2],
                              s_arColor4[3]);

    button.SetTextColor(s_arColor2[0],
                        s_arColor2[1],
                        s_arColor2[2],
                        s_arColor2[3]);

    button.EnableBorder(0);
}

void Window::SetupTextField(TextField& field)
{
    field.SetBackColor(s_arColor5[0],
                       s_arColor5[1],
                       s_arColor5[2],
                       s_arColor5[3]);
    field.SetTextColor(s_arColor3[0],
                       s_arColor3[1],
                       s_arColor3[2],
                       s_arColor3[3]);
    field.SetBorderColor(s_arColor3[0],
                         s_arColor3[1],
                         s_arColor3[2],
                         s_arColor3[3]);

    field.SetSelectBackColor(s_arColor4[0],
                             s_arColor4[1],
                             s_arColor4[2],
                             s_arColor4[3]);
    field.SetSelectTextColor(s_arColor2[0],
                             s_arColor2[1],
                             s_arColor2[2],
                             s_arColor2[3]);
    field.SetSelectBorderColor(s_arColor2[0],
                               s_arColor2[1],
                               s_arColor2[2],
                               s_arColor2[3]);

    field.SetTextScale(DEFAULT_TEXT_SCALE_X,
                       DEFAULT_TEXT_SCALE_Y);
}
