#include "Rect.h"

Rect::Rect()
{
    m_fX      = 0.0f;
    m_fY      = 0.0f;
    m_fWidth  = 1.0f;
    m_fHeight = 1.0f;
}

Rect::~Rect()
{

}

int Rect::Overlaps(Rect* pOther)
{
    if (pOther->m_fX < m_fX)
    {
        return 0;
    }
    else if (pOther->m_fX > m_fX + m_fWidth)
    {
        return 0;
    }
    else if (pOther->m_fY < m_fY)
    {
        return 0;
    }
    else if (pOther->m_fY > m_fY + m_fHeight)
    {
        return 0;
    }

    return 1;
}

int Rect::Contains(float fX,
                   float fY)
{
    if ((fX <= m_fX + m_fWidth)  &&
        (fX >= m_fX)             &&
        (fY <= m_fY + m_fHeight) &&
        (fY >= m_fY))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
