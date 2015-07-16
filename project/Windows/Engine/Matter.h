#ifndef MATTER_H
#define MATTER_H

#include "StaticMesh.h"
#include "AnimatedMesh.h"
#include "SkeletalMesh.h"
#include "Texture.h"
#include "Material.h"
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

    void SetAnimation(const char* pAnimation);

    void SetLoopMode(int nLoopMode);

    void StartAnimation();

    void StopAnimation();

    void ResetAnimation();

    void SetAnimationSpeed(float fSpeed);

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

    //## Model matrix
    Matrix m_matModel;

    int m_nCurrentAnimation;

    float m_fCurrentFrame;

    int m_nLoopMode;

    int m_nPlay;

    int m_nDirection;

    float m_fSpeed;

    Timer m_timerFrame;
};

#endif
