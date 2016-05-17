#ifndef OCTREE_H
#define OCTREE_H

#include "Box.h"
#include "List.h"


struct OctreeObject
{
    void* m_pObject;
    Box m_box;

    OctreeObject(void* pObject,
                 Box& box)
    {
        m_pObject = pObject;
        m_box = box;
    }
};

class OctreeNode
{
public:
    OctreeNode(Box& box);
    ~OctreeNode();

    void Add(void* pObject, Box& box);
    void Add(OctreeObject* pOctreeObject);
    int Remove(void* pObject, Box& box);
    void FindIntersectingObjects(Box& box, List& list);
    void Render(float* arBoxVertices);
    int GetContainedObjectsCount();
    void Subdivide();
    void CollapseSubdivisions();

    OctreeNode* m_pParent;
    OctreeNode* m_arChildren[8];
    int m_nSubdivided;
    unsigned char m_ucActiveNodes;
    Box m_bRegion;
    List m_lObjects;
};

class Octree
{
    Octree();
    ~Octree();

    void Initialize(Box& boxWorldBounds);

    void Add(void* pObject, Box& box);

    int Remove(void* pObject, Box& box);

    void FindIntersectingObjects(Box& box, List& list);

    void Render(void* pCamera,
                float* arColor);

private:
    
    int m_nInitialized;
    int m_nCount;
    OctreeNode* m_pRoot;
};

#endif
