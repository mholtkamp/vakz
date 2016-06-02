#ifndef WINDOW_H
#define WINDOW_H

#include "Scene.h"
#include "Quad.h"
#include "Text.h"
#include "Rect.h"
#include "TextField.h"
#include "Button.h"

static const float s_arColor1[4] = {1.0f, 0.851f, 0.741f, 1.0f};
static const float s_arColor2[4] = {0.992f, 0.710f, 0.502f, 1.0f};
static const float s_arColor3[4] = {0.835f, 0.510f, 0.270f, 1.0f};
static const float s_arColor4[4] = {0.686f, 0.361f, 0.122f, 1.0f};
static const float s_arColor5[4] = {0.494f, 0.208f, 0.0f, 1.0f};

#define HEADER_HEIGHT 0.08f
#define WINDOW_WIDTH 0.35f

class Window
{
public:

    Window();
    ~Window();

    int IsVisible();

    void SetVisible(int nVisible);

    void SetPadding(float fPadding);

    void SetSpacing(float fSpacing);

    void Initialize();

    virtual void HandleInput() = 0;

    virtual void UpdateView();

    virtual void InitializeView();

    virtual void RegisterScene(Scene* pScene);
    
protected:

    void ColorButtonDown(Button& button);
    void ColorButtonHover(Button& button);
    void ColorButtonOff(Button& button);

    void SetupTextField(TextField& field);

    Text m_textHeader;
    Quad m_quadHeader;
    Quad m_quadBackground;

    Rect m_rect;

    float m_fPadding;
    float m_fSpacing;

    int m_nVisible;
    Scene* m_pScene;
};

#endif