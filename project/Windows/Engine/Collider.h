#ifndef COLLIDER_H
#define COLLIDER_H

#include "Matrix.h"

enum ColliderType
{
    COLLIDER_NO_TYPE      = -1,
    COLLIDER_BOX          =  0,
    COLLIDER_MESH         =  1,
    COLLIDER_ORIENTED_BOX =  2
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
        m_fOverlapDepth = 0.0f;
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

    //## **********************************************************************
    //## Render
    //##
    //## Renders the collider for debugging purposes. The MVP matrix provided
    //## should be the same as the matrix used to render the parent Matter. 
    //## Must be implemented by child.
    //##
    //## Input:
    //##   pMVP - model-view-projection matrix
    //## **********************************************************************
    virtual void Render(Matrix* pMVP) = 0;

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

    //## **********************************************************************
    //## Enable/DisableRendering
    //##
    //## Enable/Disable rendering of this individual collider for debugging.
    //## **********************************************************************
    void EnableRendering();
    void DisableRendering();

    //## **********************************************************************
    //## IsRenderingEnabled
    //##
    //## Returns:
    //##   int - '1' if rendering has been disabled.
    //##       - '0' otherwise.
    //## **********************************************************************
    int IsRenderingEnabled();

protected:

    //## Type of collider (ie. Mesh/Box/Sphere)
    int m_nType;

    //## Position of collider in world coordinates
    float m_arPosition[3];

    //## Color to be used when rendering the collider while debugging.
    float m_arRenderColor[4];

    //## Render enable flag
    int m_nRenderEnable;
};

#endif