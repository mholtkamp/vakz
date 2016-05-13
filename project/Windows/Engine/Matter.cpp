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
    m_arPosition[0] = 0.0f;
    m_arPosition[1] = 0.0f;
    m_arPosition[2] = 0.0f;

    m_arRotation[0] = 0.0f;
    m_arRotation[1] = 0.0f;
    m_arRotation[2] = 0.0f;

    m_arScale[0] = 1.0f;
    m_arScale[1] = 1.0f;
    m_arScale[2] = 1.0f;

    m_nVisible = 1;

    m_pMesh     = 0;
    m_pMaterial = 0;
    m_pTexture  = 0;

    m_nCurrentAnimation   = 0;
    m_nPlayOnceAnimation  = -1;
    m_fCurrentFrame       = 0.0f;
    m_nLoopMode           = LOOP_NONE;
    m_nPlay               = 0;
    m_nDirection          = DIRECTION_FORWARD;
    m_fSpeed              = 1.0f;

    m_nPhysical = 0;
    m_nMobile   = 0;
    m_nRenderColliders = 0;
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
void Matter::AddCollider(Collider* pCollider)
{
    m_lColliders.Add(pCollider);
}

//*****************************************************************************
// SetCollider
//*****************************************************************************
void Matter::RemoveCollider(Collider* pCollider)
{
    m_lColliders.Remove(pCollider);
}

