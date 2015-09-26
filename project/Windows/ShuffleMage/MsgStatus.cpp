#include "MsgStatus.h"

MsgStatus::MsgStatus()
{
    Clear();
}

MsgStatus::~MsgStatus()
{

}

void MsgStatus::Read(char* pBuffer)
{
    m_nPlayer    = *reinterpret_cast<int*>(pBuffer + HEADER_SIZE);
    m_nStatus    = *reinterpret_cast<int*>(pBuffer + HEADER_SIZE + 4);
    m_nAfflicted = *reinterpret_cast<int*>(pBuffer + HEADER_SIZE + 8);
}

void MsgStatus::Write(char* pBuffer)
{
    *reinterpret_cast<int*>(pBuffer)                   = MSG_STATUS;
    *reinterpret_cast<int*>(pBuffer + HEADER_SIZE)     = m_nPlayer;
    *reinterpret_cast<int*>(pBuffer + HEADER_SIZE + 4) = m_nStatus;
    *reinterpret_cast<int*>(pBuffer + HEADER_SIZE + 8) = m_nAfflicted;
}

int MsgStatus::Size()
{
    return MSG_STATUS_SIZE;
}

void MsgStatus::Clear()
{
    m_nPlayer    = 0;
    m_nStatus    = 0;
    m_nAfflicted = 0;
}
