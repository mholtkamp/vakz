#include "MsgCard.h"

MsgCard::MsgCard()
{
    Clear();
}

MsgCard::~MsgCard()
{

}


void MsgCard::Read(char* pBuffer)
{
    m_nCard   = *reinterpret_cast<int*>(pBuffer + HEADER_SIZE);
    m_nCaster = *reinterpret_cast<int*>(pBuffer + HEADER_SIZE + 4);
}

void MsgCard::Write(char* pBuffer)
{
    *reinterpret_cast<int*>(pBuffer)                   = MSG_CARD;
    *reinterpret_cast<int*>(pBuffer + HEADER_SIZE)     = m_nCard;
    *reinterpret_cast<int*>(pBuffer + HEADER_SIZE + 4) = m_nCaster;
}

int MsgCard::Size()
{
    return MSG_CARD_SIZE;;
}

void MsgCard::Clear()
{
    m_nCard   = 0;
    m_nCaster = 0;
}
