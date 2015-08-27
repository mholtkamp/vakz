#ifndef MSG_QUEUE_H
#define MSG_QUEUE_H

#include "Message.h"

class MsgQueue : public Message
{
public:

    MsgQueue();
    ~MsgQueue();

    void Read(char* pBuffer);

    void Write(char* pBuffer);

    int Size();

    void Clear();

    int m_nQueueType;

};

#endif