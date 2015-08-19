#ifndef COLLIDER_H
#define COLLIDER_H

#include "Matrix.h"

enum ColliderType
{
    COLLIDER_NO_TYPE  = -1,
    COLLIDER_BOX      =  0,
    COLLIDER_MESH     =  1
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
    ~Collider();

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
    void SetPosition(float fX,
                     float fY,
                     float fZ);

    //## **********************************************************************
    //## SetScale
    //## 
    //## Sets the scale of the collider. This should be called whenever the
    //## scale of the parent Matter is changed.
    //## 
    //## Input:
    //##   fScaleX - x scaling component.
    //##   fScaleY - y scaling component.
    //##   fScaleZ - z scaling component.
    //## **********************************************************************
    void SetScale(float fScaleX,
                  float fScaleY,
                  float fScaleZ);

    //## **********************************************************************
    //## SetRotation
    //##
    //## Sets the rotation of the collider. This should be called whenever the
    //## rotation of the parent Matter has changed.
    //##
    //## Input:
    //##   fRotX - rotation about x axis in degrees.
    //##   fRotY - rotation about y axis in degrees.
    //##   fRotZ - rotation about z axis in degrees.
    //## **********************************************************************
    void SetRotation(float fRotX,
                     float fRotY,
                     float fRotZ);

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
    //## SetMatter
    //## 
    //## Sets the parent Matter. This function should be called whenever 
    //## this collider gets assigned to a Matter.
    //## 
    //## Input:
    //##   pMatter - pointer to new parent matter.
    //## **********************************************************************
    void SetMatter(void* pMatter);

    //## **********************************************************************
    //## GetMatter
    //##
    //## Use to retrieve the Matter that this collider is currently bound to.
    //##
    //## Returns:
    //##   void* - pointer to parent Matter.
    //## **********************************************************************
    void* GetMatter();

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
    virtual int Overlaps(Collider* pOther) = 0;

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
    float m_fX;
    float m_fY;
    float m_fZ;

    //## Scale of the collider
    float m_fScaleX;
    float m_fScaleY;
    float m_fScaleZ;

    //## Rotation of the collider
    float m_fRotX;
    float m_fRotY;
    float m_fRotZ;

    //## Color to be used when rendering the collider while debugging.
    float m_arRenderColor[4];

    //## Render enable flag
    int m_nRenderEnable;

    //## Pointer to the parent Matter. Collider-Matter pairing is 
    //## 1 to 1, there cannot be one collider assigned to multiple matters.
    void* m_pMatter;
};

#endif