#include "MsgLogin.h"
#include <string.h>

MsgLogin::MsgLogin()
{
    Clear();
}

MsgLogin::~MsgLogin()
{

}

void MsgLogin::Read(char* pBuffer)
{

}

void MsgLogin::Clear()
{
    Message::Clear();

    memset(m_arUser, 0, USER_BUFFER_LENGTH);
    memset(m_arPass, 0, PASS_BUFFER_LENGTH);
}

void MsgLogin::Write(char* pBuffer)
{
    *reinterpret_cast<int*>(pBuffer) = MSG_LOGIN;
    memcpy(pBuffer + HEADER_SIZE, 
           m_arUser,
           USER_BUFFER_LENGTH);
    memcpy(pBuffer + HEADER_SIZE + USER_BUFFER_LENGTH,
           m_arPass,
           PASS_BUFFER_LENGTH);
}