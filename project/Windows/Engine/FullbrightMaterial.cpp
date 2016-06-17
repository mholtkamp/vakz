#include "FullbrightMaterial.h"
#include "VGL.h"

//*****************************************************************************
// Constructor
//*****************************************************************************
FullbrightMaterial::FullbrightMaterial()
{
    m_nType = MATERIAL_FULLBRIGHT;
}

//*****************************************************************************
// Destructor
//*****************************************************************************
FullbrightMaterial::~FullbrightMaterial()
{
    
}

//*****************************************************************************
// SetRenderState
//*****************************************************************************
void FullbrightMaterial::SetRenderState(void        *pScene,
                                        unsigned int hProg)
{
    int hColor = glGetUniformLocation(hProg, "uColor");
    glUniform4fv(hColor, 1, m_arColor);
}
