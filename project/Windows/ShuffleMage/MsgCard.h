#ifndef MSG_CARD_H
#define MSG_CARD_H

#include "Message.h"

class MsgCard : public Message
{
public:

    MsgCard();
    ~MsgCard();

    void Read(char* pBuffer);
    
    void Write(char* pBuffer);

    int Size();

    void Clear();

    int m_nCard;
    int m_nCaster;
};

#endif