#include "Inspector.h"

Inspector::Inspector()
{
    m_pPrevHover = 0;
    m_pSelectedField = 0;
    m_pAsset = 0;
}

Inspector::~Inspector()
{

}

void Inspector::RegisterScene(Scene* pScene)
{
    Window::RegisterScene(pScene);
    
    // Common
    pScene->AddGlyph(&m_tName);
    m_tfName.AddToScene(*pScene);
    pScene->AddGlyph(&m_tVisible);
    m_btVisible.AddToScene(*pScene);
    pScene->AddGlyph(&m_tPosition);
    
    // Matter
    pScene->AddGlyph(&m_tRotation);
    pScene->AddGlyph(&m_tScale);

    pScene->AddGlyph(&m_tMesh);
    m_btMesh.AddToScene(*pScene);
    m_btAssignMesh.AddToScene(*pScene);

    pScene->AddGlyph(&m_tTexture);
    m_btTexture.AddToScene(*pScene);
    m_btAssignTexture.AddToScene(*pScene);

    pScene->AddGlyph(&m_tRenderCollider);
    m_btRenderCollider.AddToScene(*pScene);

}
