#ifndef BOX_COLLIDER_H
#define BOX_COLLIDER_H

#include "Collider.h"

class BoxCollider : public Collider
{

public:

    //## **********************************************************************
    //## Constructor
    //## **********************************************************************
    BoxCollider();

    //## **********************************************************************
    //## Destructor
    //## **********************************************************************
    ~BoxCollider();

    //## **********************************************************************
    //## SetExtents
    //##
    //## Sets the bounds of the axis-aligned bounding box collider. If the 
    //## X Half extent is set to 0.5f, then the width of the collider will
    //## be 1.0f along the X-Axis (always, as AABBs can not rotate.)
    //## **********************************************************************
    void SetHalfExtents(float fHalfX,
                        float fHalfY,
                        float fHalfZ);

    const float* GetHalfExtents();

    //## **********************************************************************
    //## Render
    //## 
    //## Renders the box collider using the static mesh fullbright shader. This
    //## feature is useful for debugging. Before rendering, the render 
    //## flag must be enabled by calling EnableRendering().
    //##
    //## Input:
    //##   pMVP - matrix
    //##   pScene - current scene
    //##   pMVP - the model-view-projection combined matrix
    //## **********************************************************************
    void Render(void* pMatter,
                void* pScene,
                Matrix* pMVP);

    //## **********************************************************************
    //## Overlaps
    //## 
    //## Overridden method to determine if this collider overlaps another
    //## collider. 
    //##
    //## Input:
    //##   pOther - the other collider of interest. Does not need to be a
    //##            BoxCollider.
    //## **********************************************************************
    OverlapResult Overlaps(Collider* pOther,
                           void*     pOtherMatter,
                           void*     pThisMatter);

    //## **********************************************************************
    //## Enable/DisableRendering
    //##
    //## Enable collider rendering for debugging.
    //## **********************************************************************
    void EnableRendering();
    void DisableRendering();

    void GenerateLocalCoordinates(float* pRes);

    void GetBounds(void* pMatter, float* arMin, float* arMax);

private:

    //## The local half-extents of the box.
    float m_arHalfExtents[3];
};

#endif