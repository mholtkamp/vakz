#include "Collider.h"
#include "BoxCollider.h"
#include "OrientedBoxCollider.h"

#include "Matter.h"
#include "VMath.h"

#define DEFAULT_COLLIDER_COLOR_R 0.2f
#define DEFAULT_COLLIDER_COLOR_G 0.8f
#define DEFAULT_COLLIDER_COLOR_B 0.25f
#define DEFAULT_COLLIDER_COLOR_A 0.3f

//*****************************************************************************
// Constructor
//*****************************************************************************
Collider::Collider()
{
    m_nType = COLLIDER_NO_TYPE;

    m_arPosition[0] = 0.0f;
    m_arPosition[1] = 0.0f;
    m_arPosition[2] = 0.0f;

    m_arRenderColor[0] = DEFAULT_COLLIDER_COLOR_R;
    m_arRenderColor[1] = DEFAULT_COLLIDER_COLOR_G;
    m_arRenderColor[2] = DEFAULT_COLLIDER_COLOR_B;
    m_arRenderColor[3] = DEFAULT_COLLIDER_COLOR_A;
}

//*****************************************************************************
// Destructor
//*****************************************************************************
Collider::~Collider()
{

}

//*****************************************************************************
// GetType
//*****************************************************************************
int Collider::GetType()
{
    return m_nType;
}

//*****************************************************************************
// GetRelativePosition
//*****************************************************************************
const float* Collider::GetRelativePosition()
{
    return m_arPosition;
}

//*****************************************************************************
// SetPosition
//*****************************************************************************
void Collider::SetRelativePosition(float fX,
                                   float fY,
                                   float fZ)
{
    m_arPosition[0] = fX;
    m_arPosition[1] = fY;
    m_arPosition[2] = fZ;
}

//*****************************************************************************
// SetColor
//*****************************************************************************
void Collider::SetColor(float fRed,
                        float fGreen,
                        float fBlue,
                        float fAlpha)
{
    m_arRenderColor[0] = fRed;
    m_arRenderColor[1] = fGreen;
    m_arRenderColor[2] = fBlue;
    m_arRenderColor[3] = fAlpha;
}

