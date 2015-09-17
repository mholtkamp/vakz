#ifndef MSG_HEALTH_H
#define MSG_HEALTH_H

#include "Message.h"

class MsgHealth : public Message
{
public:

    MsgHealth();
    ~MsgHealth();

    void Read(char* pBuffer);

    void Write(char* pBuffer);

    int Size();

    void Clear();

    int m_nPlayer;
    int m_nHealth;
};

#endif
