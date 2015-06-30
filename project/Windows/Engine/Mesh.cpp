#include "Mesh.h"

Mesh::Mesh()
{
    m_nType = MESH_NO_TYPE;
}

Mesh::~Mesh()
{

}

int Mesh::GetType()
{
    return m_nType;
}