OverlapResult Collider::OBB_Overlaps_OBB(Collider* pColA,
                                         void* pColMatterA,
                                         Collider* pColB,
                                         void* pColMatterB)
{
    // Returning result by value, so it is okay
    // to use this stack object.
    OverlapResult orResult;

    int i = 0;
    float arTemp[3] = {0.0f, 0.0f, 0.0f};

    static float arXVector[3] = {1.0f, 0.0f, 0.0f};
    static float arYVector[3] = {0.0f, 1.0f, 0.0f};
    static float arZVector[3] = {0.0f, 0.0f, 1.0f};
    
    // Temporary array to hold model space vertices of both OBB A and B
    float arBoxVertices[VERTICES_PER_BOX * 3] = {0.0f};

    // These arrays hold the world position coordinates for each 
    // vertex in OBB A and B
    float arAVerts[VERTICES_PER_BOX * 3];
    float arBVerts[VERTICES_PER_BOX * 3];
    float arACenter[3];
    float arBCenter[3];

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
    
    OrientedBoxCollider* pA = reinterpret_cast<OrientedBoxCollider*>(pColA);
    OrientedBoxCollider* pB = reinterpret_cast<OrientedBoxCollider*>(pColB);
    pTMatter = reinterpret_cast<Matter*>(pColMatterA);
    pOMatter = reinterpret_cast<Matter*>(pColMatterB);

    // Compute the combined matrices for OBB A + B
    matA = *pTMatter->GetModelMatrix() * *pA->GetRotationMatrix();
    matB = *pOMatter->GetModelMatrix() * *pB->GetRotationMatrix();

    // Find all the coordinates of box A (this box) in world space.
    pA->GenerateLocalCoordinates(arBoxVertices);

    // Transform so they are in world space now by multiplying matter's
    // model matrix.
    Matrix* pMatModelT = pTMatter->GetModelMatrix();
    for(i = 0; i < VERTICES_PER_BOX; i++)
    {
        // Now multiply by the model matrix
        pMatModelT->MultiplyVec3(&arBoxVertices[i*3], &arAVerts[i*3]);
    }
    pMatModelT->MultiplyVec3(pA->m_arPosition, arACenter);

    // Now find the second box's coordinates in world space.
    pB->GenerateLocalCoordinates(arBoxVertices);

    // Transform so they are in world space now by multiplying matter's
    // model matrix.
    Matrix* pMatModelO = pOMatter->GetModelMatrix();
    for(i = 0; i < VERTICES_PER_BOX; i++)
    {
        pMatModelO->MultiplyVec3(&arBoxVertices[i*3], &arBVerts[i*3]);
    }
    pMatModelO->MultiplyVec3(pB->m_arPosition, arBCenter);

    // Find the primary axes in world space for A and B
    matA.MultiplyVec3Dir(arXVector,arAxesA);
    matA.MultiplyVec3Dir(arYVector,&arAxesA[3]);
    matA.MultiplyVec3Dir(arZVector,&arAxesA[6]);

    matB.MultiplyVec3Dir(arXVector,arAxesB);
    matB.MultiplyVec3Dir(arYVector,&arAxesB[3]);
    matB.MultiplyVec3Dir(arZVector,&arAxesB[6]);

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
        if (CheckIntervalOverlap(&arAxesA[i*3], arAVerts, arBVerts, orResult) == 0)
        {
            orResult.m_nOverlapping = 0;
            return orResult;
        }

        if (CheckIntervalOverlap(&arAxesB[i*3], arAVerts, arBVerts, orResult) == 0)
        {
            orResult.m_nOverlapping = 0;
            return orResult;
        }
    }

    for (i = 0; i < 9; i++)
    {
        if (CheckIntervalOverlap(&arAxesC[i*3], arAVerts, arBVerts, orResult) == 0)
        {
            orResult.m_nOverlapping = 0;
            return orResult;
        }
    }

    // No axis separates, so boxes are overlapping
    orResult.m_nOverlapping = 1;

    // Check if axis of least pen should be inverted.
    float arCenterDisp[3] = {0.0f};
    arCenterDisp[0] = arBCenter[0] - arACenter[0];
    arCenterDisp[1] = arBCenter[1] - arACenter[1];
    arCenterDisp[2] = arBCenter[2] - arACenter[2];

    if (AngleBetweenVectors(arCenterDisp, orResult.m_arLeastPenAxis) < 90.0f)
    {
        // Vectors point in a similar direction, invert the axis
        orResult.m_arLeastPenAxis[0] *= -1.0f;
        orResult.m_arLeastPenAxis[1] *= -1.0f;
        orResult.m_arLeastPenAxis[2] *= -1.0f;
    }

    return orResult;
}

