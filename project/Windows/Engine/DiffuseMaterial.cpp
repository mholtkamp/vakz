#include "DiffuseMaterial.h"
#include "VGL.h"

//*****************************************************************************
// Constructor
//*****************************************************************************
DiffuseMaterial::DiffuseMaterial()
{
    m_nType = MATERIAL_DIFFUSE;
}

//*****************************************************************************
// Destructor
//*****************************************************************************
DiffuseMaterial::~DiffuseMaterial()
{

}

//*****************************************************************************
// SetRenderState
//*****************************************************************************
void DiffuseMaterial::SetRenderState(void*        pScene,
                                     unsigned int hProg)
{
    int hColor = glGetUniformLocation(hProg, "uDiffuseColor");
    glUniform4fv(hColor, 1, m_arColor);
}
