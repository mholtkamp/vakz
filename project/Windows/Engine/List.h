#ifndef LIST_H
#define LIST_H

struct ListNode
{
    void* m_pData;
    ListNode* m_pNext;
    ListNode* m_pPrev;

    ListNode()
    {
        m_pData = 0;
        m_pNext = 0;
        m_pPrev = 0;
    }
};

class List
{
public:

    List();
    ~List();

    void Add(void* pData);

    void Insert(void* pData, int nPosition);

    int Remove(void* pData);

    int Contains(void* pData);

    ListNode* GetHead();

    ListNode* GetTail();

    ListNode* Get(int nPosition);

    ListNode* Get(void* pData);

    int Count();

private:

    ListNode* m_pHead;
    ListNode* m_pTail;
    int m_nCount;

};

#endif