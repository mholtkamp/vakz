#include "StaticMesh.h"
#include "VGL.h"

//*****************************************************************************
// Constructor
//*****************************************************************************
StaticMesh::StaticMesh()
{
    m_nType = MESH_STATIC;

    m_nVertexCount = 0;
    m_unVBO        = 0;

    m_pClientPosition = 0;
    m_pClientTexCoord = 0;
    m_pClientNormal   = 0;
}

//*****************************************************************************
// Destructor
//*****************************************************************************
StaticMesh::~StaticMesh()
{

}

//*****************************************************************************
// SetVertexCount
//*****************************************************************************
void StaticMesh::SetVertexCount(int nCount)
{
    m_nVertexCount = nCount;
}

//*****************************************************************************
// SetPositionArray
//*****************************************************************************
void StaticMesh::SetPositionArray(float* pArray)
{
    m_pClientPosition = pArray;
}

//*****************************************************************************
// SetTexCoordArray
//*****************************************************************************
void StaticMesh::SetTexCoordArray(float* pArray)
{
    m_pClientTexCoord = pArray;
}

//*****************************************************************************
// SetNormalArray
//*****************************************************************************
void StaticMesh::SetNormalArray(float* pArray)
{
    m_pClientNormal = pArray;
}

//*****************************************************************************
// GetVertexCount
//*****************************************************************************
int StaticMesh::GetVertexCount()
{
    return m_nVertexCount;
}

//*****************************************************************************
// GetPositionArray
//*****************************************************************************
float* StaticMesh::GetPositionArray()
{
    return m_pClientPosition;
}

//*****************************************************************************
// GetTexCoordArray
//*****************************************************************************
float* StaticMesh::GetTexCoordArray()
{
    return m_pClientTexCoord;
}

//*****************************************************************************
// GetNormalArray
//*****************************************************************************
float* StaticMesh::GetNormalArray()
{
    return m_pClientNormal;
}

//*****************************************************************************
// SetRenderState
//*****************************************************************************
void StaticMesh::SetRenderState(void*        pScene,
                                unsigned int hProg)
{
    // Assign vertex attributes
    int hPosition = glGetAttribLocation(hProg, "aPosition");
    int hTexCoord = glGetAttribLocation(hProg, "aTexCoord");
    int hNormal   = glGetAttribLocation(hProg, "aNormal");

    // All static meshes will have a position array.
    glEnableVertexAttribArray(hPosition);
    glVertexAttribPointer(hPosition,
                          3,                       // X,Y,Z coordinates
                          GL_FLOAT,
                          GL_FALSE,
                          0,                       // 0 means tightly packed
                          m_pClientPosition);

    // If a texcoord array has been set, assign it.
    glEnableVertexAttribArray(hTexCoord);
    if (m_pClientTexCoord != 0)
    {
        glVertexAttribPointer(hTexCoord,
                              2,                   // U,V coordinates
                              GL_FLOAT,
                              GL_FALSE,
                              0,
                              m_pClientTexCoord);
    }

    // If a normal array has been set, assign it.
    glEnableVertexAttribArray(hNormal);
    if (m_pClientNormal != 0)
    {
        glVertexAttribPointer(hNormal,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              0,
                              m_pClientNormal);
    }
}
