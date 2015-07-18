#ifndef ANIMATED_MESH_H
#define ANIMATED_MESH_H

#include "Mesh.h"
#include "Timer.h"

class AnimatedMesh : public Mesh
{

public:

    //## **********************************************************************
    //## Constructor
    //## **********************************************************************
    AnimatedMesh();

    //## **********************************************************************
    //## Destructor
    //## **********************************************************************
    ~AnimatedMesh();

    //## **********************************************************************
    //## SetRenderState
    //##
    //## Sets up the OpenGL state so that an AnimatedMesh can be rendered.
    //## Uploads the "mix" value to the shader based on the provided animation
    //## and frame. Assigns attribute arrays for the two keyframes that will
    //## be interpolated in the vertex shader.
    //##
    //## Input:
    //##   pScene          - pointer to scene being rendered.
    //##   hProg           - handle to pre-bound shader program
    //##   nAnimationIndex - animation identifier.
    //##   fCurrentFrame   - the frame of the animation to render.
    //## **********************************************************************
    void SetRenderState(void*        pScene,
                        unsigned int hProg,
                        int          nAnimationIndex,
                        float        fCurrentFrame);

    //## **********************************************************************
    //## Load
    //##
    //## Opens an animated mesh from a .AMF file.
    //##
    //## Input:
    //##   pFileName - path to amf file.
    //## **********************************************************************
    void Load(const char* pFileName);

    //## **********************************************************************
    //## GetAnimationIndex
    //## 
    //## Gets an animation index based on the matching animation name.
    //##
    //## Input:
    //##   pAnimation - name of animation.
    //## **********************************************************************
    int GetAnimationIndex(const char* pAnimation);

    //## **********************************************************************
    //## GetKeyFrameCount
    //## 
    //## Returns the number of keyframes contained in an animation.
    //##
    //## Input:
    //##   nAnimationIndex - animation index of interest.
    //##                     (see GetAnimationIndex()).
    //##
    //## Returns:
    //##   int - number of keyframes contained in animation.
    //## **********************************************************************
    int GetKeyFrameCount(int nAnimationIndex);

    //## **********************************************************************
    //## GetLastFrame
    //##
    //## Returns the index of the last frame in the specified animation.
    //##
    //## Input:
    //##   nAnimationIndex - animation index of interest.
    //## 
    //## Returns:
    //##   int - last keyframe index
    //## **********************************************************************
    int GetLastFrame(int nAnimationIndex);

    //## **********************************************************************
    //## GetFramesPerSecond
    //##
    //## Returns the framerate described in the loaded AMF file.
    //##
    //## Returns:
    //##   int - frames per second
    //## **********************************************************************
    int GetFramesPerSecond();

    enum AnimatedMeshEnum
    {
        DEFUALT_FPS    = 60
    };

private:
    
    //## An array of names for each animation
    char** m_arAnimationNames;

    //## Total number of animations
    int m_nAnimationCount;

    //## Array of the number of keyframes per animation
    int* m_arKeyFrameCounts;

    //## Array of the starting frames for each keyframe for each animation
    int** m_arKeyFrameStarts;
    
    //## Array of face count for each keyframe per animation.
    unsigned int** m_arFaces;

    //## Array of VBOs for each keyframe per animation.
    unsigned int** m_arVBO;

    //## The frames per second the animations were meant to be 
    //## played at.
    int m_nFramesPerSecond;
};

#endif
