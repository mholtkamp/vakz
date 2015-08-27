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

}

void MsgResQueue::Write(char* pBuffer)
{
    *reinterpret_cast<int*>(pBuffer) = MSG_RES_QUEUE;
    *reinterpret_cast<int*>(pBuffer + HEADER_SIZE)     = m_nSuccess;
}

int MsgResQueue::Size()
{
    return MSG_RES_QUEUE_SIZE;
}

void MsgResQueue::Clear()
{
    m_nSuccess = QUEUE_STATUS_NONE;
}