OverlapResult Collider::AABB_Overlaps_OBB(Collider* pColAABB,
                                          void* pColMatterAABB,
                                          Collider* pColOBB,
                                          void* pColMatterOBB)
{
    // Returning result by value, so it is okay
    // to use this stack object.
    OverlapResult orResult;

    int i = 0;
    float arTemp[3] = {0.0f, 0.0f, 0.0f};

    static float arXVector[3] = {1.0f, 0.0f, 0.0f};
    static float arYVector[3] = {0.0f, 1.0f, 0.0f};
    static float arZVector[3] = {0.0f, 0.0f, 1.0f};
    
    // Temporary array to hold model space vertices of both OBB A and B
    float arBoxVertices[VERTICES_PER_BOX * 3] = {0.0f};

    // These arrays hold the world position coordinates for each 
    // vertex in OBB A and B
    float arAABBVerts[VERTICES_PER_BOX * 3];
    float arOBBVerts[VERTICES_PER_BOX * 3];
    float arAABBCenter[3];
    float arOBBCenter[3];

    // Pointers to the matters for each collider
    Matter* pAABBMatter = reinterpret_cast<Matter*>(pColMatterAABB);
    Matter* pOBBMatter = reinterpret_cast<Matter*>(pColMatterOBB);

    // The combined matrices for matrix A and matrix B. By combined, I mean
    // the local rotation matrix of the OBB multiplied with the owning
    // Matter's model matrix.
    Matrix matAABB;
    Matrix matOBB;

    // Vectors for the primary axes in OBB A/B.
    // 3 vectors of size 3 each
    float arAxesAABB[3 * 3] = {0.0f};
    float arAxesOBB[3 * 3] = {0.0f};

    // The combined axes from each A axis cross with each B axis
    float arAxesC[3 * 9] = {0.0f};
    
    BoxCollider* pAABB = reinterpret_cast<BoxCollider*>(pColAABB);
    OrientedBoxCollider* pOBB = reinterpret_cast<OrientedBoxCollider*>(pColOBB);

    // Compute the combined matrices for OBB A + B
    matOBB = *pOBBMatter->GetModelMatrix() * *pOBB->GetRotationMatrix();

    // Find all the coordinates of box A (this box) in world space.
    pAABB->GenerateLocalCoordinates(arBoxVertices);

    const float* pAABBMatPosition = pAABBMatter->GetPosition();
    const float* pAABBMatScale = pAABBMatter->GetScale();

    // Find the world coordinates by using the Matter's position and scale
    for (i = 0; i < VERTICES_PER_BOX; i++)
    {
        arAABBVerts[i*3 + 0] = arBoxVertices[i*3 + 0] * pAABBMatScale[0] + pAABBMatPosition[0];
        arAABBVerts[i*3 + 1] = arBoxVertices[i*3 + 1] * pAABBMatScale[1] + pAABBMatPosition[1];
        arAABBVerts[i*3 + 2] = arBoxVertices[i*3 + 2] * pAABBMatScale[2] + pAABBMatPosition[2];
    }
    const float* pAABBRelativePos = pAABB->GetRelativePosition();
    arAABBCenter[0] = pAABBRelativePos[0] * pAABBMatScale[0] + pAABBMatPosition[0];
    arAABBCenter[1] = pAABBRelativePos[1] * pAABBMatScale[1] + pAABBMatPosition[1];
    arAABBCenter[2] = pAABBRelativePos[2] * pAABBMatScale[2] + pAABBMatPosition[2];

    // Now find the second box's coordinates in world space.
    pOBB->GenerateLocalCoordinates(arBoxVertices);

    // Transform so they are in world space now by multiplying matter's
    // model matrix.
    Matrix* pMatModelOBB = pOBBMatter->GetModelMatrix();
    for(i = 0; i < VERTICES_PER_BOX; i++)
    {
        pMatModelOBB->MultiplyVec3(&arBoxVertices[i*3], &arOBBVerts[i*3]);
    }
    pMatModelOBB->MultiplyVec3(pOBB->GetRelativePosition(), arOBBCenter);

    // Find the primary axes in world space for A and B
    arAxesAABB[0] = arXVector[0];
    arAxesAABB[1] = arXVector[1];
    arAxesAABB[2] = arXVector[2];

    arAxesAABB[3] = arYVector[0];
    arAxesAABB[4] = arYVector[1];
    arAxesAABB[5] = arYVector[2];

    arAxesAABB[6] = arZVector[0];
    arAxesAABB[7] = arZVector[1];
    arAxesAABB[8] = arZVector[2];

    matOBB.MultiplyVec3Dir(arXVector,arAxesOBB);
    matOBB.MultiplyVec3Dir(arYVector,&arAxesOBB[3]);
    matOBB.MultiplyVec3Dir(arZVector,&arAxesOBB[6]);

    // Find the combined axes of each primary A axis
    // crossed with each primary B axis. (9 total)
    CrossProduct(&arAxesAABB[0*3], &arAxesOBB[0*3], &arAxesC[0*3]);
    CrossProduct(&arAxesAABB[0*3], &arAxesOBB[1*3], &arAxesC[1*3]);
    CrossProduct(&arAxesAABB[0*3], &arAxesOBB[2*3], &arAxesC[2*3]);

    CrossProduct(&arAxesAABB[1*3], &arAxesOBB[0*3], &arAxesC[3*3]);
    CrossProduct(&arAxesAABB[1*3], &arAxesOBB[1*3], &arAxesC[4*3]);
    CrossProduct(&arAxesAABB[1*3], &arAxesOBB[2*3], &arAxesC[5*3]);

    CrossProduct(&arAxesAABB[2*3], &arAxesOBB[0*3], &arAxesC[6*3]);
    CrossProduct(&arAxesAABB[2*3], &arAxesOBB[1*3], &arAxesC[7*3]);
    CrossProduct(&arAxesAABB[2*3], &arAxesOBB[2*3], &arAxesC[8*3]);

    // Now that all axes are found, go through each one and project
    // the world space vertices of A and B onto the normal and find the ranges.
    // CheckIntervalOverlap will return 1 if there is an overlap of A's 
    // vertex projections and B's vertex projections. If 0 is returned, then we
    // have found an axis that separates the OBBs and thus, they do not
    // overlap.
    for (i = 0; i < 3; i++)
    {
        if (CheckIntervalOverlap(&arAxesAABB[i*3], arAABBVerts, arOBBVerts, orResult) == 0)
        {
            orResult.m_nOverlapping = 0;
            return orResult;
        }

        if (CheckIntervalOverlap(&arAxesOBB[i*3], arAABBVerts, arOBBVerts, orResult) == 0)
        {
            orResult.m_nOverlapping = 0;
            return orResult;
        }
    }

    for (i = 0; i < 9; i++)
    {
        if (CheckIntervalOverlap(&arAxesC[i*3], arAABBVerts, arOBBVerts, orResult) == 0)
        {
            orResult.m_nOverlapping = 0;
            return orResult;
        }
    }

    // No axis separates, so boxes are overlapping
    orResult.m_nOverlapping = 1;

    // Check if axis of least pen should be inverted.
    float arCenterDisp[3] = {0.0f};
    arCenterDisp[0] = arOBBCenter[0] - arAABBCenter[0];
    arCenterDisp[1] = arOBBCenter[1] - arAABBCenter[1];
    arCenterDisp[2] = arOBBCenter[2] - arAABBCenter[2];

    if (AngleBetweenVectors(arCenterDisp, orResult.m_arLeastPenAxis) < 90.0f)
    {
        // Vectors point in a similar direction, invert the axis
        orResult.m_arLeastPenAxis[0] *= -1.0f;
        orResult.m_arLeastPenAxis[1] *= -1.0f;
        orResult.m_arLeastPenAxis[2] *= -1.0f;
    }

    return orResult;
}