//*****************************************************************************
// GetCollider
//*****************************************************************************
List* Matter::GetColliderList()
{
    return &m_lColliders;
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
    List*             pLightList    = reinterpret_cast<Scene*>(pScene)->GetLightList();
    int               nNumLights    = reinterpret_cast<Scene*>(pScene)->GetNumLights();
    float*            pAmbientColor = 0;
    DirectionalLight* pDirLight     = 0;
    Matrix            matMVP;

    // Generate model matrix
    // Note: May want to move this generation to the SetPosition()/
    // SetRotation()/SetScale() functions to reduce calculations.
    GenerateModelMatrix();

    // Update animations (if there are any)
    UpdateAnimation();
    
    if (m_pMesh       != 0 &&
        m_pMaterial   != 0 &&
        pCamera       != 0 &&
        m_nVisible    != 0)
    {
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

        // Render all the attached colliders if feature is enabled.
        if (m_nRenderColliders != 0)
        {
            ListNode* pNode = m_lColliders.GetHead();

            // Setup state for rendering translucent collision meshes
            glDepthMask(GL_FALSE);
            glDisable(GL_CULL_FACE);

            while (pNode != 0)
            {
                reinterpret_cast<Collider*>(pNode->m_pData)->Render(&matMVP);
                pNode = pNode->m_pNext;
            }

            // Restore state
            glEnable(GL_CULL_FACE);
            glDepthMask(GL_TRUE);
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
    m_matModel.Translate(m_arPosition[0], 
                         m_arPosition[1], 
                         m_arPosition[2]);

    // Rotate
    m_matModel.Rotate(m_arRotation[0], 1.0f, 0.0f, 0.0f);
    m_matModel.Rotate(m_arRotation[1], 0.0f, 1.0f, 0.0f);
    m_matModel.Rotate(m_arRotation[2], 0.0f, 0.0f, 1.0f);

    // Scale
    m_matModel.Scale(m_arScale[0], 
                     m_arScale[1], 
                     m_arScale[2]);
}

const float* Matter::GetPosition()
{
    return m_arPosition;
}

const float* Matter::GetRotation()
{
    return m_arRotation;
}

const float* Matter::GetScale()
{
    return m_arScale;
}
//*****************************************************************************
// SetPosition
//*****************************************************************************
void Matter::SetPosition(float fX,
                         float fY,
                         float fZ)
{
    m_arPosition[0] = fX;
    m_arPosition[1] = fY;
    m_arPosition[2] = fZ;
}

//*****************************************************************************
// SetRotation
//*****************************************************************************
void Matter::SetRotation(float fRotX,
                         float fRotY,
                         float fRotZ)
{
    m_arRotation[0] = fRotX;
    m_arRotation[1] = fRotY;
    m_arRotation[2] = fRotZ;
}

//*****************************************************************************
// SetScale
//*****************************************************************************
void Matter::SetScale(float fScaleX,
                      float fScaleY,
                      float fScaleZ)
{
    m_arScale[0] = fScaleX;
    m_arScale[1] = fScaleY;
    m_arScale[2] = fScaleZ;
}

//*****************************************************************************
// Translate
//*****************************************************************************
void Matter::Translate(float fTransX,
                       float fTransY,
                       float fTransZ,
                       void* pScene)
{
    float arOldPos[3];
    float arDir[3];
    ListNode* pMatterNode;
    ListNode* pColliderNode;
    Matter* pMatter;
    Collider* pCollider;

    if (m_nPhysical != 0 &&
        m_nMobile   != 0)
    {
        // Record previous position
        arOldPos[0] = m_arPosition[0];
        arOldPos[1] = m_arPosition[1];
        arOldPos[2] = m_arPosition[2];

        // Update the position to the tentative new position
        m_arPosition[0] += fTransX;
        m_arPosition[1] += fTransY;
        m_arPosition[2] += fTransZ;

        // Save the direction of movement in vector-array format
        arDir[0] = fTransX;
        arDir[1] = fTransY;
        arDir[2] = fTransZ;

        // TODO: After implementing Octree, replace this list iteration
        // with the octree iteration.
        // Iterate through all matters
        pMatterNode = reinterpret_cast<Scene*>(pScene)->GetMatterList()->GetHead();

        while (pMatterNode != 0)
        {
            pMatter = reinterpret_cast<Matter*>(pMatterNode->m_pData);
            pMatterNode = pMatterNode->m_pNext;

            //pColliderNode = pMatter->GetColliderList();
        }

    }
    else if (m_nMobile != 0)
    {
        // The object is mobile, so just move it :)
        m_arPosition[0] += fTransX;
        m_arPosition[1] += fTransY;
        m_arPosition[2] += fTransZ;
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
    //if (pOther                != 0 &&
    //    pOther->GetCollider() != 0 &&
    //    m_pCollider           != 0)
    //{
    //    return m_pCollider->Overlaps(pOther->GetCollider(),
    //                                 pOther,
    //                                 this).m_nOverlapping;
    //}
    //else
    //{
    //    return 0;
    //}

    int nOverlapping = 0;
    ListNode* pThisColNode = m_lColliders.GetHead();
    ListNode* pOtherColNode = pOther->m_lColliders.GetHead();

    // This double loop will compare each collider with 
    // The other matter's colliders. It seems like an expensive
    // operation, but most matter has only one collider, so it
    // boils down to only one check. But this could get expensive with
    // let's say one matter that has 3 colliders, and another one that has
    // 4 colliders. That's a total of 12 collision checks.
    // If all the colliders are OBBs... then that might be really
    // expensive. Might have to optimize this part.
    while (pThisColNode != 0)
    {
        while (pOtherColNode != 0)
        {
            nOverlapping = reinterpret_cast<Collider*>(pThisColNode->m_pData)->Overlaps(
                reinterpret_cast<Collider*>(pOtherColNode->m_pData),
                pOther,
                this).m_nOverlapping;

            if (nOverlapping != 0)
            {
                return 1;
            }

            pOtherColNode = pOtherColNode->m_pNext;
        }

        pThisColNode = pThisColNode->m_pNext;
    }

    return 0;
}

//*****************************************************************************
// SetPhysical
//*****************************************************************************
void Matter::SetPhysical(int nPhysical)
{
    m_nPhysical = nPhysical;
}

//*****************************************************************************
// SetMobile
//*****************************************************************************
void Matter::SetMobile(int nMobile)
{
    m_nMobile = nMobile;
}

void Matter::EnableColliderRendering()
{
    m_nRenderColliders = 1;
}

void Matter::DisableColliderRendering()
{
    m_nRenderColliders = 0;
}

int Matter::IsColliderRenderingEnabled()
{
    return m_nRenderColliders;
}

//*****************************************************************************
// IsVisible
//*****************************************************************************
int Matter::IsVisible()
{
    return m_nVisible;
}

//*****************************************************************************
// SetVisible
//*****************************************************************************
void Matter::SetVisible(int nVisible)
{
    m_nVisible = nVisible;
}
