#include "MeshCollider.h"
#include "MeshLoader.h"
#include "VGL.h"

//*****************************************************************************
// Constructor
//*****************************************************************************
MeshCollider::MeshCollider()
{
    m_nType = COLLIDER_MESH;
    m_arPosition = 0;
    m_nVertices  = 0;
}

//*****************************************************************************
// Destructor
//*****************************************************************************
MeshCollider::~MeshCollider()
{
    if (m_arPosition != 0)
    {
        delete [] m_arPosition;
        m_arPosition = 0;
    }
}

//*****************************************************************************
// AssignMesh
//*****************************************************************************
void MeshCollider::AssignMesh(StaticMesh* pStaticMesh)
{
    m_arPosition = pStaticMesh->GetPositionArray();
    m_nVertices  = pStaticMesh->GetVertexCount();
}

//*****************************************************************************
// Render
//*****************************************************************************
void MeshCollider::Render(Matrix* pMVP)
{
    int hProg = GetShaderProgram(STATIC_FULLBRIGHT_PROGRAM);
    int hPosition    = -1;
    int hTextureMode = -1;
    int hColor       = -1;
    int hMatrixMVP   = -1;

    glUseProgram(hProg);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    hPosition    = glGetAttribLocation(hProg, "aPosition");
    hTextureMode = glGetUniformLocation(hProg, "uTextureMode");
    hColor       = glGetUniformLocation(hProg, "uColor");
    hMatrixMVP   = glGetUniformLocation(hProg, "uMatrixMVP");

    glEnableVertexAttribArray(hPosition);
    glVertexAttribPointer(hPosition,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          m_arPosition);

    glUniformMatrix4fv(hMatrixMVP, 1, GL_FALSE, pMVP->GetArray());

    glUniform1i(hTextureMode, 0);
    glUniform4fv(hColor, 1, m_arRenderColor);

    glDrawArrays(GL_TRIANGLES, 0, m_nVertices);
}

//*****************************************************************************
// Overlaps
//*****************************************************************************
int MeshCollider::Overlaps(Collider* pOther)
{
    return 0;
}

//*****************************************************************************
// GetVertexCount
//*****************************************************************************
int MeshCollider::GetVertexCount()
{
    return m_nVertices;
}

//*****************************************************************************
// GetPositionArray
//*****************************************************************************
float* MeshCollider::GetPositionArray()
{
    return m_arPosition;
}

//*****************************************************************************
// GetTriangle
//*****************************************************************************
void MeshCollider::GetTriangle(int   nIndex,
                               float arTriangle[3][3])
{
    // Vertex 1
    arTriangle[0][0] = m_arPosition[nIndex*9 + 0];
    arTriangle[0][1] = m_arPosition[nIndex*9 + 1];
    arTriangle[0][2] = m_arPosition[nIndex*9 + 2];
    
    // Vertex 2
    arTriangle[1][0] = m_arPosition[nIndex*9 + 3];
    arTriangle[1][1] = m_arPosition[nIndex*9 + 4];
    arTriangle[1][2] = m_arPosition[nIndex*9 + 5];

    // Vertex 3
    arTriangle[2][0] = m_arPosition[nIndex*9 + 6];
    arTriangle[2][1] = m_arPosition[nIndex*9 + 7];
    arTriangle[2][2] = m_arPosition[nIndex*9 + 8];
}
