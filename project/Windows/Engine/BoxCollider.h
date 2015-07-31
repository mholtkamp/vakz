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

    void Render();

    int Overlaps(Collider* pOther);

    float GetMinX();

    float GetMaxX();

    float GetMinY();
    
    float GetMaxY();

    float GetMinZ();

    float GetMaxZ();

private:

    //## Extents of axis aligned bounding box
    //## These are local extents.
    float m_fMinX;
    float m_fMaxX;
    float m_fMinY;
    float m_fMaxY;
    float m_fMinZ;
    float m_fMaxZ;
};

#endif