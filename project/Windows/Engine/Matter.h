#ifndef MATTER_H
#define MATTER_H

#include "StaticMesh.h"
#include "AnimatedMesh.h"
#include "SkeletalMesh.h"
#include "Texture.h"
#include "Material.h"

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

    void Render();

private:

    StaticMesh*   m_pStaticMesh;
    AnimatedMesh* m_pAnimatedMesh;
    SkeletalMesh* m_pSkeletalMesh;
};

#endif
