#ifndef ANIMATED_MESH_H
#define ANIMATED_MESH_H

#include "Mesh.h"

class AnimatedMesh : public Mesh
{

public:

    AnimatedMesh();

    ~AnimatedMesh();

    void SetRenderState(void*        pScene,
                        unsigned int hProg);

    void Load(const char* pFileName);

private:
    
    char** m_arAnimationNames;

    unsigned int m_nAnimationCount;

    unsigned int* m_arKeyFrameCounts;

    unsigned int** m_arKeyFrameStarts;
    
    unsigned int** m_arFaces;

    unsigned int** m_arVBO;
};

#endif
