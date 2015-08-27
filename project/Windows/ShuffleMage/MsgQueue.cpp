#include "MsgQueue.h"

MsgQueue::MsgQueue()
{
    Clear();
}

MsgQueue::~MsgQueue()
{

}

void MsgQueue::Read(char* pBuffer)
{
    m_nQueueType = *reinterpret_cast<int*>(pBuffer + HEADER_SIZE);
}

void MsgQueue::Write(char* pBuffer)
{
    *reinterpret_cast<int*>(pBuffer) = MSG_QUEUE;
    *reinterpret_cast<int*>(pBuffer + HEADER_SIZE) = m_nQueueType;
}

int MsgQueue::Size()
{
    return MSG_QUEUE_SIZE;
}

void MsgQueue::Clear()
{
    m_nQueueType = QUEUE_TYPE_NONE;
}
