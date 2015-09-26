#ifndef MATTER_H
#define MATTER_H

#include "StaticMesh.h"
#include "AnimatedMesh.h"
#include "SkeletalMesh.h"
#include "Texture.h"
#include "Material.h"
#include "Collider.h"
#include "Matrix.h"

class Matter
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
    //## SetCollider
    //##
    //## Assigns a collider to the matter. A collider is used for detecting
    //## and handling different matters overlapping.
    //##
    //## Input:
    //##   pCollider - pointer to new collider.
    //## **********************************************************************
    void SetCollider(Collider* pCollider);

    //## **********************************************************************
    //## GetCollider
    //##
    //## Assigns a texture to the matter. This texture will be drawn onto
    //## the Matter's mesh based on the UV cooridinates specificed in the mesh's
    //## vertex attributes.
    //##
    //## Returns:
    //##   Collider* - pointer to this Matter's currently assigned collider.
    //## **********************************************************************
    Collider* GetCollider();
    
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
    //## **********************************************************************
    void Translate(float fTransX,
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
    //## SetRigid
    //##
    //## Flag the matter as "rigid". Rigid matter cannot be moved and is
    //## stored in an octree for collision detection purposes.
    //##
    //## Input:
    //##   nRigid - rigid flag.
    //##            '0' to mark as non-rigid.
    //##            '1' to mark as rigid.
    //## **********************************************************************
    void SetRigid(int nRigid);

    //## **********************************************************************
    //## SetVelocity
    //## 
    //## Sets the velocity of the matter. Velocity will cause the position of
    //## the object to change during the UpdatePhysics() function.
    //##
    //## Input:
    //##  fXVel - x component of the velocity in units per second.
    //##  fYVel - y component of the velocity in units per second.
    //##  fZVel - z component of the velocity in units per second.
    //## **********************************************************************
    void SetVelocity(float fXVel,
                     float fYVel,
                     float fZVel);

    //## **********************************************************************
    //## Set*Velocity
    //## 
    //## Sets one component of the matter's velocity (X/Y/Z).
    //## **********************************************************************
    void SetXVelocity(float fXVel);
    void SetYVelocity(float fYVel);
    void SetZVelocity(float fZVel);

    //## **********************************************************************
    //## UpdatePhysics
    //##
    //## Updates the position of the matter based on physical factors. 
    //## Collision detection and handling is performed for physical matters.
    //## Terrain collision and handling is performed for physical matters.
    //##
    //## Input:
    //##   pScene   - pointer to current scene.
    //##   fSeconds - time elapsed in seconds since last frame 
    //## **********************************************************************
    void UpdatePhysics(void* pScene,
                       float fSeconds);

    //## **********************************************************************
    //## GetModelMatrix
    //## 
    //## Generates and returns a pointer to the Matter's model matrix.
    //##
    //## Returns:
    //##   Matrix* - pointer to the matter's model matrix.
    //## **********************************************************************
    Matrix* GetModelMatrix();

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

    //## Generates the model matrix
    void GenerateModelMatrix();

    //## Updates variables reponsible for animation
    void UpdateAnimation();

    //## Visible flag
    int m_nVisible;

    //## Position
    float m_fX;
    float m_fY;
    float m_fZ;

    //## Orientation
    float m_fRotX;
    float m_fRotY;
    float m_fRotZ;

    //## Scale
    float m_fScaleX;
    float m_fScaleY;
    float m_fScaleZ;

    //## Mesh pointer
    Mesh*   m_pMesh;

    //## Material pointer
    Material* m_pMaterial;

    //## Texture pointer
    Texture* m_pTexture;
    
    //## Collider pointer
    Collider* m_pCollider;

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

    //## Physical flag, let's engine know to apply physics on object.
    int m_nPhysical;

    //## Rigid flag, collider should be used for applying physics to 
    //## physical matters. Rigid matters are also added to the octree
    //## upon becoming rigid.
    int m_nRigid;

    //## The velocity of the matter, split into X/Y/Z components
    float m_arVelocity[3];
};

#endif
