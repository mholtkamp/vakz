#include "Camera.h"
#include "Log.h"
#include "VMath.h"
#include <math.h>

//*****************************************************************************
// Constructor
//*****************************************************************************
Camera::Camera()
{
    m_fX = 0.0f;
    m_fY = 0.0f;
    m_fZ = 0.0f;
    
    m_fRotX = 0.0f;
    m_fRotY = 0.0f;
    m_fRotZ = 0.0f;

    m_fFovX = 0.0f;
    m_fFovY = 0.0f;

    m_fNear = 0.0f;
    m_fFar  = 0.0f;

    m_nProjectionType = CAMERA_PERSPECTIVE;

    m_matView.LoadIdentity();
    m_matProjection.LoadIdentity();
}

//*****************************************************************************
// Destructor
//*****************************************************************************
Camera::~Camera()
{

}

//*****************************************************************************
// SetPosition
//*****************************************************************************
void Camera::SetPosition(float fX,
                         float fY,
                         float fZ)
{
    m_fX = fX;
    m_fY = fY;
    m_fZ = fZ;
}

//*****************************************************************************
// SetRotation
//*****************************************************************************
void Camera::SetRotation(float fRotX,
                         float fRotY,
                         float fRotZ)
{
    m_fRotX = fRotX;
    m_fRotY = fRotY;
    m_fRotZ = fRotZ;
}

//*****************************************************************************
// LookAt
//*****************************************************************************
void Camera::LookAt(float fX,
                    float fY,
                    float fZ)
{
    float fDistX = fX - m_fX;
    float fDistY = fY - m_fY;
    float fDistZ = fZ - m_fZ;

    // This function currently only supports y-axis is up
    m_fRotZ = 0.0f; 

    // Pitch is in range -90 degrees to 90 degrees
    m_fRotX = atan(fDistY/(-1*fDistZ)) * RADIANS_TO_DEGREES;
   
    // The Yaw is in range -90 to 270
    if (fDistZ < 0.0f)
    {
        m_fRotY = atan(fDistX/(-1*fDistZ)) * RADIANS_TO_DEGREES;
    }
    else
    {
        m_fRotY = (atan(fDistX/(-1*fDistZ)) + 180.0f) * RADIANS_TO_DEGREES;
    }

    //@@ TODO:
    //@@ gluLookAt method... produces view matrix.
    //@@ Remove this after testing the above implemented method
    //float fMagS   = 0.0f;
    //float arNS[3] = {0.0f, 0.0f, 0.0f};

    //// Side, Up, and Forward vectors
    //float arS[3] = {0.0f, 0.0f, 0.0f};
    //float arU[3] = {0.0f, 0.0f, 0.0f};
    //float arF[3] = {fX - m_fX,
    //                fY - m_fY,
    //                fZ - m_fZ};

    //float arUp[3] = {0.0f, 1.0f, 0.0f};

    //// Calculate magnitude of forward vector
    //float fMagF = sqrt(arF[0]*arF[0]
    //                 + arF[1]*arF[1]
    //                 + arF[2]*arF[2]);
    //
    //// Check to see if the forward vector needs to be normalized
    //if (fMagF != 1.0f)
    //{
    //    if (fMagF != 0.0f)
    //    {
    //        // Normalize
    //        arF[0] = arF[0]/fMagF;
    //        arF[1] = arF[1]/fMagF;
    //        arF[2] = arF[2]/fMagF;
    //    }
    //    else
    //    {
    //        LogWarning("Camera::LookAt - Eye and Center are same point.");
    //        return;
    //    }
    //}

    //// Take cross product of arF X arUp to get arS
    //arS[0] =        arF[1]*arUp[2] - arF[2]*arUp[1];
    //arS[1] = -1.0f*(arF[0]*arUp[2] - arF[2]*arUp[0]);
    //arS[2] =        arF[0]*arUp[1] - arF[1]*arUp[0];

    //fMagS = sqrt(arS[0]*arS[0]
    //           + arS[1]*arS[1]
    //           + arS[2]*arS[2]);

    //if (fMagS != 1.0f)
    //{
    //    if (fMagS != 0.0f)
    //    {
    //        arNS[0] = arS[0]/fMagS;
    //        arNS[1] = arS[1]/fMagS;
    //        arNS[2] = arS[2]/fMagS;
    //    }
    //    else
    //    {
    //        LogWarning("Camera::LookAt - Side vector is 0.");
    //        return;
    //    }
    //}

    //// Take cross product of arNS X arF to get arU
    //arU[0] =        arNS[1]*arF[2] - arNS[2]*arF[1];
    //arU[1] = -1.0f*(arNS[0]*arF[2] - arNS[2]*arF[0]);
    //arU[2] =        arNS[0]*arF[1] - arNS[1]*arF[0];

    //m_matView.Set();
}

//*****************************************************************************
// SetFieldOfView
//*****************************************************************************
void Camera::SetFieldOfView(float fFovX,
                            float fFovY)
{
    m_fFovX = fFovX;
    m_fFovY = fFovY;
}

//*****************************************************************************
// SetNearPlane
//*****************************************************************************
void Camera::SetNearPlane(float fNear)
{
    if (fNear > 0.0f)
    {
        m_fNear = fNear;
    }
    else
    {
        LogWarning("Invalid near plane value in Camera::SetNearPlane.");
    }
}

//*****************************************************************************
// SetFarPlane
//*****************************************************************************
void Camera::SetFarPlane(float fFar)
{
    if (fFar > 0.0f)
    {
        m_fFar = fFar;
    }
    else
    {
        LogWarning("Invalid far plane value in Camera::SetFarPlane().");
    }
}

//*****************************************************************************
// SetProjectionType
//*****************************************************************************
void Camera::SetProjectionType(int nProjectionType)
{
    if (nProjectionType == CAMERA_ORTHOGRAPHIC ||
        nProjectionType == CAMERA_PERSPECTIVE)
    {
        m_nProjectionType = nProjectionType;
    }
    else
    {
        LogWarning("Invalid projection type in Camera::SetProjectionType().");
    }
}

//*****************************************************************************
// GenerateViewMatrix()
//*****************************************************************************
Matrix* Camera::GenerateViewMatrix()
{
    //@@ TODO
    return &m_matView;
}

//*****************************************************************************
// GenerateProjectionMatrix()
//*****************************************************************************
Matrix* Camera::GenerateProjectionMatrix()
{
    //@@ TODO

    return &m_matProjection;
}
