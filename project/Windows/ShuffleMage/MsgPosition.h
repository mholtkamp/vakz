#ifndef MSG_POSITION_H
#define MSG_POSITION_H

#include "Message.h"

class MsgPosition : public Message
{
public:

    MsgPosition();
    ~MsgPosition();

    void Read(char* pBuffer);

    void Write(char* pBuffer);

    int Size();

    void Clear();

    int m_nPlayer;
    int m_nX;
    int m_nZ;
};

#endif