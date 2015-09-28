#ifndef MSG_TILE_H
#define MSG_TILE_H

#include "Message.h"

class MsgTile : public Message
{
public:

    MsgTile();
    ~MsgTile();

    void Read(char* pBuffer);

    void Write(char* pBuffer);

    int Size();

    void Clear();

    int m_nX;
    int m_nZ;
    int m_nOwner;
    int m_nType;
};

#endif