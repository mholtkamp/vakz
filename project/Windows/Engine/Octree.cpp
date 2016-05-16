#include "Octree.h"
#include "Log.h"
#include <string.h>

#define PLUS_X 1
#define PLUS_Y 2
#define PLUS_Z 4

OctreeNode::OctreeNode(Box& box)
{
    m_pParent = 0;
    memset(m_arChildren, 0, 8 * sizeof(OctreeNode*));
    m_ucActiveNodes = 0;
    m_bRegion = box;
}

OctreeNode::~OctreeNode()
{
    int i = 0;

    // Delete children nodes
    for (i = 0; i < 8; i++)
    {
        if ( m_arChildren[i] != 0)
        {
            delete m_arChildren[i];
            m_arChildren[i] = 0;
        }
    }

    // Delete OctreeObjects contained by m_lObjects, as these objects
    // are allocated in the Add() method.
    ListNode* pCur = m_lObjects.GetHead();
    OctreeObject* pOctreeObject = 0;

    while (pCur != 0)
    {
        pOctreeObject = reinterpret_cast<OctreeObject*>(pCur->m_pData);
        pCur = pCur->m_pNext;

        delete pOctreeObject;
        pOctreeObject = 0;
    }
}

void OctreeNode::Add(void* pObject, Box& box)
{
    Add(new OctreeObject(pObject, box));
}

void OctreeNode::Add(OctreeObject* pOctreeObject)
{
    int i = 0;

    // Check if any objects are contained in this node
    if (m_lObjects.Count() == 0)
    {
        // No objects in this node, so just add the pending
        // object to the list and return.
        m_lObjects.Add(pOctreeObject);
        return;
    }
    else
    {
        // Check if the node is already subdivided
        if (m_nSubdivided == 0)
        {
            // Perform first-time subdivision
            Subdivide();
        }

        // Iterate though child octreenodes and see if the new object
        // can fit into one of the children
        for (i = 0; i < 8; i++)
        {
            if (m_arChildren[i]->m_bRegion.Contains(&pOctreeObject->m_box))
            {
                // Add it to the better-fitting child node
                m_arChildren[i]->Add(pOctreeObject);

                return;
            }
        }

        // A child octreenode did not fully enclose the pending object's
        // bounding box, so just stick it into this octreenode's list
        m_lObjects.Add(pOctreeObject);
    }
}

int OctreeNode::Remove(void* pObject, Box& box)
{
    int  i = 0;

    // Iterate over this node's list, and check if the pObject
    // is contained in this node.
    ListNode* pCur = m_lObjects.GetHead();
    OctreeObject* pOctreeObject = 0;

    while (pCur != 0)
    {
        pOctreeObject = reinterpret_cast<OctreeObject*>(pCur->m_pData);
        pCur = pCur->m_pNext;

        if (pOctreeObject->m_pObject == pObject)
        {
            // Perform the remove operation from the list
            m_lObjects.Remove(pOctreeObject);

            // Since the octree object is no longer being used by 
            // the tree, delete it.
            delete pOctreeObject;
            pOctreeObject = 0;

            return 1;
        }
    }

    // Well, the object wasn't found in this octree node.
    // Check which children the search box overlaps, and then
    // try removing the target object from those nodes.
    if (m_nSubdivided != 0)
    {
        for (i = 0; i < 8; i++)
        {
            if (m_arChildren[i]->m_lObjects.GetHead() != 0 &&
                m_arChildren[i]->m_bRegion.Intersects(&box) != 0)
            {
                if (m_arChildren[i]->Remove(pObject, box) != 0)
                {
                    // The object was removed, return 1
                    return 1;
                }
            }
        }
    }

    // The object wasn't found in this node nor any of its children
    return 0;

}

