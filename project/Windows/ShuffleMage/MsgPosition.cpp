#include "MsgPosition.h"

MsgPosition::MsgPosition()
{
    Clear();
}

MsgPosition::~MsgPosition()
{

}

void MsgPosition::Read(char* pBuffer)
{
    m_nPlayer = *reinterpret_cast<int*>(pBuffer + HEADER_SIZE);
    m_nX      = *reinterpret_cast<int*>(pBuffer + HEADER_SIZE + 4);
    m_nZ      = *reinterpret_cast<int*>(pBuffer + HEADER_SIZE + 8);
}

void MsgPosition::Write(char* pBuffer)
{
    *reinterpret_cast<int*>(pBuffer)                   = MSG_POSITION;
    *reinterpret_cast<int*>(pBuffer + HEADER_SIZE)     = m_nPlayer;
    *reinterpret_cast<int*>(pBuffer + HEADER_SIZE + 4) = m_nX;
    *reinterpret_cast<int*>(pBuffer + HEADER_SIZE + 8) = m_nZ;
}

int MsgPosition::Size()
{
    return MSG_POSITION_SIZE;
}

void MsgPosition::Clear()
{
    m_nPlayer = 0;
    m_nX      = 0;
    m_nZ      = 0;
}
