#ifndef MATTER_H
#define MATTER_H

#include "Actor.h"
#include "StaticMesh.h"
#include "AnimatedMesh.h"
#include "SkeletalMesh.h"
#include "Texture.h"
#include "Material.h"
#include "Collider.h"
#include "Matrix.h"
#include "List.h"
#include "Box.h"

class Matter : public Actor
{

public:

    //## **********************************************************************
    //## Constructor
    //## **********************************************************************
    Matter();

    //## **********************************************************************
    //## Destructor
    //## **********************************************************************
    ~Matter();

    //## **********************************************************************
    //## SetMesh
    //##
    //## Assigns a mesh to this Matter.
    //##
    //## Input:
    //##   pMesh - pointer to StaticMesh that this matter will use to render.
    //## **********************************************************************
    void SetMesh(Mesh* pMesh);

    //## **********************************************************************
    //## SetMaterial
    //##
    //## Assigns a material to the matter.
    //##
    //## Input:
    //##   pMaterial - pointer to new material.
    //## **********************************************************************
    void SetMaterial(Material* pMaterial);

    //## **********************************************************************
    //## SetTexture
    //##
    //## Assigns a texture to the matter. This texture will be drawn onto
    //## the Matter's mesh based on the UV cooridinates specificed in the mesh's
    //## vertex attributes.
    //##
    //## Input:
    //##   pTexture - pointer to new texture.
    //## **********************************************************************
    void SetTexture(Texture* pTexture);

    //## **********************************************************************
    //## AddCollider
    //##
    //## Assigns a collider to the matter. A collider is used for detecting
    //## and handling different matters overlapping. Multiple colliders
    //## can be assigned to the same matter, and multiple matters can use
    //## the same collider object
    //##
    //## Input:
    //##   pCollider - pointer to new collider.
    //## **********************************************************************
    void AddCollider(Collider* pCollider);

    void RemoveCollider(Collider* pCollider);

    //## **********************************************************************
    //## GetColliderList
    //##
    //## Get the list of all colliders assigned to this mesh
    //##
    //## Returns:
    //##   List* - pointer to the list
    //## **********************************************************************
    List* GetColliderList();
    
    //## **********************************************************************
    //## Render
    //##
    //## Renders the matter.
    //##
    //## Input:
    //##   pScene - pointer to the scene that is rendering this matter.
    //## **********************************************************************
    void Render(void* pScene);

    //## **********************************************************************
    //## RenderColliders
    //##
    //## Renders the matter's attached colliders
    //##
    //## Input:
    //##   pScene - pointer to the scene that is rendering this matter.
    //## **********************************************************************

    void RenderColliders(void* pScene);

    const float* GetPosition();

    //## **********************************************************************
    //## SetPosition
    //##
    //## Sets the matters position in world coordinates.
    //##
    //## Input:
    //##   fX - x position.
    //##   fY - y position.
    //##   fZ - z position.
    //## **********************************************************************
    void SetPosition(float fX,
                     float fY,
                     float fZ);

    const float* GetRotation();

    //## **********************************************************************
    //## SetRotation
    //##
    //## Sets the matter's rotation in degrees.
    //## 
    //## Input:
    //##   fRotX - rotation about x axis in degrees.
    //##   fRotY - rotation about y axis in degrees.
    //##   fRotZ - rotation about z axis in degrees.
    //## **********************************************************************
    void SetRotation(float fRotX,
                     float fRotY,
                     float fRotZ);

    const float* GetScale();

    //## **********************************************************************
    //## SetScale
    //##
    //## Sets the matter's scale.
    //##
    //## Input:
    //##   fScaleX - scaling along x axis.
    //##   fScaleY - scaling along y axis.
    //##   fScaleZ - scaling along z axis.
    //## **********************************************************************
    void SetScale(float fScaleX,
                  float fScaleY,
                  float fScaleZ);

    //## **********************************************************************
    //## Translate
    //##
    //## Translate the matter in respect to its current position.
    //##
    //## Input:
    //##   fTransX - x translation.
    //##   fTransY - y translation.
    //##   fTransZ - z translation.
    //## 
    //## Returns:
    //##   int - 1 if hit a sorted object (if this matter is physical)
    //##         0 otherwise.
    //## **********************************************************************
    int Translate(float fTransX,
                  float fTransY,
                  float fTransZ);

    //## **********************************************************************
    //## IsVisible
    //##
    //## Returns:
    //##   int - '1' if matter is visible and should be rendered.
    //##         '0' if matter is invisible and should not be rendered.
    //## **********************************************************************
    int IsVisible();

    //## **********************************************************************
    //## SetVisible
    //##
    //## Sets the visible flag to let the scene know whether to render the 
    //## matter or not.
    //##
    //## Input:
    //##   nVisible - flag to set matter as visible for rendering.
    //## **********************************************************************
    void SetVisible(int nVisible);

    //## **********************************************************************
    //## SetAnimation
    //##
    //## If the matter has a bound mesh with animation data, then this 
    //## method will change the animation to the one matching the provided
    //## name. A warning will be produced if no matching animation is found.
    //## 
    //## Input:
    //##   pAnimation - name of animation to switch to.
    //## **********************************************************************
    void SetAnimation(const char* pAnimation);

    //## **********************************************************************
    //## PlayAnimationOnce
    //##
    //## Plays an animation just once, returning to whatever animation was 
    //## set previously when finished.
    //## 
    //## Input:
    //##   pAnimation - name of animation to play.
    //## **********************************************************************
    void PlayAnimationOnce(const char* pAnimation);

