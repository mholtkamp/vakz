#include "AnimatedMesh.h"
#include "VGL.h"
#include "MeshLoader.h"

AnimatedMesh::AnimatedMesh()
{
    m_arAnimationNames      = 0;
    m_nAnimationCount       = 0;
    m_arKeyFrameCounts      = 0;
    m_arKeyFrameStarts      = 0;
    m_arFaces               = 0;
    m_arVBO                 = 0;
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

}

void AnimatedMesh::Load(const char* pFileName)
{
    if (pFileName != 0)
    {
        MeshLoader::LoadAMF(pFileName,
                            &m_arAnimationNames,
                            &m_nAnimationCount,
                            &m_arKeyFrameCounts,
                            &m_arKeyFrameStarts,
                            &m_arFaces,
                            &m_arVBO);
    }
}
