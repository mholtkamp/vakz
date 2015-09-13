#ifndef MSG_DRAW_H
#define MSG_DRAW_H

#include "Constants.h"
#include "Message.h"

class MsgDraw : public Message
{
public:

    MsgDraw();
    ~MsgDraw();

    void Read(char* pBuffer);

    void Write(char* pBuffer);

    int Size();

    void Clear();

    int m_arCards[HAND_SIZE];

};

#endif