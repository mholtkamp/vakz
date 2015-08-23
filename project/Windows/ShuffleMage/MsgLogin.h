#ifndef MSG_LOGIN_H
#define MSG_LOGIN_H

#define USER_BUFFER_LENGTH 16
#define PASS_BUFFER_LENGTH 16

#include "Message.h"

class MsgLogin : public Message
{
public:

    MsgLogin();
    ~MsgLogin();

    void Read(char* pBuffer);

    void Write(char* pBuffer);

    void Clear();

    char m_arUser[USER_BUFFER_LENGTH];
    char m_arPass[PASS_BUFFER_LENGTH];

};

#endif