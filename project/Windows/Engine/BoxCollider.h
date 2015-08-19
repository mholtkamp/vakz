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
    //## Sets the bounds of the axis-aligned bounding box collider. As of now,
    //## abs(fMinx) should equal abs(fMaxX). The coordinates should be
    //## provided in model space, when checking collisions, the coordinates
    //## will be translated to their world positions.
    //## **********************************************************************
    void SetExtents(float fMinX,
                    float fMaxX,
                    float fMinY,
                    float fMaxY,
                    float fMinZ,
                    float fMaxZ);

    //## **********************************************************************
    //## Render
    //## 
    //## Renders the box collider using the static mesh fullbright shader. This
    //## feature is useful for debugging. Before rendering, the render 
    //## flag must be enabled by calling EnableRendering().
    //##
    //## Input:
    //##   pMVP - the model-view-projection combined matrix
    //## **********************************************************************
    void Render(Matrix* pMVP);

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
    int Overlaps(Collider* pOther);

    //## **********************************************************************
    //## GetMin/GetMax
    //## 
    //## Returns the extent of the bounding box in world space coordinates.
    //## **********************************************************************
    float GetMinX();
    float GetMaxX();
    float GetMinY();
    float GetMaxY();
    float GetMinZ();
    float GetMaxZ();

    //## **********************************************************************
    //## Enable/DisableRendering
    //##
    //## Enable collider rendering for debugging.
    //## **********************************************************************
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