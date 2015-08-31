#include "MsgResQueue.h"
#include "Constants.h"
MsgResQueue::MsgResQueue()
{
    Clear();
}

MsgResQueue::~MsgResQueue()
{

}

void MsgResQueue::Read(char* pBuffer)
{
    m_nSuccess = *reinterpret_cast<int*>(pBuffer + HEADER_SIZE);
    m_nSide    = *reinterpret_cast<int*>(pBuffer + HEADER_SIZE + 4);
}

void MsgResQueue::Write(char* pBuffer)
{
    *reinterpret_cast<int*>(pBuffer) = MSG_RES_QUEUE;
    *reinterpret_cast<int*>(pBuffer + HEADER_SIZE)     = m_nSuccess;
    *reinterpret_cast<int*>(pBuffer + HEADER_SIZE + 4) = m_nSide;
}

int MsgResQueue::Size()
{
    return MSG_RES_QUEUE_SIZE;
}

void MsgResQueue::Clear()
{
    m_nSuccess = QUEUE_STATUS_NONE;
    m_nSide = 0;
}
