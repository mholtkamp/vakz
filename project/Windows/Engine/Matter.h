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
    //## SetStaticMesh
    //##
    //## Assigns a StaticMesh to this Matter. Clears any AnimatedMesh or
    //## SkeletalMesh that has already been assigned to this Matter.
    //##
    //## Input:
    //##   pMesh - pointer to StaticMesh that this matter will use to render.
    //## **********************************************************************
    void SetStaticMesh(StaticMesh* pMesh);

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
    //## Render
    //##
    //## Renders the matter.
    //## **********************************************************************
    void Render(void* pCamera);

    void SetPosition(float fX,
                     float fY,
                     float fZ);

    void SetRotation(float fRotX,
                     float fRotY,
                     float fRotZ);

    void SetScale(float fScaleX,
                  float fScaleY,
                  float fScaleZ);

private:

    //## Generates the model matrix
    void GenerateModelMatrix();

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

    //## Mesh pointers
    StaticMesh*   m_pStaticMesh;
    AnimatedMesh* m_pAnimatedMesh;
    SkeletalMesh* m_pSkeletalMesh;

    //## Material pointer
    Material* m_pMaterial;

    //## Model matrix
    Matrix m_matModel;
};

#endif
