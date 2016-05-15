#include "OrientedBoxCollider.h"
#include "BoxCollider.h"
#include "Matter.h"
#include "Log.h"
#include "VMath.h"
#include "VGL.h"

OrientedBoxCollider::OrientedBoxCollider()
{
    m_nType = COLLIDER_ORIENTED_BOX;
    m_arHalfExtents[0] = 1.0f;
    m_arHalfExtents[1] = 1.0f;
    m_arHalfExtents[2] = 1.0f;
    m_arRotation[0] = 0.0f;
    m_arRotation[1] = 0.0f;
    m_arRotation[2] = 0.0f;
}

OrientedBoxCollider::~OrientedBoxCollider()
{

}

void OrientedBoxCollider::Render(void* pMatter,
                                 void* pScene,
                                 Matrix* pMVP)
{
    int i = 0;
    int j = 0;
    float arTemp[3] = {0.0f, 0.0f, 0.0f};

    int hProg = GetShaderProgram(STATIC_FULLBRIGHT_PROGRAM);
    int hPosition    = -1;
    int hTextureMode = -1;
    int hColor       = -1;
    int hMatrixMVP   = -1;

    // First generate the point coordinates and indices
    float arBoxVertices[VERTICES_PER_OBB * 3] = {0.0f};
    static unsigned char arBoxIndices[6 * 2 * 3] = {0, 1, 2,
                                                    1, 2, 3,
                                                    4, 5, 6,
                                                    5, 6, 7,
                                                    0, 4, 2,
                                                    4, 2, 6,
                                                    5, 1, 7,
                                                    1, 7, 3,
                                                    0, 1, 5,
                                                    0, 4, 5,
                                                    2, 7, 3,
                                                    2, 6, 7};

    GenerateLocalCoordinates(arBoxVertices);

    glUseProgram(hProg);

    // Unbind VBO and Texture
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
                          arBoxVertices);

    glUniformMatrix4fv(hMatrixMVP, 1, GL_FALSE, pMVP->GetArray());

    // No texturing enabled.
    glUniform1i(hTextureMode, 0);

    glUniform4fv(hColor, 1, m_arRenderColor);

    glDrawElements(GL_TRIANGLES, 6 * 2 * 3, GL_UNSIGNED_BYTE, arBoxIndices);
}

OverlapResult OrientedBoxCollider::Overlaps(Collider* pOther,
                                            void* pOtherMatter,
                                            void* pThisMatter)
{
    if (pOther->GetType() == COLLIDER_ORIENTED_BOX)
    {
        return OBB_Overlaps_OBB(this, pThisMatter, pOther, pOtherMatter);
    }
    else if (pOther->GetType() == COLLIDER_BOX)
    {
        return OBB_Overlaps_AABB(this, pThisMatter, pOther, pOtherMatter);
    }

    return OverlapResult();
}

void OrientedBoxCollider::SetHalfExtents(float fHalfX,
                                         float fHalfY,
                                         float fHalfZ)
{
    m_arHalfExtents[0] = fHalfX;
    m_arHalfExtents[1] = fHalfY;
    m_arHalfExtents[2] = fHalfZ;
}

void OrientedBoxCollider::SetRelativeRotation(float fRotX,
                                              float fRotY,
                                              float fRotZ)
{
    m_arRotation[0] = fRotX;
    m_arRotation[1] = fRotY;
    m_arRotation[2] = fRotZ;
    m_matRotation.Rotate(fRotX, 1.0f, 0.0f, 0.0f);
    m_matRotation.Rotate(fRotY, 0.0f, 1.0f, 0.0f);
    m_matRotation.Rotate(fRotZ, 0.0f, 0.0f, 1.0f);
}

void OrientedBoxCollider::GenerateLocalCoordinates(float* pRes)
{
    int i = 0;
    float arTemp[3] = {0.0f, 0.0f, 0.0f};

    for (i = 0; i < VERTICES_PER_OBB; i++)
    {
        pRes[i*3 + 0] = (1 - (i & 1)*2)*m_arHalfExtents[0];
        pRes[i*3 + 1] = (1 - ((i & 2) >> 1) *2)*m_arHalfExtents[1];
        pRes[i*3 + 2] = (1 - ((i & 4) >> 2) *2)*m_arHalfExtents[2];

        // Rotate around OBB center
        m_matRotation.MultiplyVec3(&pRes[i*3], arTemp);

        // Add relative position offset
        pRes[i*3 + 0] = arTemp[0] + m_arPosition[0];
        pRes[i*3 + 1] = arTemp[1] + m_arPosition[1];
        pRes[i*3 + 2] = arTemp[2] + m_arPosition[2];
    }
}

const Matrix* OrientedBoxCollider::GetRotationMatrix()
{
    return &m_matRotation;
}
