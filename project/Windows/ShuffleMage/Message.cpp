#include "Message.h"

Message::Message()
{
    Clear();
}

Message::~Message()
{
    
}

void Message::Read(char* pBuffer)
{
    // pBuffer != 0 check should be performed at a higher level.
    m_nID   = *reinterpret_cast<int*>(pBuffer);
}

void Message::Write(char* pBuffer)
{

}

void Message::Clear()
{
    m_nID   = 0;
}
