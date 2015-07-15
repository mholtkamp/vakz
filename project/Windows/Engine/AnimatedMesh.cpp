#include "AnimatedMesh.h"
#include "VGL.h"
#include "MeshLoader.h"
#include "Log.h"

AnimatedMesh::AnimatedMesh()
{
    // Overwrite the type
    m_nType                 = MESH_ANIMATED;

    m_arAnimationNames      = 0;
    m_nAnimationCount       = 0;
    m_arKeyFrameCounts      = 0;
    m_arKeyFrameStarts      = 0;
    m_arFaces               = 0;
    m_arVBO                 = 0;
    m_nCurrentAnimation     = 0;
    m_fCurrentFrame         = 0.0f;
    m_nLoopMode             = LOOP_NONE;
    m_nFramesPerSecond      = DEFUALT_FPS;
    m_nPlay                 = 0;
    m_nDirection            = DIRECTION_FORWARD;
    m_fSpeed                = 1.0f;
}

AnimatedMesh::~AnimatedMesh()
{
    int i = 0;
    int j = 0;

    // Destroy video memory
    for (i = 0; i < m_nAnimationCount; i++)
    {
        for (j = 0; j < m_arKeyFrameCounts[i]; j++)
        {
            glDeleteBuffers(j, m_arVBO[i]);
        }
    }
    
    // Destroy name array
    if (m_arAnimationNames != 0)
    {
        for (i = 0; i < m_nAnimationCount; i++)
        {
            if(m_arAnimationNames[i] != 0)
            {
                delete [] m_arAnimationNames[i];
                m_arAnimationNames[i] = 0;
            }
        }

        delete [] m_arAnimationNames;
        m_arAnimationNames = 0;
    }

    // Destroy Keyframe Counts array
    if (m_arKeyFrameCounts != 0)
    {
        delete [] m_arKeyFrameCounts;
        m_arKeyFrameCounts = 0;
    }

    // Destroy KeyFrame Start array
    if (m_arKeyFrameStarts != 0)
    {
        for (i = 0; i < m_nAnimationCount; i++)
        {
            if (m_arKeyFrameStarts[i] != 0)
            {
                delete [] m_arKeyFrameStarts[i];
                m_arKeyFrameStarts[i] = 0;
            }
        }

        delete [] m_arKeyFrameStarts;
        m_arKeyFrameStarts = 0;
    }

    // Destroy Faces array
    if (m_arFaces != 0)
    {
        for (i = 0; i < m_nAnimationCount; i++)
        {
            if (m_arFaces[i] != 0)
            {
                delete [] m_arFaces[i];
                m_arFaces[i] = 0;
            }
        }

        delete [] m_arFaces;
        m_arFaces = 0;
    }

    // Destroy VBO array
    if (m_arVBO != 0)
    {
        for (i = 0; i < m_nAnimationCount; i++)
        {
            if (m_arVBO[i] != 0)
            {
                delete [] m_arVBO[i];
                m_arVBO[i] = 0;
            }
        }

        delete [] m_arVBO;
        m_arVBO = 0;
    }
}

