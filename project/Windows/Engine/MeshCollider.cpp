#include "MeshCollider.h"
#include "MeshLoader.h"
#include "VGL.h"

MeshCollider::MeshCollider()
{
    m_arPosition = 0;
    m_nVertices  = 0;
}

MeshCollider::~MeshCollider()
{
    if (m_arPosition != 0)
    {
        delete [] m_arPosition;
        m_arPosition = 0;
    }
}

void MeshCollider::AssignMesh(StaticMesh* pStaticMesh)
{
    m_arPosition = pStaticMesh->GetPositionArray();
    m_nVertices  = pStaticMesh->GetVertexCount();
}

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

int MeshCollider::Overlaps(Collider* pOther)
{
    return 0;
}
