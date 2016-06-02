#ifndef WINDOW_H
#define WINDOW_H

#include "Scene.h"
#include "Quad.h"
#include "Text.h"
#include "Rect.h"
#include "TextField.h"
#include "Button.h"

static const float s_arColor1[4] = {1.0f, 0.9333f, 0.871f, 1.0f};
static const float s_arColor2[4] = {1.0f, 0.749f, 0.510f, 1.0f};
static const float s_arColor3[4] = {0.894f, 0.553f, 0.227f, 1.0f};
static const float s_arColor4[4] = {0.753f, 0.384f, 0.310f, 1.0f};
static const float s_arColor5[4] = {0.573f, 0.282f, 0.0f, 1.0f};

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

    int m_nVisible;
    Scene* m_pScene;
};

#endif