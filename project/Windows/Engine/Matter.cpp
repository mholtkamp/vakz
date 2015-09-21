#include "Matter.h"
#include "Log.h"
#include "VGL.h"
#include "Camera.h"
#include "Scene.h"
#include <stdio.h>

#define MATTER_MAX_POINT_LIGHTS 4
#define MATTER_MAX_DIRECTIONAL_LIGHTS 1

//*****************************************************************************
// Constructor
//*****************************************************************************
Matter::Matter()
{
    m_fX = 0.0f;
    m_fY = 0.0f;
    m_fZ = 0.0f;

    m_fRotX = 0.0f;
    m_fRotY = 0.0f;
    m_fRotZ = 0.0f;

    m_fScaleX = 1.0f;
    m_fScaleY = 1.0f;
    m_fScaleZ = 1.0f;

    m_pMesh     = 0;
    m_pMaterial = 0;
    m_pTexture  = 0;
    m_pCollider = 0;

    m_nCurrentAnimation   = 0;
    m_nPlayOnceAnimation  = -1;
    m_fCurrentFrame       = 0.0f;
    m_nLoopMode           = LOOP_NONE;
    m_nPlay               = 0;
    m_nDirection          = DIRECTION_FORWARD;
    m_fSpeed              = 1.0f;

    m_nPhysical = 0;
    m_nRigid    = 0;

    m_arVelocity[0] = 0.0f;
    m_arVelocity[1] = 0.0f;
    m_arVelocity[2] = 0.0f;
}

//*****************************************************************************
// Destructor
//*****************************************************************************
Matter::~Matter()
{
    
}

//*****************************************************************************
// SetMesh
//*****************************************************************************
void Matter::SetMesh(Mesh* pMesh)
{
    // Set the new mesh.
    m_pMesh   = pMesh;
}

//*****************************************************************************
// SetMaterial
//*****************************************************************************
void Matter::SetMaterial(Material* pMaterial)
{
    // Set the new material
    m_pMaterial = pMaterial;
}

//*****************************************************************************
// SetTexture
//*****************************************************************************
void Matter::SetTexture(Texture* pTexture)
{
    m_pTexture = pTexture;
}

//*****************************************************************************
// SetCollider
//*****************************************************************************
void Matter::SetCollider(Collider* pCollider)
{
    m_pCollider = pCollider;
    if (m_pCollider != 0)
    {
        m_pCollider->SetPosition(m_fX, m_fY, m_fZ);
        m_pCollider->SetRotation(m_fRotX, m_fRotY, m_fRotZ);
        m_pCollider->SetScale(m_fScaleX, m_fScaleY, m_fScaleZ);
        m_pCollider->SetMatter(this);
    }
}

//*****************************************************************************
// GetCollider
//*****************************************************************************
Collider* Matter::GetCollider()
{
    return m_pCollider;
}

