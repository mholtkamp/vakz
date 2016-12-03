#include "PointLight.h"
#include "VGL.h"
#include "Scene.h"

PointLight::PointLight()
{
    m_nType = LIGHT_POINT;

    m_arPosition[0] = 0.0f;
    m_arPosition[1] = 0.0f;
    m_arPosition[2] = 0.0f;

    m_fIntensity = 1.0f;
    m_fRadius    = 1.0f;
}

PointLight::~PointLight()
{

}

void PointLight::SetRadius(float fRadius)
{
    m_fRadius = fRadius;
}

void PointLight::SetIntensity(float fIntensity)
{
    m_fIntensity = fIntensity;
}

void PointLight::SetPosition(float fX,
                             float fY,
                             float fZ)
{
    Scene* pScene = reinterpret_cast<Scene*>(GetScene());

    // Remove the light from wherever it was in the octree
    pScene->RemovePointLightFromOctree(this);

    // Update the position
    m_arPosition[0] = fX;
    m_arPosition[1] = fY;
    m_arPosition[2] = fZ;

    // Update the box center
    m_box.m_arCenter[0] = fX;
    m_box.m_arCenter[1] = fY;
    m_box.m_arCenter[2] = fZ;

    pScene->AddPointLightToOctree(this);
}

void PointLight::SetBox(Box& box)
{
    m_box = box;
}

float PointLight::GetRadius()
{
    return m_fRadius;
}

float PointLight::GetIntensity()
{
    return m_fIntensity;
}

Box* PointLight::GetBox()
{
    return &m_box;
}

float* PointLight::GetPosition()
{
    return m_arPosition;
}

void PointLight::SetRenderState(void*        pScene,
                                unsigned int hProg)
{
    // Pointlight OpenGL State is set in Matter class
}