void AnimatedMesh::SetRenderState(void* pScene,
                                  unsigned int hProg)
{
    int   i                    = 0;
    float fElapsedTime         = 0.0f;
    int   nKeyFrames           = m_arKeyFrameCounts[m_nCurrentAnimation];
    float fLastFrame           = static_cast<float>(m_arKeyFrameStarts[m_nCurrentAnimation][nKeyFrames - 1]);
    int   nKeyFrame1           = 0;
    int   nKeyFrame2           = 0;
    int* arStarts              = m_arKeyFrameStarts[m_nCurrentAnimation];
    float fMix                 = 0.0f;

    // Shader handles
    int   hMix                 = -1;
    int   hPosition1           = -1;
    int   hTexCoord1           = -1;
    int   hNormal1             = -1;
    int   hPosition2           = -1;
    int   hTexCoord2           = -1;
    int   hNormal2             = -1;

    if(m_nPlay != 0)
    {
        // Get the time since last render
        m_timerFrame.Stop();
        fElapsedTime = m_timerFrame.Time();
        
        // Reset the timer to record next frame's time
        m_timerFrame.Start();

        if (m_nLoopMode == LOOP_NONE)
        {
            if (static_cast<int>(m_fCurrentFrame) < fLastFrame)
            {
                // If the animation hasn't ended, increase the frame count.
                m_fCurrentFrame += fElapsedTime * 
                                   static_cast<float>(m_nFramesPerSecond) * 
                                   m_fSpeed;
            }
        }
        else if (m_nLoopMode == LOOP_CYCLE)
        {
            // Increase the framecount
            m_fCurrentFrame += fElapsedTime * 
                               static_cast<float>(m_nFramesPerSecond) * 
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
                                   static_cast<float>(m_nFramesPerSecond) *
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
                                   static_cast<float>(m_nFramesPerSecond) *
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

        // Now that we know the current frame, we can find the two keyframes
        // that the frame is between.
        for (i = 0; i < m_arKeyFrameCounts[m_nCurrentAnimation]; i++)
        {
            if (static_cast<int>(m_fCurrentFrame) < arStarts[i])
            {
                nKeyFrame1 = i - 1;
                nKeyFrame2 = i;
                break;
            }
        }

        if (i == m_arKeyFrameCounts[m_nCurrentAnimation])
        {
            // If the current frame is higher than
            // the last keyframe's framestart, then it isn't
            // higher by much. Just set the 2nd keyframe to the
            // last keyframe of this animation. 
            nKeyFrame1 = i - 2;
            nKeyFrame2 = i - 1;
        }

        // Calculate the mix factor (that will be sent to vertex shader).
        fMix = (m_fCurrentFrame - static_cast<float>(arStarts[nKeyFrame1])) / 
               static_cast<float>(arStarts[nKeyFrame2] - arStarts[nKeyFrame1]);

        // Get the uniform and attribute locations
        hMix = glGetUniformLocation(hProg, "uMix");
        hPosition1 = glGetAttribLocation(hProg, "aPosition1");
        hTexCoord1 = glGetAttribLocation(hProg, "aTexCoord1");
        hNormal1   = glGetAttribLocation(hProg, "aNormal1");
        hPosition2 = glGetAttribLocation(hProg, "aPosition2");
        hTexCoord2 = glGetAttribLocation(hProg, "aTexCoord2");
        hNormal2   = glGetAttribLocation(hProg, "aNormal2");

        // Set the mix uniform
        glUniform1f(hMix, fMix);

        // Enable vertex attribute arrays
        glEnableVertexAttribArray(hPosition1);
        glEnableVertexAttribArray(hTexCoord1);
        glEnableVertexAttribArray(hNormal1);
        glEnableVertexAttribArray(hPosition2);
        glEnableVertexAttribArray(hTexCoord2);
        glEnableVertexAttribArray(hNormal2);

        // Set the vertex attributes for first keyframe
        // The proper VBO needs to be bound first.
        glBindBuffer(GL_ARRAY_BUFFER, m_arVBO[m_nCurrentAnimation][nKeyFrame1]);

        glVertexAttribPointer(hPosition1,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              8 * sizeof(float),
                              0);
        glVertexAttribPointer(hTexCoord1,
                              2,
                              GL_FLOAT,
                              GL_FALSE,
                              8 * sizeof(float),
                              (void*) (3 * sizeof(float)));
        glVertexAttribPointer(hNormal1,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              8 * sizeof(float),
                              (void*) (5 * sizeof(float)));

        // Now set the vertex arrays for the second keyframe
        glBindBuffer(GL_ARRAY_BUFFER, m_arVBO[m_nCurrentAnimation][nKeyFrame2]);

        glVertexAttribPointer(hPosition2,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              8 * sizeof(float),
                              0);
        glVertexAttribPointer(hTexCoord2,
                              2,
                              GL_FLOAT,
                              GL_FALSE,
                              8 * sizeof(float),
                              (void*) (3 * sizeof(float)));
        glVertexAttribPointer(hNormal2,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              8 * sizeof(float),
                              (void*) (5 * sizeof(float)));
    }
}

void AnimatedMesh::Load(const char* pFileName)
{
    if (pFileName != 0)
    {
        MeshLoader::LoadAMF(pFileName,
                            &m_arAnimationNames,
                            &m_nAnimationCount,
                            &m_nFramesPerSecond,
                            &m_arKeyFrameCounts,
                            &m_arKeyFrameStarts,
                            &m_arFaces,
                            &m_arVBO);

        // Every face has 3 vertices.
        m_nVertexCount = m_arFaces[0][0] * 3;
    }
}

void AnimatedMesh::SetAnimation(const char* pAnimation)
{
    int i = 0;

    for(i = 0; i < m_nAnimationCount; i++)
    {
        if(strcmp(pAnimation, m_arAnimationNames[i]) == 0)
        {
            m_nCurrentAnimation = i;
            m_fCurrentFrame = 0.0f;
            return;
        }
    }

    LogWarning("No matching animation found in AnimatedMesh::SetAnimation().");
}

void AnimatedMesh::SetLoopMode(int nLoopMode)
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

void AnimatedMesh::StartAnimation()
{
    m_nPlay = 1;
    m_timerFrame.Start();
}

void AnimatedMesh::StopAnimation()
{
    m_nPlay = 0;
    m_timerFrame.Stop();
}

void AnimatedMesh::ResetAnimation()
{
    m_fCurrentFrame = 0.0f;
}

void AnimatedMesh::SetSpeed(float fSpeed)
{
    m_fSpeed = fSpeed;
}