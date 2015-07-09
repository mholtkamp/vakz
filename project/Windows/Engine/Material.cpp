#include "Material.h"

//*****************************************************************************
// Constructor
//*****************************************************************************
Material::Material()
{
    m_nType = MATERIAL_NO_TYPE;
}

//*****************************************************************************
// Destructor
//*****************************************************************************
Material::~Material()
{

}

//*****************************************************************************
// GetType
//*****************************************************************************
int Material::GetType()
{
    return m_nType;
}