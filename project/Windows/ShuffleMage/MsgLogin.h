#ifndef MSG_LOGIN_H
#define MSG_LOGIN_H

#include "Message.h"

class MsgLogin : public Message
{
public:

    MsgLogin();
    ~MsgLogin();

    void Read(char* pBuffer);

    void Write(char* pBuffer);

    int Size();

    void Clear();

    char m_arUser[USER_BUFFER_SIZE];
    char m_arPass[PASS_BUFFER_SIZE];
};

#endif