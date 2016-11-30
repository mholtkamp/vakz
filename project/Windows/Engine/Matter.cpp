#include "Matter.h"
#include "Log.h"
#include "VGL.h"
#include "Camera.h"
#include "Scene.h"
#include "VMath.h"
#include "Octree.h"
#include "PointLight.h"
#include <stdio.h>

#define COL_BUFFER 0.001f
#define MATTER_MAX_POINT_LIGHTS 3
#define MATTER_MAX_DIRECTIONAL_LIGHTS 1

int Matter::s_nGlobalColliderRenderingEnable = 1;

//*****************************************************************************
// Constructor
//*****************************************************************************
Matter::Matter()
{
    m_nClass = ACTOR_MATTER;

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
    m_nSorted   = 0;
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
    Scene* pScene = reinterpret_cast<Scene*>(m_pScene);

    if (pScene    != 0 &&
        m_nSorted != 0)
    {
        // Adding a collider to a matter that is already part of
        // a scene, means it must be replaced into the octree

        // First remove it from the octree
        pScene->GetMatterOctree()->Remove(this, m_box);

        // Update the bounding box, this will take into consideration
        // the newly added collider now.
        UpdateBoundingBox();

        // Re-insert to octree with new bounds
        pScene->GetMatterOctree()->Add(this, m_box);
    }
    else
    {
        UpdateBoundingBox();
    }
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
    int               hMatrixModel  = -1;
    int               hMatrixNormal = -1;
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
                                      hProg);
            glUniform1i(glGetUniformLocation(hProg, "uDirLightOn"), 1);
        }
        else
        {
            // Disable directional light in shader
            glUniform1i(glGetUniformLocation(hProg, "uDirLightOn"), 0);

            float arZeroColor[] = {0.0f, 0.0f, 0.0f};
            glUniform3fv(glGetUniformLocation(hProg, "uDirLightColor"), 1, arZeroColor);
        }

        // Setup point lights
        SetPointLightRenderState(pScene,
                                 hProg);
        
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

            // Uploade the model matrix (for lighting)
            hMatrixModel = glGetUniformLocation(hProg, "uMatrixModel");
            glUniformMatrix4fv(hMatrixModel, 1, GL_FALSE, m_matModel.GetArray());

            // Create the normal matrix
            m_matModel.Inverse();
            m_matModel.Transpose();

            // Upload normal matrix to shader for normal transformations
            hMatrixNormal = glGetUniformLocation(hProg, "uMatrixNormal");
            glUniformMatrix4fv(hMatrixNormal, 1, GL_FALSE, m_matModel.GetArray());
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
        if (s_nGlobalColliderRenderingEnable != 0 &&
            m_nRenderColliders != 0)
        {
            ListNode* pNode = m_lColliders.GetHead();

            // Setup state for rendering translucent collision meshes
            glDepthMask(GL_FALSE);
            glDisable(GL_CULL_FACE);

            while (pNode != 0)
            {
                reinterpret_cast<Collider*>(pNode->m_pData)->Render(this, pScene, &matMVP);
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

    UpdateBoundingBox();
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

    UpdateBoundingBox();
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
    UpdateBoundingBox();
}

//*****************************************************************************
// Translate
//*****************************************************************************
int Matter::Translate(float fTransX,
                      float fTransY,
                      float fTransZ)
{
    int i = 0;
    int nRet = 0;
    float arOldPos[3] = {0.0f};
    float arDir[3] = {0.0f};

    OverlapResult orResult;
    ListNode* pMatterNode = 0;
    ListNode* pColliderNode = 0;
    ListNode* pThisColliderNode = 0;
    Matter* pMatter = 0;
    Collider* pCollider = 0;
    Collider* pThisCollider = 0;

    if (m_pScene == 0)
    {
        return 0;
    }

    // If this object needs to be sorted, first remove it
    // from the octree because its position will be updated
    if (m_nSorted != 0)
    {
        reinterpret_cast<Scene*>(m_pScene)->GetMatterOctree()->Remove(this, m_box);
    }
    

    // Save the direction of movement in vector-array format
    arDir[0] = fTransX;
    arDir[1] = fTransY;
    arDir[2] = fTransZ;

    // Record previous position
    arOldPos[0] = m_arPosition[0];
    arOldPos[1] = m_arPosition[1];
    arOldPos[2] = m_arPosition[2];

    // Update the position to the tentative new position
    m_arPosition[0] += fTransX;
    m_arPosition[1] += fTransY;
    m_arPosition[2] += fTransZ;

    UpdateBoundingBox();

    if (m_nPhysical != 0 &&
        m_nMobile   != 0)
    {
        //@@ DEBUG: for testing non-octree, use this linked list instead
        //pMatterNode = reinterpret_cast<Scene*>(m_pScene)->GetMatterList()->GetHead();
        
        List lNearbyMatter;
        reinterpret_cast<Scene*>(m_pScene)->GetNearbyMatter(this, lNearbyMatter);
        pMatterNode = lNearbyMatter.GetHead();

        // Loop through the scene matter list
        while (pMatterNode != 0)
        {
            pMatter = reinterpret_cast<Matter*>(pMatterNode->m_pData);
            pMatterNode = pMatterNode->m_pNext;

            // Do not check collision with self...
            if (pMatter == this)
            {
                continue;
            }

            pColliderNode = pMatter->GetColliderList()->GetHead();
            
            // Loop though the other matter's colliders
            while (pColliderNode != 0)
            {
                pCollider = reinterpret_cast<Collider*>(pColliderNode->m_pData);
                pColliderNode = pColliderNode->m_pNext;

                pThisColliderNode = GetColliderList()->GetHead();

                // Loop through this matter's colliders
                while (pThisColliderNode != 0)
                {
                    pThisCollider = reinterpret_cast<Collider*>(pThisColliderNode->m_pData);
                    pThisColliderNode = pThisColliderNode->m_pNext;

                    // Perform the collision detection.
                    orResult = pThisCollider->Overlaps(pCollider, pMatter, this);

                    if (orResult.m_nOverlapping != 0)
                    {
                        // Flag the translate return as true.
                        nRet = 1;

                        // Must push back the matter along least penetration axis.
                        // First add a small extra buffer to the pen depth to make sure
                        // that the collider is no longer overlapping
                        orResult.m_fOverlapDepth += COL_BUFFER;

                        // No scale the unit-length axis vector by the overlap depth
                        orResult.m_arLeastPenAxis[0] *= orResult.m_fOverlapDepth;
                        orResult.m_arLeastPenAxis[1] *= orResult.m_fOverlapDepth;
                        orResult.m_arLeastPenAxis[2] *= orResult.m_fOverlapDepth;

                        // Now add the scaled vector to m_arPosition
                        m_arPosition[0] += orResult.m_arLeastPenAxis[0];
                        m_arPosition[1] += orResult.m_arLeastPenAxis[1];
                        m_arPosition[2] += orResult.m_arLeastPenAxis[2];

                        UpdateBoundingBox();
                    }
                }
            }
        }
    }

    if (m_nSorted != 0)
    {
        reinterpret_cast<Scene*>(m_pScene)->GetMatterOctree()->Add(this, m_box);
    }

    return nRet;
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
// GetBoundingBox
//*****************************************************************************
Box* Matter::GetBoundingBox()
{
    return &m_box;
}

//*****************************************************************************
// UpdateBoundingBox
//*****************************************************************************
void Matter::UpdateBoundingBox()
{
    Collider* pCollider = 0;
    
    // Hold the collective min/max
    float arMin[3] = {0.0f};
    float arMax[3] = {0.0f};

    // Hold the min and max for a specific collider
    float arColMin[3] = {0.0f};
    float arColMax[3] = {0.0f};

    // Iterate over all colliders to find collective bounds
    ListNode* pNode = m_lColliders.GetHead();
    while (pNode != 0)
    {
        pCollider =  reinterpret_cast<Collider*>(pNode->m_pData);

        pCollider->GetBounds(this, arColMin, arColMax);

        // If this is the first collider being examined,
        // then just set the collective min/max to its min/max
        if (pNode == m_lColliders.GetHead())
        {
            arMin[0] = arColMin[0];
            arMin[1] = arColMin[1];
            arMin[2] = arColMin[2];

            arMax[0] = arColMax[0];
            arMax[1] = arColMax[1];
            arMax[2] = arColMax[2];
        }
        else
        {
            // Check mins
            if (arColMin[0] < arMin[0])
                arMin[0] = arColMin[0];
            if (arColMin[1] < arMin[1])
                arMin[1] = arColMin[1];
            if (arColMin[2] < arMin[2])
                arMin[2] = arColMin[2];

            // Check maxes
             if (arColMax[0] < arMax[0])
                arMax[0] = arColMax[0];
            if (arColMax[1] < arMax[1])
                arMax[1] = arColMax[1];
            if (arColMax[2] < arMax[2])
                arMax[2] = arColMax[2];
        }

        // Iterate listnode
        pNode = pNode->m_pNext;
    }

    // Now that mins and maxes have been found, update the m_box object.
    m_box.m_arCenter[0] = (arMin[0] + arMax[0])/2.0f;
    m_box.m_arCenter[1] = (arMin[1] + arMax[1])/2.0f;
    m_box.m_arCenter[2] = (arMin[2] + arMax[2])/2.0f;

    m_box.m_arExtent[0] = (arMax[0] - arMin[0])/2.0f;
    m_box.m_arExtent[1] = (arMax[1] - arMin[1])/2.0f;
    m_box.m_arExtent[2] = (arMax[2] - arMin[2])/2.0f;
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

void Matter::SetSorted(int nSorted)
{
    m_nSorted = nSorted;
}

int Matter::IsSorted()
{
    return m_nSorted;
}

void Matter::SetPointLightRenderState(void*        pScene,
                                      unsigned int hProg)
{
    int i = 0;

    int hLightPositions   = glGetUniformLocation(hProg, "uPointLightPositions");
    int hLightColors      = glGetUniformLocation(hProg, "uPointLightColors");
    int hLightRadii       = glGetUniformLocation(hProg, "uPointLightRadii");
    int hLightIntensities = glGetUniformLocation(hProg, "uPointLightIntensities");
    int hNumLights        = glGetUniformLocation(hProg, "uNumPointLights");

    float fDist    = 0.0f;
    float fMaxDist = 0.0f;
    int nFarthestLight = 0;

    PointLight* arPointLights[MATTER_MAX_POINT_LIGHTS] = {0,0,0};
    float arLightPositions[MATTER_MAX_POINT_LIGHTS * 3] = {0.0f};
    float arLightColors[MATTER_MAX_POINT_LIGHTS * 3] = {0.0f};
    float arLightIntensities[MATTER_MAX_POINT_LIGHTS] = {0.0f};
    float arLightRadii[MATTER_MAX_POINT_LIGHTS] = {0.0f};

    int nNumLights = 0;

    // Get nearby point lights from scene's pointlight octree
    List lPointLights;
    reinterpret_cast<Scene*>(pScene)->GetNearbyPointLights(this, lPointLights);

    // Iterate through list and choose the 3 closest point lights
    ListNode* pNode = lPointLights.GetHead();
    PointLight* pCurLight = 0;

    while (pNode != 0)
    {
        pCurLight = reinterpret_cast<PointLight*>(pNode->m_pData);

        if (nNumLights < MATTER_MAX_POINT_LIGHTS)
        {
            // Haven't found 3 lights yet, so just add this to the array
            arPointLights[nNumLights] = pCurLight;

            fDist = DistanceBetweenPoints(pCurLight->GetPosition(),
                                          m_arPosition);

            // Record the maximum distance
            if (fDist >= fMaxDist)
            {
                fMaxDist       = fDist;
                nFarthestLight = nNumLights;
            }
            // Increment light count
            nNumLights++;
        }
        else
        {
            // 3 lights have been found. Check if this one is closer
            fDist = DistanceBetweenPoints(pCurLight->GetPosition(),
                                          m_arPosition);

            if (fDist < fMaxDist)
            {
                // This light is closer than the farthest light on record.
                // Replace the farthest light with this one
                arPointLights[nFarthestLight] = pCurLight;

                fMaxDist = 0.0f;

                // Iterate through array and find the new farthest light
                for (i = 0; i < MATTER_MAX_POINT_LIGHTS; i++)
                {
                    fDist = DistanceBetweenPoints(arPointLights[i]->GetPosition(), m_arPosition);

                    if (fDist >= fMaxDist)
                    {
                        fMaxDist = fDist;
                        nFarthestLight = i;
                    }
                }
            }
        }

        pNode = pNode->m_pNext;
    }

    // Setup pos/color arrays
    for (i = 0; i < nNumLights; i++)
    {
        float* arLightPos = arPointLights[i]->GetPosition();
        float* arLightCol = arPointLights[i]->GetColor();

        arLightPositions[i*3 + 0] = arLightPos[0];
        arLightPositions[i*3 + 1] = arLightPos[1];
        arLightPositions[i*3 + 2] = arLightPos[2];

        arLightColors[i*3 + 0] = arLightCol[0];
        arLightColors[i*3 + 1] = arLightCol[1];
        arLightColors[i*3 + 2] = arLightCol[2];

        arLightIntensities[i] = arPointLights[i]->GetIntensity();
        arLightRadii[i]       = arPointLights[i]->GetRadius();
    }

    // Now time to actually set the shader uniforms
    glUniform3fv(hLightPositions, 3, arLightPositions);
    glUniform3fv(hLightColors, 3, arLightColors);
    glUniform1fv(hLightIntensities, 3, arLightIntensities);
    glUniform1fv(hLightRadii, 3, arLightRadii);
    glUniform1i(hNumLights, nNumLights);
}

void Matter::SetGlobalColliderRenderingEnable(int nEnable)
{
    s_nGlobalColliderRenderingEnable = nEnable;
}