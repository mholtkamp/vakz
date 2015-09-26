#ifndef MSG_STATUS_H
#define MSG_STATUS_H

#include "Message.h"

class MsgStatus : public Message
{
public:

    MsgStatus();
    ~MsgStatus();

    void Read(char* pBuffer);
    
    void Write(char* pBuffer);

    int Size();

    void Clear();

    int m_nPlayer;
    int m_nStatus;
    int m_nAfflicted;
};

#endif