#ifndef MSG_RES_LOGIN_H
#define MSG_RES_LOGIN_H

#include "Message.h"


class MsgResLogin : public Message
{
public:

    MsgResLogin();
    ~MsgResLogin();

    void Read(char* pBuffer);

    void Write(char* pBuffer);

    void Clear();

    int  m_nSuccess;
    int  m_nPlayerID;
    int  m_nGold;
    int  m_arCollection[COLLECTION_SIZE];
    int  m_arDeck1[DECK_SIZE];
    int  m_arDeck2[DECK_SIZE];
    int  m_arDeck3[DECK_SIZE];
};




#endif
