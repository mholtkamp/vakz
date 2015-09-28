#include "MsgTile.h"

MsgTile::MsgTile()
{
    Clear();
}

MsgTile::~MsgTile()
{

}

void MsgTile::Read(char* pBuffer)
{
    m_nX     = *reinterpret_cast<int*>(pBuffer + HEADER_SIZE);
    m_nZ     = *reinterpret_cast<int*>(pBuffer + HEADER_SIZE + 4);
    m_nOwner = *reinterpret_cast<int*>(pBuffer + HEADER_SIZE + 8);
    m_nType  = *reinterpret_cast<int*>(pBuffer + HEADER_SIZE + 12);
}

void MsgTile::Write(char* pBuffer)
{
    *reinterpret_cast<int*>(pBuffer)                    = MSG_TILE;
    *reinterpret_cast<int*>(pBuffer + HEADER_SIZE)      = m_nX;
    *reinterpret_cast<int*>(pBuffer + HEADER_SIZE + 4)  = m_nZ;
    *reinterpret_cast<int*>(pBuffer + HEADER_SIZE + 8)  = m_nOwner;
    *reinterpret_cast<int*>(pBuffer + HEADER_SIZE + 12) = m_nType;
}

int MsgTile::Size()
{
    return MSG_TILE_SIZE;
}

void MsgTile::Clear()
{
    m_nX     = 0;
    m_nZ     = 0;
    m_nOwner = 0;
    m_nType  = 0;
}
