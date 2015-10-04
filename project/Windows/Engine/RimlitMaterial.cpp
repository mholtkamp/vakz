#include "RimlitMaterial.h"
#include "VGL.h"
#include "Scene.h"

RimlitMaterial::RimlitMaterial()
{
    m_nType = MATERIAL_RIMLIT;

    m_arDiffuseColor[0] = 1.0f;
    m_arDiffuseColor[1] = 1.0f;
    m_arDiffuseColor[2] = 1.0f;
    m_arDiffuseColor[3] = 1.0f;

    m_arRimColor[0] = 1.0f;
    m_arRimColor[1] = 0.0f;
    m_arRimColor[2] = 0.0f;
    m_arRimColor[3] = 1.0f;
}

RimlitMaterial::~RimlitMaterial()
{

}

void RimlitMaterial::SetRenderState(void*        pScene,
                                    unsigned int hProg)
{

    float arViewVec[3] = {0.0f};

    // Fetch uniform locations
    int hDiffuseColor = glGetUniformLocation(hProg, "uDiffuseColor");
    int hRimColor     = glGetUniformLocation(hProg, "uRimColor");
    int hViewVector   = glGetUniformLocation(hProg, "uViewVector");


    // Set uniforms
    glUniform4fv(hDiffuseColor, 1, m_arDiffuseColor);
    glUniform4fv(hRimColor, 1, m_arRimColor);

    reinterpret_cast<Scene*>(pScene)->GetCamera()->GetViewVector(arViewVec);
    glUniform3fv(hViewVector, 1, arViewVec);
}