OverlapResult Collider::OBB_Overlaps_AABB(Collider* pColOBB,
                                          void* pColMatterOBB,
                                          Collider* pColAABB,
                                          void* pColMatterAABB)
{
    OverlapResult orResult;

    // Call the converse method
    orResult = AABB_Overlaps_OBB(pColAABB, pColMatterAABB, pColOBB, pColMatterOBB);
    
    // Flip the returned axis because collider order was swapped
    orResult.m_arLeastPenAxis[0] *= -1.0f;
    orResult.m_arLeastPenAxis[1] *= -1.0f;
    orResult.m_arLeastPenAxis[2] *= -1.0f;

    return orResult;
}

OverlapResult Collider::AABB_Overlaps_AABB(Collider* pColA,
                                           void* pColMatterA,
                                           Collider* pColB,
                                           void* pColMatterB)
{
    // Returning result by value, so it is okay
    // to use this stack object.
    OverlapResult orResult;

    int i = 0;
    float arTemp[3] = {0.0f, 0.0f, 0.0f};

    static float arXVector[3] = {1.0f, 0.0f, 0.0f};
    static float arYVector[3] = {0.0f, 1.0f, 0.0f};
    static float arZVector[3] = {0.0f, 0.0f, 1.0f};
    
    // Temporary array to hold model space vertices of both OBB A and B
    float arBoxVertices[VERTICES_PER_BOX * 3] = {0.0f};

    // These arrays hold the world position coordinates for each 
    // vertex in OBB A and B
    float arAVerts[VERTICES_PER_BOX * 3];
    float arBVerts[VERTICES_PER_BOX * 3];
    float arACenter[3];
    float arBCenter[3];

    // Pointers to the matters for each collider
    Matter* pAMatter = reinterpret_cast<Matter*>(pColMatterA);
    Matter* pBMatter = reinterpret_cast<Matter*>(pColMatterB);
    
    BoxCollider* pA = reinterpret_cast<BoxCollider*>(pColA);
    BoxCollider* pB = reinterpret_cast<BoxCollider*>(pColB);

    // Find all the coordinates of box A (this box) in world space.
    pA->GenerateLocalCoordinates(arBoxVertices);

    const float* pAMatPosition = pAMatter->GetPosition();
    const float* pAMatScale = pAMatter->GetScale();

    // Find the world coordinates by using the Matter's position and scale
    for (i = 0; i < VERTICES_PER_BOX; i++)
    {
        arAVerts[i*3 + 0] = arBoxVertices[i*3 + 0] * pAMatScale[0] + pAMatPosition[0];
        arAVerts[i*3 + 1] = arBoxVertices[i*3 + 1] * pAMatScale[1] + pAMatPosition[1];
        arAVerts[i*3 + 2] = arBoxVertices[i*3 + 2] * pAMatScale[2] + pAMatPosition[2];
    }
    const float* pARelativePos = pA->GetRelativePosition();
    arACenter[0] = pARelativePos[0] * pAMatScale[0] + pAMatPosition[0];
    arACenter[1] = pARelativePos[1] * pAMatScale[1] + pAMatPosition[1];
    arACenter[2] = pARelativePos[2] * pAMatScale[2] + pAMatPosition[2];

    // Now find the second box's coordinates in world space.
    pB->GenerateLocalCoordinates(arBoxVertices);

    const float* pBMatPosition = pBMatter->GetPosition();
    const float* pBMatScale = pBMatter->GetScale();

    // Find the world coordinates by using the Matter's position and scale
    for (i = 0; i < VERTICES_PER_BOX; i++)
    {
        arBVerts[i*3 + 0] = arBoxVertices[i*3 + 0] * pBMatScale[0] + pBMatPosition[0];
        arBVerts[i*3 + 1] = arBoxVertices[i*3 + 1] * pBMatScale[1] + pBMatPosition[1];
        arBVerts[i*3 + 2] = arBoxVertices[i*3 + 2] * pBMatScale[2] + pBMatPosition[2];
    }
    const float* pBRelativePos = pB->GetRelativePosition();
    arBCenter[0] = pBRelativePos[0] * pBMatScale[0] + pBMatPosition[0];
    arBCenter[1] = pBRelativePos[1] * pBMatScale[1] + pBMatPosition[1];
    arBCenter[2] = pBRelativePos[2] * pBMatScale[2] + pBMatPosition[2];

    // For checking AABB collisions, only the 3 primary axes need to be checked
    if (CheckIntervalOverlap(arXVector, arAVerts, arBVerts, orResult) == 0)
    {
        orResult.m_nOverlapping = 0;
        return orResult;
    }
    if (CheckIntervalOverlap(arYVector, arAVerts, arBVerts, orResult) == 0)
    {
        orResult.m_nOverlapping = 0;
        return orResult;
    }
    if (CheckIntervalOverlap(arZVector, arAVerts, arBVerts, orResult) == 0)
    {
        orResult.m_nOverlapping = 0;
        return orResult;
    }
  
    // No axis separates, so boxes are overlapping
    orResult.m_nOverlapping = 1;

    // Check if axis of least pen should be inverted.
    float arCenterDisp[3] = {0.0f};
    arCenterDisp[0] = arBCenter[0] - arACenter[0];
    arCenterDisp[1] = arBCenter[1] - arACenter[1];
    arCenterDisp[2] = arBCenter[2] - arACenter[2];

    if (AngleBetweenVectors(arCenterDisp, orResult.m_arLeastPenAxis) < 90.0f)
    {
        // Vectors point in a similar direction, invert the axis
        orResult.m_arLeastPenAxis[0] *= -1.0f;
        orResult.m_arLeastPenAxis[1] *= -1.0f;
        orResult.m_arLeastPenAxis[2] *= -1.0f;
    }

    return orResult;
}

