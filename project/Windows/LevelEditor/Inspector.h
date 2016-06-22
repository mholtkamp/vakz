#ifndef INSPECTOR_H
#define INSPECTOR_H

#include "Window.h"
#include "Asset.h"

class Inspector : public Window
{
public:
    Inspector();
    ~Inspector();

    void HandleInput();

    void UpdateView();

    void InitializeView();

    void RegisterScene(Scene* pScene);

    void HideViews();

private:

    // Common Views
    Text m_tName;
    TextField m_tfName;

    Text m_tVisible;
    Button m_btVisible;

    Text m_tPosition;
    TextField m_tfPosition[3];

    // Matter Views
    Text m_tRotation;
    TextField m_tfRotation[3];
    Text m_tScale;
    TextField m_tfScale[3];

    Text m_tMesh;
    Button m_btMesh;
    Button m_btAssignMesh;

    Text m_tTexture;
    Button m_btTexture;
    Button m_btAssignTexture;

    Text m_tRenderCollider;
    Button m_btRenderCollider;

    // Light Views
    // Position
    Text m_tBoxCenter;
    TextField m_tfBoxCenter[3];
    Text m_tBoxExtents;
    TextField m_tfBoxExtents[3];

    Text m_tColor;
    TextField m_tfColor[4];
    Text m_tIntensity;
    TextField m_tfIntensity;
    Text m_tRadius;
    TextField m_tfRadius;

    // Particle Views
    Text m_tCount;
    TextField m_tfCount;

    Text m_tSize;
    TextField m_tfMinSize;
    TextField m_tfMaxSize;

    Text m_tVelocity;
    TextField m_tfMinVelocity[3];
    TextField m_tMaxVelocity[3];

    TextField m_tfMinColor[4];
    TextField m_tfMaxColor[4];

    Text m_tGravity;
    TextField m_tfGravity;

    // Data
    Button* m_pPrevHover;
    TextField* m_pSelectedField;
    Asset* m_pAsset;
};

#endif