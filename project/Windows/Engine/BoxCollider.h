#ifndef BOX_COLLIDER_H
#define BOX_COLLIDER_H

#include "Collider.h"

class BoxCollider : public Collider
{

public:

    BoxCollider();

    ~BoxCollider();

    void SetExtents(float fMinX,
                    float fMaxX,
                    float fMinY,
                    float fMaxY,
                    float fMinZ,
                    float fMaxZ);

    void Render(Matrix* pMVP);

    int Overlaps(Collider* pOther);

    float GetMinX();

    float GetMaxX();

    float GetMinY();
    
    float GetMaxY();

    float GetMinZ();

    float GetMaxZ();

    void EnableRendering();

    void DisableRendering();

private:

    //## Extents of axis aligned bounding box
    //## These are local extents.
    float m_fMinX;
    float m_fMaxX;
    float m_fMinY;
    float m_fMaxY;
    float m_fMinZ;
    float m_fMaxZ;

    //## Flag to indicate whether the box should be rendered
    //## for debugging purposes.
    int m_nRender;

    //## A position array used for rendering the box collider
    //## if the enable has been set.
    float* m_arPosition;
};

#endif