int Collider::CheckIntervalOverlap(float* arAxis, float* arVertsA, float* arVertsB, OverlapResult& orResult)
{
    int i        = 0;
    float fDotA  = 0.0f;
    float fDotB  = 0.0f;
    float fDepth = 0.0f;

    // Do not check interval if axis is {0,0,0}
    // I think this happens when crossing two parallel vectors.
    if (arAxis[0] == 0.0f &&
        arAxis[1] == 0.0f &&
        arAxis[2] == 0.0f)
    {
        return 1;
    }

    // Waited to normalize the vector until now to cut down
    // on unnecessary normalizations. Normalization needs to be 
    // performed to make sure the dot products return the actual
    // projected length. The axis may not be a unit-vector if the
    // Matter's model matrix contains scaling factors.
    NormalizeVector(arAxis);

    // Set initial mins and maxes
    float fMinA = DotProduct(arAxis, arVertsA);
    float fMaxA = fMinA;

    float fMinB = DotProduct(arAxis, arVertsB);
    float fMaxB = fMinB;

    for (i = 1; i < VERTICES_PER_BOX; i++)
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

    // The intervals do overlap, so return 1.
    // But also record the penetration depth if it is lower
    // than the previous recording in the OverlapResult object
    if (fMaxB > fMaxA)
    {
        fDepth = fMaxA - fMinB;
    }
    else
    {
        fDepth = fMaxB - fMinA;
    }

    if (fDepth < orResult.m_fOverlapDepth)
    {
        orResult.m_fOverlapDepth = fDepth;
        orResult.m_arLeastPenAxis[0] = arAxis[0];
        orResult.m_arLeastPenAxis[1] = arAxis[1];
        orResult.m_arLeastPenAxis[2] = arAxis[2];
    }

    return 1;
}
