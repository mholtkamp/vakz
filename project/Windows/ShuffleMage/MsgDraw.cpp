#include "MsgDraw.h"
#include <string.h>

MsgDraw::MsgDraw()
{
    Clear();
}

MsgDraw::~MsgDraw()
{

}

void MsgDraw::Read(char* pBuffer)
{
    memcpy(m_arCards, pBuffer + HEADER_SIZE, HAND_SIZE * sizeof(int));
}

void MsgDraw::Write(char* pBuffer)
{
    *reinterpret_cast<int*>(pBuffer) = MSG_DRAW;
    memcpy(pBuffer + HEADER_SIZE, m_arCards, HAND_SIZE * sizeof(int));
}

int MsgDraw::Size()
{
    return MSG_DRAW_SIZE;
}

void MsgDraw::Clear()
{
        memset(m_arCards, 0, HAND_SIZE * sizeof(int));
}
