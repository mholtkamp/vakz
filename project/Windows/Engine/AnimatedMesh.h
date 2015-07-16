#ifndef ANIMATED_MESH_H
#define ANIMATED_MESH_H

#include "Mesh.h"
#include "Timer.h"

class AnimatedMesh : public Mesh
{

public:

    AnimatedMesh();

    ~AnimatedMesh();

    void SetRenderState(void*        pScene,
                        unsigned int hProg,
                        int          nAnimationIndex,
                        float        fCurrentFrame);

    void Load(const char* pFileName);

    int GetAnimationIndex(const char* pAnimation);

    int GetKeyFrameCount(int nAnimationIndex);

    int GetLastFrame(int nAnimationIndex);

    int GetFramesPerSecond();

    enum AnimatedMeshEnum
    {
        DEFUALT_FPS    = 60
    };

private:
    
    char** m_arAnimationNames;

    int m_nAnimationCount;

    int* m_arKeyFrameCounts;

    int** m_arKeyFrameStarts;
    
    unsigned int** m_arFaces;

    unsigned int** m_arVBO;

    int m_nFramesPerSecond;
};

#endif