//*****************************************************************************
// Render
//*****************************************************************************
void Matter::Render(void* pScene)
{
    int               i             =  0;
    unsigned int      hProg         =  0;
    int               hMatrixMVP    = -1;
    int               hMatrixM      = -1;
    int               hAmbientColor = -1;
    int               hTexture      = -1;
    int               hTextureMode  = -1;
    Matrix*           pView         =  0;
    Matrix*           pProjection   =  0;
    Camera*           pCamera       = reinterpret_cast<Scene*>(pScene)->GetCamera();
    Light**           pLights       = reinterpret_cast<Scene*>(pScene)->GetLightArray();
    int               nNumLights    = reinterpret_cast<Scene*>(pScene)->GetNumLights();
    float*            pAmbientColor = 0;
    DirectionalLight* pDirLight     = 0;
    Matrix            matMVP;

    // Generate model matrix
    // Note: May want to move this generation to the SetPosition()/
    // SetRotation()/SetScale() functions to reduce calculations.
    GenerateModelMatrix();
    
    if (m_pMesh       != 0 &&
        m_pMaterial   != 0 &&
        pCamera       != 0)
    {
        // Update animations (if there are any)
        UpdateAnimation();

        // Get the proper shader based on mesh type and material type
        hProg = GetMatterShaderProgram(m_pMesh->GetType(),
                                       m_pMaterial->GetType());
        
        // Start Rendering
        glUseProgram(hProg);

        // Matter is textured, bind texture and set uniforms
        hTexture     = glGetUniformLocation(hProg, "uTexture");
        hTextureMode = glGetUniformLocation(hProg, "uTextureMode");
        if (m_pTexture != 0)
        {
            m_pTexture->Bind();
            glUniform1i(hTexture, 0);
            glUniform1i(hTextureMode, 1);
        }
        else
        {
            glBindTexture(GL_TEXTURE_2D, 0);
            glUniform1i(hTextureMode, 0);
        }
    
        // Set up uniforms/attributes
        m_pMesh->SetRenderState(pScene,
                                hProg,
                                (m_nPlayOnceAnimation != -1) ? m_nPlayOnceAnimation : m_nCurrentAnimation,
                                m_fCurrentFrame);
        m_pMaterial->SetRenderState(pScene,
                                    hProg);

        // Set up directional lighting
        pDirLight = reinterpret_cast<Scene*>(pScene)->GetDirectionalLight();
        if (pDirLight != 0)
        {
            pDirLight->SetRenderState(pScene,
                                      hProg,
                                      0);
        }
        
        // Fetch ambient light from the scene
        pAmbientColor = reinterpret_cast<Scene*>(pScene)->GetAmbientLight();
        hAmbientColor = glGetUniformLocation(hProg, "uAmbientColor");
        glUniform4fv(hAmbientColor, 1, pAmbientColor);

        // Get View and Projection matrices from camera
        pView       = pCamera->GetViewMatrix();
        pProjection = pCamera->GetProjectionMatrix();

        // Construct MVP matrix
        if (pView       != 0 &&
            pProjection != 0)
        {
            // MVP matrix = Projection * View * Model
            matMVP = (*pProjection) * (*pView) * m_matModel;
            hMatrixMVP = glGetUniformLocation(hProg, "uMatrixMVP");
            glUniformMatrix4fv(hMatrixMVP, 1, GL_FALSE, matMVP.GetArray());

            // Create the normal matrix
            m_matModel.Inverse();
            m_matModel.Transpose();

            // Upload model matrix to shader for normal transformations
            hMatrixM = glGetUniformLocation(hProg, "uMatrixM");
            glUniformMatrix4fv(hMatrixM, 1, GL_FALSE, m_matModel.GetArray());
        }
        else
        {
            LogWarning("Could not retrieve view/projection matrix from camera in Matter::Render().");
            return;
        }
        
        glDrawArrays(GL_TRIANGLES,
                     0,
                     m_pMesh->GetVertexCount());
    }
}

//*****************************************************************************
// UpdatePhysics
//*****************************************************************************
void Matter::UpdatePhysics(void* pScene,
                           float fSeconds)
{
    Scene* pScn = reinterpret_cast<Scene*>(pScene);

    if (m_nPhysical != 0)
    {
        m_arVelocity[1] += -1 * pScn->GetGravity() * fSeconds; 
        m_fX += m_arVelocity[0] * fSeconds;
        m_fY += m_arVelocity[1] * fSeconds;
        m_fZ += m_arVelocity[2] * fSeconds;

        if (m_pCollider != 0)
        {
            m_pCollider->SetPosition(m_fX, m_fY, m_fZ);
        }
    }
}

//*****************************************************************************
// GenerateModelMatrix
//*****************************************************************************
Matrix* Matter::GetModelMatrix()
{
    GenerateModelMatrix();
    return &m_matModel;
}

//*****************************************************************************
// GenerateModelMatrix
//*****************************************************************************
void Matter::GenerateModelMatrix()
{
    // Reset the matrix
    m_matModel.LoadIdentity();

    // Translate
    m_matModel.Translate(m_fX, m_fY, m_fZ);

    // Rotate
    m_matModel.Rotate(m_fRotX, 1.0f, 0.0f, 0.0f);
    m_matModel.Rotate(m_fRotY, 0.0f, 1.0f, 0.0f);
    m_matModel.Rotate(m_fRotZ, 0.0f, 0.0f, 1.0f);

    // Scale
    m_matModel.Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
}

//*****************************************************************************
// SetPosition
//*****************************************************************************
void Matter::SetPosition(float fX,
                         float fY,
                         float fZ)
{
    m_fX = fX;
    m_fY = fY;
    m_fZ = fZ;

    if (m_pCollider != 0)
    {
        m_pCollider->SetPosition(fX, fY, fZ);
    }
}

//*****************************************************************************
// SetRotation
//*****************************************************************************
void Matter::SetRotation(float fRotX,
                         float fRotY,
                         float fRotZ)
{
    m_fRotX = fRotX;
    m_fRotY = fRotY;
    m_fRotZ = fRotZ;

    if (m_pCollider != 0)
    {
        m_pCollider->SetRotation(fRotX, fRotY, fRotZ);
    }
}