    //## **********************************************************************
    //## SetLoopMode
    //##
    //## Sets the animation looping mode. Loop mode controls the repetition
    //## of animation while an animation is being played.
    //## 
    //## Loop Modes:
    //## LOOP_ONCE - animation stops on last frame. Must be reset to replay.
    //## LOOP_CYCLE - animation resets to first frame after the last frame.
    //## LOOP_PING_PONG - aniamtion reverses once hitting last frame and 
    //##                  continues forward when reaching the first frame.
    //##
    //## Input:
    //##   nLoopMode - loop mode.
    //## **********************************************************************
    void SetLoopMode(int nLoopMode);

    //## **********************************************************************
    //## StartAnimation
    //## 
    //## Plays the current animation.
    //## **********************************************************************
    void StartAnimation();

    //## **********************************************************************
    //## StopAnimation
    //##
    //## Stops the current animation.
    //## **********************************************************************
    void StopAnimation();

    //## **********************************************************************
    //## ResetAnimation
    //##
    //## Sets the matter's current animation frame to the first frame.
    //## Does not affect whether the animation is playing or stopped.
    //## **********************************************************************
    void ResetAnimation();

    //## **********************************************************************
    //## SetAnimationSpeed
    //##
    //## Changes the speed of animation. Default speed is 1.0f.
    //## 
    //## Input:
    //##   fSpeed - new speed factor.
    //## **********************************************************************
    void SetAnimationSpeed(float fSpeed);

    //## **********************************************************************
    //## Overlaps
    //##
    //## Checks if two Matters overlap based on their colliders. If either
    //## Matter has no collider attached, this function will return 0.
    //## 
    //## Returns:
    //##   int - '1' if the Matters overlap.
    //##       - '0' otherwise.
    //## **********************************************************************
    int Overlaps(Matter* pOther);

    //## **********************************************************************
    //## SetPhysical
    //##
    //## Flag the matter as "physical". Physical objects are affected by 
    //## gravity, collisions with rigid matter, and terrain.
    //## 
    //## Input:
    //##   nPhysical - physical flag. 
    //##               '0' to mark as non-physical. 
    //##               '1' to mark as physical.
    //## **********************************************************************
    void SetPhysical(int nPhysical);

    //## **********************************************************************
    //## Set Mobile
    //##
    //## Flag the matter as "mobile". Mobile matter can be moved. Objects
    //## in environments that will not move should be marked as non-mobile
    //## to improve performance.
    //##
    //## Input:
    //##   nRigid - rigid flag.
    //##            '0' to mark as non-mobile.
    //##            '1' to mark as mobile.
    //## **********************************************************************
    void SetMobile(int nMobile);

    void SetSorted(int nSorted);

    int IsSorted();

    void EnableColliderRendering();
    void DisableColliderRendering();

    int IsColliderRenderingEnabled();

    static void SetGlobalColliderRenderingEnable(int nEnable);
    

    //## **********************************************************************
    //## GetModelMatrix
    //## 
    //## Generates and returns a pointer to the Matter's model matrix.
    //##
    //## Returns:
    //##   Matrix* - pointer to the matter's model matrix.
    //## **********************************************************************
    Matrix* GetModelMatrix();

    Box* GetBoundingBox();

    enum MatterEnum
    {
        LOOP_NONE      = 0,
        LOOP_CYCLE     = 1,
        LOOP_PING_PONG = 2,

        ANIMATION_STOP = 0,
        ANIMATION_PLAY = 1,

        DIRECTION_FORWARD = 0,
        DIRECTION_REVERSE = 1
    };

private:

    static int s_nGlobalColliderRenderingEnable;

    //## Uses collider vertex data to update all-inclusive
    //## bounding box.
    void UpdateBoundingBox();

    //## Generates the model matrix
    void GenerateModelMatrix();

    //## Updates variables reponsible for animation
    void UpdateAnimation();

    //## Called to set point light uniforms in shader program
    void SetPointLightRenderState(void* pScene,
                                  unsigned int hProg);

    //## Visible flag
    int m_nVisible;

    //## Position
    float m_arPosition[3];

    //## Orientation
    float m_arRotation[3];

    //## Scale
    float m_arScale[3];

    //## Mesh pointer
    Mesh*   m_pMesh;

    //## Material pointer
    Material* m_pMaterial;

    //## Texture pointer
    Texture* m_pTexture;
    
    //## Collider list
    List m_lColliders;

    //## Model matrix
    Matrix m_matModel;

    //## Current animation index of the bound mesh
    int m_nCurrentAnimation;

    //## Play-Once animation index.
    int m_nPlayOnceAnimation;

    //## Current frame of animation
    float m_fCurrentFrame;

    //## Animation looping mode (ie. ONCE, CYCLE, PING_PONG)
    int m_nLoopMode;

    //## Flag to indicate whether animation is being updated.
    int m_nPlay;

    //## Direction of animation. (DIRECTION_FORWARD or DIRECTION_REVERSE)
    int m_nDirection;

    //## Speed of animation.
    float m_fSpeed;

    //## Timer to record frame time
    Timer m_timerFrame;

    //## Physical flag to let the engine know that the matter should be
    //## used for collisions when translating a matter
    int m_nPhysical;

    //## A flag to let the engine know that the matter can move, and
    //## should not be placed in the static collision octree.
    int m_nMobile;

    //## Flag to enable collider rendering, usually for debugging
    //## purposes. Rendering color is determined by the collider object.
    int m_nRenderColliders;

    //## Master bounding box
    Box m_box;

    //## Sorting flag. If set, matter will be stored in octree when
    //## being added to a scene.
    int m_nSorted;
};

#endif
