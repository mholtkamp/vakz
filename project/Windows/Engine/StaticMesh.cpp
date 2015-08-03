#include "StaticMesh.h"
#include "VGL.h"
#include "MeshLoader.h"

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

    m_arPosition = 0;
}

//*****************************************************************************
// Destructor
//*****************************************************************************
StaticMesh::~StaticMesh()
{
    if (m_arPosition != 0)
    {
        delete [] m_arPosition;
        m_arPosition = 0;
    }
}

//*****************************************************************************
// Load
//*****************************************************************************
int StaticMesh::Load(const char* pFile)
{
    unsigned int unFaces = 0;
    m_unVBO = MeshLoader::LoadOBJ(pFile, unFaces);

    // Every face has 3 vertices.
    m_nVertexCount = unFaces * 3;

    return 1;
}

//*****************************************************************************
// LoadGeometry
//*****************************************************************************
void StaticMesh::LoadGeometry(const char* pFile)
{
    unsigned int unFaces = 0;
    m_arPosition = MeshLoader::LoadOBJGeometry(pFile, unFaces);

    // Every face has 3 vertices.
    m_nVertexCount = unFaces * 3;
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
// GetPositionArray
//*****************************************************************************
float* StaticMesh::GetPositionArray()
{
    return m_arPosition;
}

//*****************************************************************************
// SetRenderState
//*****************************************************************************
void StaticMesh::SetRenderState(void*        pScene,
                                unsigned int hProg,
                                int          nAnimationIndex,
                                float        fCurrentFrame)
{
    // Assign vertex attributes
    int hPosition = glGetAttribLocation(hProg, "aPosition");
    int hTexCoord = glGetAttribLocation(hProg, "aTexCoord");
    int hNormal   = glGetAttribLocation(hProg, "aNormal");

    // If the mesh is using a VBO, use it when setting attributes.
    if (m_unVBO != 0)
    {
        // All static meshes will have a position array.
        glEnableVertexAttribArray(hPosition);
        glEnableVertexAttribArray(hTexCoord);
        glEnableVertexAttribArray(hNormal);

        glBindBuffer(GL_ARRAY_BUFFER, m_unVBO);
        glVertexAttribPointer(hPosition,
                              3,                     // X/Y/Z
                              GL_FLOAT,
                              GL_FALSE,
                              8 * sizeof(float),     // 8 floats per vertex.
                              0);                    // First value is at start of vbo.

        glVertexAttribPointer(hTexCoord,
                              2,                     // U/V
                              GL_FLOAT,
                              GL_FALSE,
                              8 * sizeof(float),
                              (void*) (3 * sizeof(float)));  // First texcoord is after position

        glVertexAttribPointer(hNormal,
                              3,                     // X/Y/Z
                              GL_FLOAT,
                              GL_FALSE,
                              8 * sizeof(float),     // 8 floats per vertex.
                              (void*) (5 * sizeof(float)));  // First value is after position + texcoord.
    }
    else
    {
        // Unbind any VBO, use client side arrays
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // All static meshes will have a position array.
        glEnableVertexAttribArray(hPosition);
        glVertexAttribPointer(hPosition,
                              3,                       // X,Y,Z coordinates
                              GL_FLOAT,
                              GL_FALSE,
                              0,                       // 0 means tightly packed
                              m_pClientPosition);

        // If a texcoord array has been set, assign it.
        if (m_pClientTexCoord != 0)
        {
            glEnableVertexAttribArray(hTexCoord);
            glVertexAttribPointer(hTexCoord,
                                  2,                   // U,V coordinates
                                  GL_FLOAT,
                                  GL_FALSE,
                                  0,
                                  m_pClientTexCoord);
        }

        // If a normal array has been set, assign it.
        if (m_pClientNormal != 0)
        {
            glEnableVertexAttribArray(hNormal);
            glVertexAttribPointer(hNormal,
                                  3,
                                  GL_FLOAT,
                                  GL_FALSE,
                                  0,
                                  m_pClientNormal);
        }
    }
}