//*****************************************************************************
// SetScale
//*****************************************************************************
void Matter::SetScale(float fScaleX,
                      float fScaleY,
                      float fScaleZ)
{
    m_fScaleX = fScaleX;
    m_fScaleY = fScaleY;
    m_fScaleZ = fScaleZ;

    if (m_pCollider != 0)
    {
        m_pCollider->SetScale(fScaleX, fScaleY, fScaleZ);
    }
}

//*****************************************************************************
// Translate
//*****************************************************************************
void Matter::Translate(float fTransX,
                       float fTransY,
                       float fTransZ)
{
    m_fX += fTransX;
    m_fY += fTransY;
    m_fZ += fTransZ;

    if (m_pCollider != 0)
    {
        m_pCollider->SetPosition(m_fX, m_fY, m_fZ);
    }
}

//*****************************************************************************
// SetAnimation
//*****************************************************************************
void Matter::SetAnimation(const char* pAnimation)
{
    int i                   = 0;
    int nNewAnimation       = 0;
    AnimatedMesh* pAnimMesh = 0;

    if (m_pMesh            != 0 &&
        m_pMesh->GetType() == MESH_ANIMATED)
    {
        // Convert the matter's mesh to an animated mesh
        pAnimMesh = reinterpret_cast<AnimatedMesh*>(m_pMesh);

        // Retrieve the index of the animation
        nNewAnimation = pAnimMesh->GetAnimationIndex(pAnimation);

        // If a valid animation index was returned, then set it
        if (nNewAnimation != -1)
        {
            m_nCurrentAnimation = nNewAnimation;
        }
        else
        {
            LogWarning("No matching animation found in Matter::SetAnimation().");
        }

        // Set the curret frame to 0
        m_fCurrentFrame = 0.0f;
    }
}

//*****************************************************************************
// PlayAnimationOnce
//*****************************************************************************
void Matter::PlayAnimationOnce(const char* pAnimation)
{
    int i                   = 0;
    int nNewAnimation       = 0;
    AnimatedMesh* pAnimMesh = 0;

    if (m_pMesh            != 0 &&
        m_pMesh->GetType() == MESH_ANIMATED)
    {
        // Convert the matter's mesh to an animated mesh
        pAnimMesh = reinterpret_cast<AnimatedMesh*>(m_pMesh);

        // Retrieve the index of the animation
        nNewAnimation = pAnimMesh->GetAnimationIndex(pAnimation);

        // If a valid animation index was returned, then set it
        if (nNewAnimation != -1)
        {
            m_nPlayOnceAnimation = nNewAnimation;
            m_fCurrentFrame = 0;
        }
    }
}

//*****************************************************************************
// SetLoopMode
//*****************************************************************************
void Matter::SetLoopMode(int nLoopMode)
{
    if (nLoopMode == LOOP_NONE  ||
        nLoopMode == LOOP_CYCLE ||
        nLoopMode == LOOP_PING_PONG)
    {
        m_nLoopMode = nLoopMode;

        if (nLoopMode == LOOP_NONE ||
            nLoopMode == LOOP_CYCLE)
        {
            m_nDirection = DIRECTION_FORWARD;
        }
    }
    else
    {
        LogWarning("Invalid loop mode set in AnimatedMesh::SetLoopMode().");
    }
}

//*****************************************************************************
// StartAnimation
//*****************************************************************************
void Matter::StartAnimation()
{
    m_nPlay = 1;
    m_timerFrame.Start();
}

//*****************************************************************************
// StopAnimation
//*****************************************************************************
void Matter::StopAnimation()
{
    m_nPlay = 0;
    m_timerFrame.Stop();
}

//*****************************************************************************
// ResetAnimation
//*****************************************************************************
void Matter::ResetAnimation()
{
    m_fCurrentFrame = 0.0f;
}

//*****************************************************************************
// SetAnimationSpeed
//*****************************************************************************
void Matter::SetAnimationSpeed(float fSpeed)
{
    m_fSpeed = fSpeed;
}

