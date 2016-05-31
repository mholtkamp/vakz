#ifndef WINDOW_H
#define WINDOW_H

#include "Scene.h"
#include "Quad.h"
#include "Text.h"

static const float s_arColor1[4] = {1.0f, 0.0f, 0.0f, 1.0f};
static const float s_arColor2[4] = {0.85f, 0.5f, 0.42f, 1.0f};
static const float s_arColor3[4] = {0.68f, 0.5f, 0.5f, 1.0f};
static const float s_arColor4[4] = {0.7f, 0.7f, 0.4f, 1.0f};

#define HEADER_HEIGHT 0.05f

class Window
{
public:

    Window();
    ~Window();

    int IsVisible();

    void SetVisible(int nVisible);

    void SetPadding(float fPadding);

    virtual void HandleInput() = 0;

    virtual void UpdateView() = 0;

    virtual void InitializeView();

    virtual void RegisterScene(Scene* pScene);
    
private:

    Text m_textHeader;
    Quad m_quadHeader;
    Quad m_quadBackground;

    float m_fX;
    float m_fY;
    float m_fWidth;
    float m_fHeight;

    float m_fPadding;

    int m_nVisible;
    Scene* m_pScene;
};

#endif