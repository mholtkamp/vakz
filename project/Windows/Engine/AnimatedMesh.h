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
                        unsigned int hProg);

    void Load(const char* pFileName);

    void SetAnimation(const char* pAnimation);

    void SetLoopMode(int nLoopMode);

    void StartAnimation();

    void StopAnimation();

    void ResetAnimation();

    void SetSpeed(float fSpeed);

    enum AnimatedMeshEnum
    {
        LOOP_NONE      = 0,
        LOOP_CYCLE     = 1,
        LOOP_PING_PONG = 2,

        DEFUALT_FPS    = 60,

        ANIMATION_STOP = 0,
        ANIMATION_PLAY = 1,

        DIRECTION_FORWARD = 0,
        DIRECTION_REVERSE = 1
    };

private:
    
    char** m_arAnimationNames;

    int m_nAnimationCount;

    int* m_arKeyFrameCounts;

    int** m_arKeyFrameStarts;
    
    unsigned int** m_arFaces;

    unsigned int** m_arVBO;

    int m_nCurrentAnimation;

    float m_fCurrentFrame;

    int m_nLoopMode;

    int m_nFramesPerSecond;

    int m_nPlay;

    int m_nDirection;

    float m_fSpeed;

    Timer m_timerFrame;
};

#endif