//*****************************************************************************
// UpdateAnimation
//*****************************************************************************
void Matter::UpdateAnimation()
{
    AnimatedMesh* pAnimMesh    = 0;
    int   i                    = 0;
    float fElapsedTime         = 0.0f;
    int   nKeyFrames           = 0;
    float fLastFrame           = 0.0f;
    float fFramesPerSecond     = 0.0f;

    if(m_nPlay != 0 &&
       m_pMesh != 0 &&
       m_pMesh->GetType() == MESH_ANIMATED)
    {
        pAnimMesh            = reinterpret_cast<AnimatedMesh*>(m_pMesh);
        nKeyFrames           = pAnimMesh->GetKeyFrameCount(m_nCurrentAnimation);
        fLastFrame           = static_cast<float>(pAnimMesh->GetLastFrame(m_nCurrentAnimation));
        fFramesPerSecond     = static_cast<float>(pAnimMesh->GetFramesPerSecond());

        // Get the time since last render
        m_timerFrame.Stop();
        fElapsedTime = m_timerFrame.Time();
        
        // Reset the timer to record next frame's time
        m_timerFrame.Start();

        if (m_nPlayOnceAnimation != -1)
        {
            if (static_cast<int>(m_fCurrentFrame) < fLastFrame)
            {
                // If the animation hasn't ended, increase the frame count.
                m_fCurrentFrame += fElapsedTime     * 
                                   fFramesPerSecond * 
                                   m_fSpeed;
            }
            else
            {
                m_nPlayOnceAnimation = -1;
            }
        }

        if (m_nLoopMode == LOOP_NONE)
        {
            if (static_cast<int>(m_fCurrentFrame) < fLastFrame)
            {
                // If the animation hasn't ended, increase the frame count.
                m_fCurrentFrame += fElapsedTime * 
                                   fFramesPerSecond * 
                                   m_fSpeed;
            }
        }
        else if (m_nLoopMode == LOOP_CYCLE)
        {
            // Increase the framecount
            m_fCurrentFrame += fElapsedTime * 
                               fFramesPerSecond * 
                               m_fSpeed;
            
            // If the current frame is higher than the last keyframe, then
            // subtract the number of frames in the animation to reset animation.
            if (m_fCurrentFrame >= fLastFrame)
            {
                m_fCurrentFrame -= fLastFrame;
            }
        }
        else if (m_nLoopMode == LOOP_PING_PONG)
        {
            if (m_nDirection == DIRECTION_FORWARD)
            {
                // Increase the framecount
                m_fCurrentFrame += fElapsedTime * 
                                   fFramesPerSecond *
                                   m_fSpeed;

                // If the current frame is higher than the last keyframe, then
                // reverse the direction.
                if (m_fCurrentFrame >= fLastFrame)
                {
                    m_nDirection = DIRECTION_REVERSE;
                }
            }
            else
            {
                // Decrease the framecount
                m_fCurrentFrame -= fElapsedTime *
                                   fFramesPerSecond *
                                   m_fSpeed;

                // If the current frame is less than than 0, then 
                // set the direction to forward
                if (m_fCurrentFrame <= 0.0f)
                {
                    m_nDirection = DIRECTION_FORWARD;
                    m_fCurrentFrame = 0.0f;
                }
            }
        }
    }
}

//*****************************************************************************
// Overlaps
//*****************************************************************************
int Matter::Overlaps(Matter* pOther)
{
    if (pOther                != 0 &&
        pOther->GetCollider() != 0 &&
        m_pCollider           != 0)
    {
        return m_pCollider->Overlaps(pOther->GetCollider());
    }
    else
    {
        return 0;
    }
}

//*****************************************************************************
// SetPhysical
//*****************************************************************************
void Matter::SetPhysical(int nPhysical)
{
    m_nPhysical = nPhysical;
}

//*****************************************************************************
// SetRigid
//*****************************************************************************
void Matter::SetRigid(int nRigid)
{
    m_nRigid = nRigid;
}

//*****************************************************************************
// SetVelocity
//*****************************************************************************
void Matter::SetVelocity(float fXVel,
                         float fYVel,
                         float fZVel)
{
    m_arVelocity[0] = fXVel;
    m_arVelocity[1] = fYVel;
    m_arVelocity[2] = fZVel;
}

//*****************************************************************************
// SetXVelocity
//*****************************************************************************
void Matter::SetXVelocity(float fXVel)
{
    m_arVelocity[0] = fXVel;
}

//*****************************************************************************
// SetYVelocity
//*****************************************************************************
void Matter::SetYVelocity(float fYVel)
{
    m_arVelocity[1] = fYVel;
}

//*****************************************************************************
// SetZVelocity
//*****************************************************************************
void Matter::SetZVelocity(float fZVel)
{
    m_arVelocity[2] = fZVel;
}
