#include "Material.h"

Material::Material()
{
    m_nType = MATERIAL_NO_TYPE;
}

Material::~Material()
{

}

int Material::GetType()
{
    return m_nType;
}
