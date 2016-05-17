#include "BoxCollider.h"
#include "TriangleBoxCollision.h"
#include "OrientedBoxCollider.h"
#include "Matter.h"
#include "Scene.h"
#include "VGL.h"
#include "VMath.h"

//*****************************************************************************
// Constructor
//*****************************************************************************
BoxCollider::BoxCollider()
{
    m_nType = COLLIDER_BOX;
    m_arHalfExtents[0] = 1.0f;
    m_arHalfExtents[1] = 1.0f;
    m_arHalfExtents[2] = 1.0f;
}

//*****************************************************************************
// Destructor
//*****************************************************************************
BoxCollider::~BoxCollider()
{

}

//*****************************************************************************
// SetHalfExtents
//*****************************************************************************
void BoxCollider::SetHalfExtents(float fHalfX,
                                 float fHalfY,
                                 float fHalfZ)
{
    m_arHalfExtents[0] = fHalfX;
    m_arHalfExtents[1] = fHalfY;
    m_arHalfExtents[2] = fHalfZ;
}

//*****************************************************************************
// GetHalfExtents
//*****************************************************************************
const float* BoxCollider::GetHalfExtents()
{
    return m_arHalfExtents;
}

//*****************************************************************************
// Render
//*****************************************************************************
void BoxCollider::Render(void* pMatter,
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

    Scene* pTScene = reinterpret_cast<Scene*>(pScene);
    Matter* pTMatter = reinterpret_cast<Matter*>(pMatter);
    const float* pMatPosition = pTMatter->GetPosition();
    const float* pMatScale = pTMatter->GetScale();

    // Calculate the VP matrix
    Matrix matVP = *(pTScene->GetCamera()->GetProjectionMatrix()) * (*pTScene->GetCamera()->GetViewMatrix());

    // First generate the point coordinates and indices
    float arBoxVertices[VERTICES_PER_BOX * 3] = {0.0f};
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

    // Find the world coordinates by using the Matter's position and scale
    for (i = 0; i < VERTICES_PER_BOX; i++)
    {
        arBoxVertices[i*3 + 0] = arBoxVertices[i*3 + 0]*pMatScale[0] + pMatPosition[0];
        arBoxVertices[i*3 + 1] = arBoxVertices[i*3 + 1]*pMatScale[1] + pMatPosition[1];
        arBoxVertices[i*3 + 2] = arBoxVertices[i*3 + 2]*pMatScale[2] + pMatPosition[2];
    }

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

    glUniformMatrix4fv(hMatrixMVP, 1, GL_FALSE, matVP.GetArray());

    // No texturing enabled.
    glUniform1i(hTextureMode, 0);

    glUniform4fv(hColor, 1, m_arRenderColor);

    glDrawElements(GL_TRIANGLES, 6 * 2 * 3, GL_UNSIGNED_BYTE, arBoxIndices);
}


//*****************************************************************************
// Overlaps
//*****************************************************************************
OverlapResult BoxCollider::Overlaps(Collider* pOther,
                                    void* pOtherMatter,
                                    void* pThisMatter)
{
    if (pOther->GetType() == COLLIDER_ORIENTED_BOX)
    {
        return AABB_Overlaps_OBB(this, pThisMatter, pOther, pOtherMatter);
    }
    else if (pOther->GetType() == COLLIDER_BOX)
    {
        return AABB_Overlaps_AABB(this, pThisMatter, pOther, pOtherMatter);
    }

    return OverlapResult();
}

void BoxCollider::GenerateLocalCoordinates(float* pRes)
{
    int i = 0;

    for (i = 0; i < VERTICES_PER_BOX; i++)
    {
        pRes[i*3 + 0] = m_arPosition[0] + (1 - (i & 1)*2)*m_arHalfExtents[0];
        pRes[i*3 + 1] = m_arPosition[1] + (1 - ((i & 2) >> 1) *2)*m_arHalfExtents[1];
        pRes[i*3 + 2] = m_arPosition[2] + (1 - ((i & 4) >> 2) *2)*m_arHalfExtents[2];
    }
}

void BoxCollider::GetBounds(void* pMatter, float* arMin, float* arMax)
{
    Matter* pTMatter = reinterpret_cast<Matter*>(pMatter);
    const float* pMatPosition = pTMatter->GetPosition();
    const float* pMatScale = pTMatter->GetScale();

    arMin[0] = (m_arPosition[0] - m_arHalfExtents[0])*pMatScale[0] + pMatPosition[0];
    arMin[1] = (m_arPosition[1] - m_arHalfExtents[1])*pMatScale[1] + pMatPosition[1];
    arMin[2] = (m_arPosition[2] - m_arHalfExtents[2])*pMatScale[2] + pMatPosition[2];

    arMax[0] = (m_arPosition[0] + m_arHalfExtents[0])*pMatScale[0] + pMatPosition[0];
    arMax[1] = (m_arPosition[1] + m_arHalfExtents[1])*pMatScale[1] + pMatPosition[1];
    arMax[2] = (m_arPosition[2] + m_arHalfExtents[2])*pMatScale[2] + pMatPosition[2];
}
