#ifndef MSG_RES_REGISTER_H
#define MSG_RES_REGISTER_H

#include "Message.h"


class MsgResRegister : public Message
{
public:

    MsgResRegister();
    ~MsgResRegister();

    void Read(char* pBuffer);

    void Write(char* pBuffer);

    int Size();

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
