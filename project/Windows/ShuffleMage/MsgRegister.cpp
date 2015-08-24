#include "MsgRegister.h"
#include <string.h>

MsgRegister::MsgRegister()
{
    Clear();
}

MsgRegister::~MsgRegister()
{

}

void MsgRegister::Read(char* pBuffer)
{
    memcpy(m_arUser, pBuffer + HEADER_SIZE, USER_BUFFER_SIZE);
    memcpy(m_arPass, pBuffer + HEADER_SIZE + USER_BUFFER_SIZE, PASS_BUFFER_SIZE);
}

void MsgRegister::Write(char* pBuffer)
{
    *reinterpret_cast<int*>(pBuffer) = MSG_REGISTER;
    memcpy(pBuffer + HEADER_SIZE, m_arUser, USER_BUFFER_SIZE);
    memcpy(pBuffer + HEADER_SIZE + USER_BUFFER_SIZE, m_arPass, PASS_BUFFER_SIZE);
}

void MsgRegister::Clear()
{
    Message::Clear();
    memset(m_arUser, 0, USER_BUFFER_SIZE);
    memset(m_arPass, 0, PASS_BUFFER_SIZE);
}