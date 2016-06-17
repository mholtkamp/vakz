#ifndef COLLIDER_H
#define COLLIDER_H

#include "Matrix.h"
#include "Box.h"

#define VERTICES_PER_BOX 8

enum ColliderType
{
    COLLIDER_NO_TYPE      = -1,
    COLLIDER_BOX          =  0,
    COLLIDER_ORIENTED_BOX =  1
};

// This struct is returned by the Overlaps() method
struct OverlapResult
{
    int m_nOverlapping;
    float m_arLeastPenAxis[3];
    float m_fOverlapDepth;

    OverlapResult()
    {
        m_nOverlapping = 0;
        m_arLeastPenAxis[0] = 0.0f;
        m_arLeastPenAxis[1] = 0.0f;
        m_arLeastPenAxis[2] = 0.0f;
        m_fOverlapDepth = 999999.0f;
    }
};

class Collider
{

public:

    //## **********************************************************************
    //## Constructor
    //## **********************************************************************
    Collider();

    //## **********************************************************************
    //## Destructor
    //## **********************************************************************
    virtual ~Collider();

    //## **********************************************************************
    //## GetType
    //##
    //## Returns type of derived class. Refer to ColliderType enum above.
    //##
    //## Returns:
    //##   int - type of Collider
    //## **********************************************************************
    int GetType();

    //## **********************************************************************
    //## SetPosition
    //##
    //## Sets the position of the collider in world space. This should be 
    //## called whenever the parent matter is moved.
    //##
    //## Input:
    //##   fX - x coordinate.
    //##   fY - y coordinate.
    //##   fZ - z coordinate.
    //## **********************************************************************
    void SetRelativePosition(float fX,
                             float fY,
                             float fZ);

    //## **********************************************************************
    //## SetColor
    //##
    //## Sets the color that is used to render the collider when collider
    //## rendering is enabled.
    //##
    //## Input:
    //##   fRed   - red component.
    //##   fGreen - green component.
    //##   fBlue  - blue component.
    //##   fAlpha - alpha component.
    //## **********************************************************************
    void SetColor(float fRed,
                  float fGreen,
                  float fBlue,
                  float fAlpha);

    void SetColor(float* arColor);

    float* GetColor();

    //## **********************************************************************
    //## Render
    //##
    //## Renders the collider for debugging purposes. The MVP matrix provided
    //## should be the same as the matrix used to render the parent Matter. 
    //## Must be implemented by child class.
    //##
    //## Input:
    //##   pMatter - related matter that is using this collider
    //##   pScene - scene that collider is being rendered in
    //##   pMVP - model-view-projection matrix
    //## **********************************************************************
    virtual void Render(void* pMatter,
                        void* pScene,
                        Matrix* pMVP) = 0;

    //## **********************************************************************
    //## Overlaps
    //## 
    //## Checks whether two colliders overlap. Must be implemented by child.
    //##
    //## Input:
    //##   pOther - other collider to test.
    //##
    //## Returns:
    //##   int - '1' if the colliders overlap,
    //##         '0' otherwise.
    //## **********************************************************************
    virtual OverlapResult Overlaps(Collider* pOther,
                                   void*   pOtherMatter,
                                   void*   pThisMatter) = 0;

    const float* GetRelativePosition();

    virtual void GetBounds(void* pMatter, float* arMin, float* arMax) = 0;

protected:

    //## Collision functions
    static OverlapResult OBB_Overlaps_OBB(Collider* pColA,
                                          void* pColMatterA,
                                          Collider* pColB,
                                          void* pColMatterB);

    static OverlapResult AABB_Overlaps_OBB(Collider* pColAABB,
                                           void* pColMatterAABB,
                                           Collider* pColOBB,
                                           void* pColMatterOBB);

    static OverlapResult OBB_Overlaps_AABB(Collider* pColOBB,
                                           void* pColMatterOBB,
                                           Collider* pColAABB,
                                           void* pColMatterAABB);

    static OverlapResult AABB_Overlaps_AABB(Collider* pColA,
                                            void* pColMatterA,
                                            Collider* pColB,
                                            void* pColMatterB);

    static int CheckIntervalOverlap(float* arAxis, float* arVertsA, float* arVertsB, OverlapResult& orResult);

    //## Type of collider (ie. Mesh/Box/Sphere)
    int m_nType;

    //## Position of collider in world coordinates
    float m_arPosition[3];

    //## Color to be used when rendering the collider while debugging.
    float m_arRenderColor[4];
};

#endif