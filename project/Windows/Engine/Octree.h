#ifndef OCTREE_H
#define OCTREE_H

#include "Box.h"
#include "List.h"

class OctreeNode
{
public:
    OctreeNode();
    ~OctreeNode();

    OctreeNode* m_pParent;
    OctreeNode* m_arChildren[8];
    unsigned char m_ucActiveNodes;
    Box m_bRegion;
    List m_lObjects;
};

class Octree
{
    Octree();
    ~Octree();

    void Add(void* pObject);

    void Remove(void* pObject);

    void Render(void* pCamera);
};

#endif
