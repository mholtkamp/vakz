#include "Camera.h"
#include "Settings.h"
#include "Log.h"
#include "VMath.h"
#include <math.h>

#define DEFAULT_NEAR 0.1f
#define DEFAULT_FAR 1024.0f
#define DEFAULT_FOV_X 60.0f
#define DEFAULT_FOV_Y 40.0f
#define DEFAULT_ORTHO_WIDTH 30.0f
#define DEFAULT_ORTHO_HEIGHT 20.0f

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

    m_fFovX = DEFAULT_FOV_X;
    m_fFovY = DEFAULT_FOV_Y;

    m_fNear = DEFAULT_NEAR;
    m_fFar  = DEFAULT_FAR;

    m_fOrthoWidth  = DEFAULT_ORTHO_WIDTH;
    m_fOrthoHeight = DEFAULT_ORTHO_HEIGHT;

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
    LogWarning("LookAt not implemented yet.");
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
// GenerateViewMatrix
//*****************************************************************************
void Camera::GenerateViewMatrix()
{
    // Reset the matrix back to identity
    m_matView.LoadIdentity();

    // Roll
    m_matView.Rotate(-m_fRotZ, 0.0f, 0.0f, 1.0f);

    // Pitch
    m_matView.Rotate(-m_fRotX, 1.0f, 0.0f, 0.0f);

    // Heading
    m_matView.Rotate(-m_fRotY, 0.0f, 1.0f, 0.0f);

    // Translation
    m_matView.Translate(-m_fX, -m_fY, -m_fZ);
}

//*****************************************************************************
// GenerateProjectionMatrix
//*****************************************************************************
void Camera::GenerateProjectionMatrix()
{
    float fWidth       = 0.0f;
    float fHeight      = 0.0f;
    float fAspectRatio = 0.0f;
    float fFovXRadians = m_fFovX * DEGREES_TO_RADIANS;
    float fFovYRadians = m_fFovY * DEGREES_TO_RADIANS;

    //Reset the matrix back to identity
    m_matProjection.LoadIdentity();

    // Perform proper projection
    if (m_nProjectionType == CAMERA_ORTHOGRAPHIC)
    {
        m_matProjection.Ortho(-m_fOrthoWidth/2.0f,
                               m_fOrthoWidth/2.0f,
                              -m_fOrthoHeight/2.0f,
                               m_fOrthoHeight,
                               m_fNear,
                               m_fFar);
    }
    else if (m_nProjectionType == CAMERA_PERSPECTIVE)
    {
        fHeight = 2.0f * tan(fFovYRadians/2.0f) * m_fNear;

        // If no x-field of view is provided, then use the
        // screen height/width from settings to calculate
        // the aspect ratio to derive the width from.
        if (fFovXRadians != 0.0f)
        {
            fWidth = 2.0f * tan(fFovXRadians/2.0f) * m_fNear;
        }
        else
        {
            fAspectRatio = ((float) g_nResolutionX)/ ((float) g_nResolutionY);
            fWidth = fHeight * fAspectRatio;
        }

        m_matProjection.Frustum(-fWidth/2.0f,
                                 fWidth/2.0f,
                                -fHeight/2.0f,
                                 fHeight/2.0f,
                                 m_fNear,
                                 m_fFar);
    }
}

//*****************************************************************************
// GetViewMatrix
//*****************************************************************************
Matrix* Camera::GetViewMatrix()
{
    return &m_matView;
}

//*****************************************************************************
// GetProjectionMatrix
//*****************************************************************************
Matrix* Camera::GetProjectionMatrix()
{
    return &m_matProjection;
}

//*****************************************************************************
// SetOrthographicWindow
//*****************************************************************************
void Camera::SetOrthographicWindow(float fWidth,
                                   float fHeight)
{
    m_fOrthoWidth  = fWidth;
    m_fOrthoHeight = fHeight;
}
