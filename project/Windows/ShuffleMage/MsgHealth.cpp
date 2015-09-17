#include "MsgHealth.h"

MsgHealth::MsgHealth()
{
    Clear();
}

MsgHealth::~MsgHealth()
{

}

void MsgHealth::Read(char* pBuffer)
{
    m_nPlayer = *reinterpret_cast<int*>(pBuffer + HEADER_SIZE);
    m_nHealth = *reinterpret_cast<int*>(pBuffer + HEADER_SIZE + 4);
}

void MsgHealth::Write(char* pBuffer)
{
    *reinterpret_cast<int*>(pBuffer)                   = MSG_HEALTH;
    *reinterpret_cast<int*>(pBuffer + HEADER_SIZE)     = m_nPlayer;
    *reinterpret_cast<int*>(pBuffer + HEADER_SIZE + 4) = m_nHealth;
}

int MsgHealth::Size()
{
    return MSG_HEALTH_SIZE;
}

void MsgHealth::Clear()
{
    m_nPlayer  = 0;
    m_nHealth  = 0;
}
