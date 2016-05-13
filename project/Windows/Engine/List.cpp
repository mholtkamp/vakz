#include "List.h"

List::List()
{
    m_pHead  = 0;
    m_pTail  = 0;
    m_nCount = 0;
}

List::~List()
{
    int i = 0;
    ListNode* pNext = m_pHead;
    ListNode* pCur  = m_pHead;

    for (i = 0; i < m_nCount; i++)
    {
        pCur = pNext;
        pNext = pCur->m_pNext;
        delete pCur;
    }

    pCur = 0;
}

void List::Add(void* pData)
{
    ListNode* pNewNode = new ListNode();

    // Check if anything is in the list
    if (m_nCount == 0)
    {
        // First element in the list

        pNewNode->m_pNext = 0;
        pNewNode->m_pPrev = 0;
        m_pHead = pNewNode;
        m_pTail = pNewNode;
    }
    else
    {
        // Not the first item in list, so append it to
        // the tail of the list.
        pNewNode->m_pPrev = m_pTail;
        pNewNode->m_pNext = 0;

        m_pTail->m_pNext = pNewNode;
        m_pTail = pNewNode;
    }

    pNewNode->m_pData = pData;
    ++m_nCount;
}

void List::Insert(void* pData, int nPosition)
{
    int i = 0;
    ListNode* pPrev = 0;
    ListNode* pNext = 0;
    ListNode* pNewNode = new ListNode();

    // Check those bounds
    if (nPosition > m_nCount)
    {
        nPosition = m_nCount;
    }

    if (nPosition < 0)
    {
        nPosition = 0;
    }

    // Adding first node.
    if (m_nCount == 0)
    {
        pNewNode->m_pNext = 0;
        pNewNode->m_pPrev = 0;
        m_pHead = pNewNode;
        m_pTail = pNewNode;
    }
    // Adding node to first element
    else if (nPosition == 0)
    {
        pNewNode->m_pNext = m_pHead;
        pNewNode->m_pPrev = 0;
        m_pHead->m_pPrev = pNewNode;
        m_pHead = pNewNode;
    }
    // Adding node to last element
    else if (nPosition == m_nCount)
    {
        pNewNode->m_pPrev = m_pTail;
        pNewNode->m_pNext = 0;
        m_pTail->m_pNext = pNewNode;
        m_pTail = pNewNode;
    }
    else
    {
        // To optimize this part, check to see if the position
        // is greater than m_nCount/2. If so, iterate from 
        // the tail to find the m_pNext and m_pPrev nodes.
        pPrev = m_pHead;

        for (i = 0; i < nPosition; i++)
        {
            pPrev = pPrev->m_pNext;
        }

        pNext = pPrev->m_pNext;

        // pPrev and pNext are the two nodes we need to
        // connect to the pNewNode
        pPrev->m_pNext = pNewNode;
        pNext->m_pPrev = pNewNode;
        pNewNode->m_pPrev = pPrev;
        pNewNode->m_pNext = pNext;
    }

    m_nCount++;
}

int List::Remove(void* pData)
{
    int i = 0;
    ListNode* pPrev = 0;
    ListNode* pNext = 0;
    ListNode* pTarget = m_pHead;

    if (m_nCount == 0)
    {
        // Nothing in list to remove
        return 0;
    }

    // Iterate though the list to find matching data
    for (i = 0; i < m_nCount; i++)
    {
        if (pTarget->m_pData == pData)
        {
            break;
        }
        
        pTarget = pTarget->m_pNext;
    }

    if (pTarget == 0)
    {
        // Reached the end of the list without finding the data
        return 0;
    }
    
    // Removing from front of list
    if (i == 0)
    {
        pTarget->m_pNext->m_pPrev = 0;
        m_pHead = pTarget->m_pNext;
    }
    else if (i == m_nCount - 1)
    {
        pTarget->m_pPrev->m_pNext = 0;
        m_pTail = pTarget->m_pPrev;
    }
    else
    {
        pTarget->m_pPrev->m_pNext = pTarget->m_pNext;
        pTarget->m_pNext->m_pPrev = pTarget->m_pPrev;
    }

    // Decrement the List count member
    --m_nCount;

    // Clean up data
    delete pTarget;
    pTarget = 0;

    // If execution made it to this point, then the node was found and removed.
    return 1;
}

int List::Contains(void* pData)
{
    ListNode* pCur = m_pHead;

    while (pCur != 0)
    {
        if (pCur->m_pData == pData)
            return 1;

        pCur = pCur->m_pNext;
    }

    return 0;
}

ListNode* List::GetHead()
{
    return m_pHead;
}

ListNode* List::GetTail()
{
    return m_pTail;
}

ListNode* List::Get(int nPosition)
{
    int i = 0;
    ListNode* pRet = m_pHead;
    
    if (nPosition < 0 ||
        nPosition >= m_nCount)
    {
        // The position is out of range...
        return 0;
    }

    for (i = 0; i < nPosition; i++)
    {
        if (pRet != 0)
            pRet = pRet->m_pNext;
    }

    return pRet;
}

ListNode* List::Get(void* pData)
{
    ListNode* pRet = m_pHead;

    while (pRet != 0)
    {
        if (pRet->m_pData == pData)
        {
            break;
        }

        pRet = pRet->m_pNext;
    }

    return pRet;
}

int List::Count()
{
    return m_nCount;
}
