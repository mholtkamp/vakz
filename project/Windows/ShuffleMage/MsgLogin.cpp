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
    memcpy(m_arUser,
           pBuffer + HEADER_SIZE,
           USER_BUFFER_SIZE);
    memcpy(m_arPass,
           pBuffer + HEADER_SIZE + USER_BUFFER_SIZE,
           PASS_BUFFER_SIZE);
}

void MsgLogin::Clear()
{
    Message::Clear();

    memset(m_arUser, 0, USER_BUFFER_SIZE);
    memset(m_arPass, 0, PASS_BUFFER_SIZE);
}

void MsgLogin::Write(char* pBuffer)
{
    *reinterpret_cast<int*>(pBuffer) = MSG_LOGIN;
    memcpy(pBuffer + HEADER_SIZE, 
           m_arUser,
           USER_BUFFER_SIZE);
    memcpy(pBuffer + HEADER_SIZE + USER_BUFFER_SIZE,
           m_arPass,
           PASS_BUFFER_SIZE);
}

int MsgLogin::Size()
{
    return MSG_LOGIN_SIZE;
}