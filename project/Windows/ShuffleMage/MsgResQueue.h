#ifndef MSG_RES_QUEUE_H
#define MSG_RES_QUEUE_H

#include "Message.h"

class MsgResQueue : public Message
{
public:

    MsgResQueue();
    ~MsgResQueue();

    void Read(char* pBuffer);

    void Write(char* pBuffer);

    int Size();

    void Clear();

    int m_nSuccess;
};

#endif