#ifndef MSG_REGISTER_H
#define MSG_REGISTER_H

#include "Message.h"

class MsgRegister : public Message
{
public:

    MsgRegister();
    ~MsgRegister();

    void Read(char* pBuffer);

    void Write(char* pBuffer);

    void Clear();

    char m_arUser[USER_BUFFER_SIZE];
    char m_arPass[PASS_BUFFER_SIZE];
};

#endif
