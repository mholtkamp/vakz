#include "MsgResLogin.h"
#include <string.h>

MsgResLogin::MsgResLogin()
{
    Clear();
}

MsgResLogin::~MsgResLogin()
{

}

void MsgResLogin::Read(char* pBuffer)
{
    m_nSuccess  = *reinterpret_cast<int*>(pBuffer + HEADER_SIZE);
    m_nPlayerID = *reinterpret_cast<int*>(pBuffer + HEADER_SIZE + 4);
    m_nGold     = *reinterpret_cast<int*>(pBuffer + HEADER_SIZE + 8);
    memcpy(m_arCollection, pBuffer + HEADER_SIZE + 12, 2048);
    memcpy(m_arDeck1, pBuffer + HEADER_SIZE + 2060, 120);
    memcpy(m_arDeck2, pBuffer + HEADER_SIZE + 2180, 120);
    memcpy(m_arDeck3, pBuffer + HEADER_SIZE + 2300, 120);
}

void MsgResLogin::Write(char* pBuffer)
{
    *reinterpret_cast<int*>(pBuffer)                   = MSG_RES_LOGIN;
    *reinterpret_cast<int*>(pBuffer + HEADER_SIZE)     = m_nSuccess;
    *reinterpret_cast<int*>(pBuffer + HEADER_SIZE + 4) = m_nPlayerID;
    *reinterpret_cast<int*>(pBuffer + HEADER_SIZE + 8) = m_nGold;
    memcpy(pBuffer + HEADER_SIZE + 12, m_arCollection, 2048);
    memcpy(pBuffer + HEADER_SIZE + 2060, m_arDeck1, 120);
    memcpy(pBuffer + HEADER_SIZE + 2180, m_arDeck2, 120);
    memcpy(pBuffer + HEADER_SIZE + 2300, m_arDeck3, 120);
}

void MsgResLogin::Clear()
{
    m_nSuccess = 0;
    m_nPlayerID = 0;
    m_nGold = 0;
    memset(m_arCollection, 0, COLLECTION_SIZE * sizeof(int));
    memset(m_arDeck1, 0, DECK_SIZE * sizeof(int));
    memset(m_arDeck2, 0, DECK_SIZE * sizeof(int));
    memset(m_arDeck3, 0, DECK_SIZE * sizeof(int));
}

int MsgResLogin::Size()
{
    return MSG_RES_LOGIN_SIZE;
}