void OctreeNode::FindIntersectingObjects(Box& box, List& list)
{
    int i = 0;

    // Iterate over this node's list, and check if the provided
    // search box overlaps any of the list's OctreeObject boxes.
    // If so, append them to the list.
    ListNode* pCur = m_lObjects.GetHead();
    OctreeObject* pOctreeObject = 0;

    while (pCur != 0)
    {
        pOctreeObject = reinterpret_cast<OctreeObject*>(pCur->m_pData);
        pCur = pCur->m_pNext;

        if (box.Intersects(&pOctreeObject->m_box))
        {
            list.Add(pOctreeObject->m_pObject);
        }
    }

    // If this node is subdivided (aka not a leaf node) then we 
    // need to check all of the children
    if (m_nSubdivided != 0)
    {
        for (i = 0; i < 8; i++)
        {
            if (m_arChildren[i]->m_lObjects.GetHead()       != 0 &&
                m_arChildren[i]->m_bRegion.Intersects(&box) != 0)
            {
                m_arChildren[i]->FindIntersectingObjects(box, list);
            }
        }
    }

}

void OctreeNode::Subdivide()
{
    int i = 0;
    Box boxOctant;
    OctreeObject* pOctreeObject = 0;

    // Not subdivided, so go ahead and create the 8 child nodes.
    for (i = 0; i < 8; i++)
    {
        boxOctant.m_arCenter[0] = m_bRegion.m_arCenter[0] + (1 - (i & 1)*2) * m_bRegion.m_arExtent[0];
        boxOctant.m_arCenter[1] = m_bRegion.m_arCenter[1] + (1 - (i & 2)*2) * m_bRegion.m_arExtent[1];
        boxOctant.m_arCenter[2] = m_bRegion.m_arCenter[2] + (1 - (i & 4)*2) * m_bRegion.m_arExtent[2];

        boxOctant.m_arExtent[0] = m_bRegion.m_arExtent[0]/2.0f;
        boxOctant.m_arExtent[1] = m_bRegion.m_arExtent[1]/2.0f;
        boxOctant.m_arExtent[2] = m_bRegion.m_arExtent[2]/2.0f;

        m_arChildren[i] = new OctreeNode(boxOctant);
    }

    // Then, see if any objects in the list (should only be one at max)
    // can be entirely enclosed by the child octants
    ListNode* pCurObj = m_lObjects.GetHead();
    while (pCurObj != 0)
    {
        pOctreeObject = reinterpret_cast<OctreeObject*>(pCurObj->m_pData);
        pCurObj = pCurObj->m_pNext;

        // Go through 8 octants and see if the octreeobject intersects any
        for (i = 0; i < 8; i++)
        {
            if (m_arChildren[i]->m_bRegion.Contains(&pOctreeObject->m_box))
            {
                // Remove it from this node
                // This is pretty inefficient (calling the list's Remove() method)
                // but if we manually removed node from linked list, then the 
                // list's count variable would be incorrect. Maybe List shouldn't
                // have a count anyway.
                m_lObjects.Remove(pOctreeObject);

                // Add it to the better-fitting child node
                m_arChildren[i]->Add(pOctreeObject);

                break;
            }
        }
    }

    // Mark subdivided flag
    m_nSubdivided = 1;
}

Octree::Octree()
{
    m_nCount = 0;
    m_pRoot  = 0;
}

Octree::~Octree()
{
    if (m_pRoot != 0)
    {
        delete m_pRoot;
        m_pRoot = 0;
    }
}

void Octree::Initialize(Box& boxWorldBounds)
{
    if (m_nInitialized == 0)
    {
        m_pRoot = new OctreeNode(boxWorldBounds);
        m_nInitialized = 1;
    }
}

void Octree::Add(void* pObject, Box& box)
{
    // Check if this is the first object being added to octree
    if (m_nInitialized == 0)
    {
        LogDebug("Cannot add object to Octree before initializing.");
        return;
    }

    m_pRoot->Add(pObject, box);
    m_nCount++;
}

int Octree::Remove(void* pObject, Box& box)
{
    int nResult = 0;

    if (m_nInitialized == 0)
    {
        return 0;
    }

    nResult = m_pRoot->Remove(pObject, box);

    if (nResult != 0)
    {
        m_nCount--;
    }

    return nResult;
}

void Octree::FindIntersectingObjects(Box& box, List& list)
{
    if (m_nInitialized != 0)
    {
        m_pRoot->FindIntersectingObjects(box, list);
    }
}
