#ifndef CAMERA_H
#define CAMERA_H

#include "Matrix.h"

class Camera
{

public:
    
    //## **********************************************************************
    //## Constructor
    //## **********************************************************************
    Camera();

    //## **********************************************************************
    //## Destructor
    //## **********************************************************************
    ~Camera();

    //## **********************************************************************
    //## SetPosition
    //## 
    //## Sets position of the camera from where a scene can be rendered.
    //##
    //## Input:
    //##   fX - position along x axis.
    //##   fY - position along y axis.
    //##   fZ - position along z axis.
    //## **********************************************************************
    void SetPosition(float fX,
                     float fY,
                     float fZ);

    //## **********************************************************************
    //## SetRotation
    //## 
    //## Sets the rotation of the camera.
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
    //## LookAt
    //##
    //## Makes proper adjustments to the rotation of the camera so that it is
    //## looking at the given point.
    //##
    //## Input:
    //##   fX - point's x coordinate.
    //##   fY - point's y cooridnate.
    //##   fZ - point's z coordinate.
    //## **********************************************************************
    void LookAt(float fX,
                float fY,
                float fZ);

    //## **********************************************************************
    //## SetFieldOfView
    //##
    //## Sets the camera's field of view. The field of view is only used if 
    //## the camera is set to a perspective projection. If 0 is passed into the
    //## function for the x field of view, the screen width and height will be
    //## used to generate the x field of view from the y field of view.
    //## This function will silently fail if both fFovx and fFovy are 0.0f.
    //##
    //## Input:
    //##   fFovX - x field of view.
    //##   fFovY - y field of view.
    //## **********************************************************************
    void SetFieldOfView(float fFovX,
                        float fFovY);

    //## **********************************************************************
    //## SetNearPlane
    //## 
    //## Sets the distance of the near clipping plane.
    //##
    //## Input:
    //##   fNear - distance from camera to near clip plane.
    //## **********************************************************************
    void SetNearPlane(float fNear);

    //## **********************************************************************
    //## SetFarPlane
    //##
    //## Sets the distance of the far clipping plane.
    //##
    //## Input:
    //##   fFar - distance from camera to the far clip plane.
    //## **********************************************************************
    void SetFarPlane(float fFar);

    //## **********************************************************************
    //## SetProjectionType
    //##
    //## Sets the projection type of the camera.
    //##
    //## Input:
    //##   nProjectionType - '0' for orthographic projection.
    //##                   - '1' for perspective projection.
    //## **********************************************************************
    void SetProjectionType(int nProjectionType);

    //## **********************************************************************
    //## GenerateViewMatrix
    //##
    //## Calculates the view matrix based on the position and orientation of
    //## the camera.
    //## **********************************************************************
    void GenerateViewMatrix();

    //## **********************************************************************
    //## GenerateProjectionMatrix
    //##
    //## Calculates the projection matrix based on near/far clip planes and
    //## field of view.
    //## **********************************************************************
    void GenerateProjectionMatrix();

    //## **********************************************************************
    //## GetViewMatrix
    //##
    //## Returns a pointer to the current View matrix. This should be called
    //## only after GenerateViewMatrix has already been called for the current
    //## rendering frame.
    //##
    //## Input:
    //##   Matrix* - pointer to view matrix.
    //## **********************************************************************
    Matrix* GetViewMatrix();

    //## **********************************************************************
    //## GetProjectionMatrix
    //##
    //## Returns a pointer to the current Projection matrix. This should be 
    //## called only after GenerateProjctionMatrix has already been called for 
    //## the current rendering frame.
    //##
    //## Input:
    //##   Matrix* - pointer to projection matrix.
    //## **********************************************************************
    Matrix* GetProjectionMatrix();

    //## **********************************************************************
    //## SetOrthographicWindow
    //##
    //## Sets the dimensions of the orthographic camera.
    //##
    //## Input:
    //##   fWidth  - width of viewing range.
    //##   fHeight - height of viewing range.
    //## **********************************************************************
    void SetOrthographicWindow(float fWidth,
                               float fHeight);

    enum CameraEnum
    {
        CAMERA_ORTHOGRAPHIC = 0,
        CAMERA_PERSPECTIVE  = 1 
    };

private:

    //## Position of camera
    float m_fX;
    float m_fY;
    float m_fZ;

    //## Rotation of camera
    float m_fRotX;
    float m_fRotY;
    float m_fRotZ;

    //## Field of view
    float m_fFovX;
    float m_fFovY;

    //## Clipping planes
    float m_fNear;
    float m_fFar;

    //## Orthographic projection dimensions
    float m_fOrthoWidth;
    float m_fOrthoHeight;

    //## Projection Type: Orthographic or Perspective
    int m_nProjectionType;

    //## View matrix
    Matrix m_matView;

    //## Projection matrix
    Matrix m_matProjection;
};

#endif
