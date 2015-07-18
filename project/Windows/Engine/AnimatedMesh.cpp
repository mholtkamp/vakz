#include "AnimatedMesh.h"
#include "VGL.h"
#include "MeshLoader.h"
#include "Log.h"
#include <string.h>
#include <stdio.h>

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
    m_nFramesPerSecond      = DEFUALT_FPS;
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

void AnimatedMesh::SetRenderState(void*        pScene,
                                  unsigned int hProg,
                                  int          nAnimationIndex,
                                  float        fCurrentFrame)
{
    int   i                    = 0;
    float fElapsedTime         = 0.0f;
    int   nKeyFrames           = m_arKeyFrameCounts[nAnimationIndex];
    float fLastFrame           = static_cast<float>(m_arKeyFrameStarts[nAnimationIndex][nKeyFrames - 1]);
    int   nKeyFrame1           = 0;
    int   nKeyFrame2           = 0;
    int* arStarts              = m_arKeyFrameStarts[nAnimationIndex];
    float fMix                 = 0.0f;

    // Shader handles
    int   hMix                 = -1;
    int   hPosition1           = -1;
    int   hTexCoord1           = -1;
    int   hNormal1             = -1;
    int   hPosition2           = -1;
    int   hTexCoord2           = -1;
    int   hNormal2             = -1;


    // Now that we know the current frame, we can find the two keyframes
    // that the frame is between.
    for (i = 0; i < m_arKeyFrameCounts[nAnimationIndex]; i++)
    {
        if (static_cast<int>(fCurrentFrame) < arStarts[i])
        {
            nKeyFrame1 = i - 1;
            nKeyFrame2 = i;
            break;
        }
    }

    if (i == m_arKeyFrameCounts[nAnimationIndex])
    {
        // If the current frame is higher than
        // the last keyframe's framestart, then it isn't
        // higher by much. Just set the 2nd keyframe to the
        // last keyframe of this animation. 
        nKeyFrame1 = i - 2;
        nKeyFrame2 = i - 1;
    }

    // Calculate the mix factor (that will be sent to vertex shader).
    fMix = (fCurrentFrame - static_cast<float>(arStarts[nKeyFrame1])) / 
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
    glBindBuffer(GL_ARRAY_BUFFER, m_arVBO[nAnimationIndex][nKeyFrame1]);

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
    glBindBuffer(GL_ARRAY_BUFFER, m_arVBO[nAnimationIndex][nKeyFrame2]);

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

int AnimatedMesh::GetAnimationIndex(const char* pAnimation)
{
    int i = 0;

    for(i = 0; i < m_nAnimationCount; i++)
    {
        if(strcmp(pAnimation, m_arAnimationNames[i]) == 0)
        {
            return i;
        }
    }

    return -1;
}

int AnimatedMesh::GetKeyFrameCount(int nAnimationIndex)
{
    if (nAnimationIndex >= 0 &&
        nAnimationIndex <  m_nAnimationCount)
    {
        return m_arKeyFrameCounts[nAnimationIndex];
    }
    else
    {
        return 0;
    }
}

int AnimatedMesh::GetLastFrame(int nAnimationIndex)
{
    int nKeyFrames = GetKeyFrameCount(nAnimationIndex);

    if (nAnimationIndex >= 0 &&
        nAnimationIndex <  m_nAnimationCount)
    {
        return m_arKeyFrameStarts[nAnimationIndex][nKeyFrames - 1];
    }

    return 0;
}

int AnimatedMesh::GetFramesPerSecond()
{
    return m_nFramesPerSecond;
}
