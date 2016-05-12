#include "Collider.h"
#include "Matrix.h"

enum OrientedBoxEnum
{
    VERTICES_PER_OBB = 8,
};

class OrientedBoxCollider : public Collider
{

public:

    OrientedBoxCollider();

    ~OrientedBoxCollider();

    void Render(Matrix* pMVP);

    int Overlaps(Collider* pOther,
                 void*     pOtherMatter,
                 void*     pThisMatter);

    void SetHalfExtents(float fHalfX,
                        float fHalfY,
                        float fHalfZ);

    void SetRelativeRotation(float fRotX,
                             float fRotY,
                             float fRotZ);

private:

    void GenerateLocalCoordinates(float* arRes);

    int CheckIntervalOverlap(float* arAxis, float* arVertsA, float* arVertsB);

    void RenderPrimaryNormals(Matrix* pMVP);

    Matrix m_matRotation;

    float m_arHalfExtents[3];

    float m_arRotation[3];

};