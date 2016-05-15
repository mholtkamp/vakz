#ifndef ORIENTED_BOX_COLLIDER_H
#define ORIENTED_BOX_COLLIDER_H

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

    void Render(void* pMatter,
                void* pScene,
                Matrix* pMVP);

    OverlapResult Overlaps(Collider* pOther,
                           void*     pOtherMatter,
                           void*     pThisMatter);

    void SetHalfExtents(float fHalfX,
                        float fHalfY,
                        float fHalfZ);

    void SetRelativeRotation(float fRotX,
                             float fRotY,
                             float fRotZ);

    const Matrix* GetRotationMatrix();

    void GenerateLocalCoordinates(float* arRes);

private:

    Matrix m_matRotation;

    float m_arHalfExtents[3];

    float m_arRotation[3];

};

#endif
