#include "OrientedBoxCollider.h"
#include "BoxCollider.h"
#include "MeshCollider.h"
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

void OrientedBoxCollider::Render(Matrix* pMVP)
{
    int hProg = GetShaderProgram(STATIC_FULLBRIGHT_PROGRAM);
    int hPosition    = -1;
    int hTextureMode = -1;
    int hColor       = -1;
    int hMatrixMVP   = -1;

    Matrix matFinal;

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

    // Create the final MVP matrix by multiplying the colliders
    // rotation matrix
    matFinal = (*pMVP) * m_matRotation;

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

    glUniformMatrix4fv(hMatrixMVP, 1, GL_FALSE, matFinal.GetArray());

    // No texturing enabled.
    glUniform1i(hTextureMode, 0);

    glUniform4fv(hColor, 1, m_arRenderColor);

    glDrawElements(GL_TRIANGLES, 6 * 2 * 3, GL_UNSIGNED_BYTE, arBoxIndices);
}

int OrientedBoxCollider::Overlaps(Collider* pOther,
                                  void* pOtherMatter,
                                  void* pThisMatter)
{
    int i = 0;

    static float arXVector[3] = {1.0f, 0.0f, 0.0f};
    static float arYVector[3] = {0.0f, 1.0f, 0.0f};
    static float arZVector[3] = {0.0f, 0.0f, 1.0f};
    
    // Temporary array to hold model space vertices of both OBB A and B
    float arBoxVertices[VERTICES_PER_OBB * 3] = {0.0f};

    // These arrays hold the world position coordinates for each 
    // vertex in OBB A and B
    float arAVerts[VERTICES_PER_OBB * 3];
    float arBVerts[VERTICES_PER_OBB * 3];

    // Pointers to the matters that are currently using these two
    // colliders. T stands for "this", O stands for "other"
    Matter* pTMatter = 0;
    Matter* pOMatter = 0;

    // The combined matrices for matrix A and matrix B. By combined, I mean
    // the local rotation matrix of the OBB multiplied with the owning
    // Matter's model matrix.
    Matrix matA;
    Matrix matB;

    // Vectors for the primary axes in OBB A/B.
    // 3 vectors of size 3 each
    float arAxesA[3 * 3] = {0.0f};
    float arAxesB[3 * 3] = {0.0f};

    // The combined axes from each A axis cross with each B axis
    float arAxesC[3 * 9] = {0.0f};
    

    if (pOther->GetType() == COLLIDER_ORIENTED_BOX)
    {
        LogDebug("Performing SAT with two OBBs");
    }
    else
    {
        OrientedBoxCollider* pA = this;
        OrientedBoxCollider* pB = reinterpret_cast<OrientedBoxCollider*>(pOther);
        pTMatter = reinterpret_cast<Matter*>(pThisMatter);
        pOMatter = reinterpret_cast<Matter*>(pOtherMatter);

        // Compute the combined matrices for OBB A + B
        matA = *pTMatter->GetModelMatrix() * m_matRotation;
        matB = *pOMatter->GetModelMatrix() * pB->m_matRotation;

        // Find all the coordinates of box A (this box) in world space.
        GenerateLocalCoordinates(arBoxVertices);
        for(i = 0; i < VERTICES_PER_OBB; i++)
        {
            matA.MultiplyVec3(&arBoxVertices[i*3], arAVerts);
        }

        // Now find the second box's coordinates in world space.
        pB->GenerateLocalCoordinates(arBoxVertices);
        for(i = 0; i < VERTICES_PER_OBB; i++)
        {
            matB.MultiplyVec3(&arBoxVertices[i*3], arBVerts);
        }

        // Find the primary axes in world space for A and B
        matA.MultiplyVec3(arXVector,arAxesA);
        matA.MultiplyVec3(arYVector,&arAxesA[3]);
        matA.MultiplyVec3(arZVector,&arAxesA[6]);

        matB.MultiplyVec3(arXVector,arAxesB);
        matB.MultiplyVec3(arYVector,&arAxesB[3]);
        matB.MultiplyVec3(arZVector,&arAxesB[6]);

        // Find the combined axes of each primary A axis
        // crossed with each primary B axis. (9 total)
        CrossProduct(&arAxesA[0*3], &arAxesB[0*3], &arAxesC[0*3]);
        CrossProduct(&arAxesA[0*3], &arAxesB[1*3], &arAxesC[1*3]);
        CrossProduct(&arAxesA[0*3], &arAxesB[2*3], &arAxesC[2*3]);

        CrossProduct(&arAxesA[1*3], &arAxesB[0*3], &arAxesC[3*3]);
        CrossProduct(&arAxesA[1*3], &arAxesB[1*3], &arAxesC[4*3]);
        CrossProduct(&arAxesA[1*3], &arAxesB[2*3], &arAxesC[5*3]);

        CrossProduct(&arAxesA[2*3], &arAxesB[0*3], &arAxesC[6*3]);
        CrossProduct(&arAxesA[2*3], &arAxesB[1*3], &arAxesC[7*3]);
        CrossProduct(&arAxesA[2*3], &arAxesB[2*3], &arAxesC[8*3]);

        // Now that all axes are found, go through each one and project
        // the world space vertices of A and B onto the normal and find the ranges.
        // CheckIntervalOverlap will return 1 if there is an overlap of A's 
        // vertex projections and B's vertex projections. If 0 is returned, then we
        // have found an axis that separates the OBBs and thus, they do not
        // overlap.
        for (i = 0; i < 3; i++)
        {
            if (CheckIntervalOverlap(&arAxesA[i*3], arAVerts, arBVerts) == 0)
            {
                return 0;
            }

            if (CheckIntervalOverlap(&arAxesB[i*3], arAVerts, arBVerts) == 0)
            {
                return 0;
            }
        }

        for (i = 0; i < 9; i++)
        {
            if (CheckIntervalOverlap(&arAxesC[i*3], arAVerts, arBVerts) == 0)
            {
                return 0;
            }
        }
    }

    return 1;
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

    for (i = 0; i < VERTICES_PER_OBB; i++)
    {
        pRes[i*3 + 0] = m_arPosition[0] + (1 - (i & 1)*2)*m_arHalfExtents[0];
        pRes[i*3 + 1] = m_arPosition[1] + (1 - ((i & 2) >> 1) *2)*m_arHalfExtents[1];
        pRes[i*3 + 2] = m_arPosition[2] + (1 - ((i & 4) >> 2) *2)*m_arHalfExtents[2];
    }
}

int OrientedBoxCollider::CheckIntervalOverlap(float* arAxis, float* arVertsA, float* arVertsB)
{
    int i       = 0;
    float fDotA = 0.0f;
    float fDotB = 0.0f;

    // Set initial mins and maxes
    float fMinA = DotProduct(arAxis, arVertsA);
    float fMaxA = fMinA;

    float fMinB = DotProduct(arAxis, arVertsB);
    float fMaxB = fMinB;

    for (i = 1; i < VERTICES_PER_OBB; i++)
    {
        fDotA = DotProduct(arAxis, &arVertsA[i*3]);
        fDotB = DotProduct(arAxis, &arVertsB[i*3]);

        if (fDotA < fMinA)
        {
            fMinA = fDotA;
        }

        if (fDotA > fMaxA)
        {
            fMaxA = fDotA;
        }

        if (fDotB < fMinB)
        {
            fMinB = fDotB;
        }

        if (fDotB > fMaxB)
        {
            fMaxB = fDotB;
        }
    }

    // Check interval overlaps
    if (fMinB > fMaxA)
    {
        return 0;
    }

    if (fMaxB < fMinA)
    {
        return 0;
    }

    // The intervals didnt overlap, so return 1.
    return 1;
}