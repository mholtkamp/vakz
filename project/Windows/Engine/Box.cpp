#include "Box.h"

Box::Box()
{
    m_arCenter[0] = 0.0f;
    m_arCenter[1] = 0.0f;
    m_arCenter[2] = 0.0f;

    m_arExtent[0] = 0.5f;
    m_arExtent[1] = 0.5f;
    m_arExtent[2] = 0.5f;
}

Box::~Box()
{

}

int Box::Intersects(Box* pOther)
{
    if (m_arCenter[0] + m_arExtent[0] < pOther->m_arCenter[0] - pOther->m_arExtent[0])
        return 0;
    if (m_arCenter[1] + m_arExtent[1] < pOther->m_arCenter[1] - pOther->m_arExtent[1])
        return 0;
    if (m_arCenter[2] + m_arExtent[2] < pOther->m_arCenter[2] - pOther->m_arExtent[2])
        return 0;
    if (m_arCenter[0] - m_arExtent[0] > pOther->m_arCenter[0] + pOther->m_arExtent[0])
        return 0;
    if (m_arCenter[1] - m_arExtent[1] > pOther->m_arCenter[1] + pOther->m_arExtent[1])
        return 0;
    if (m_arCenter[2] - m_arExtent[2] > pOther->m_arCenter[2] + pOther->m_arExtent[2])
        return 0;

    return 1;
}

int Box::Contains(Box* pOther)
{
    if ((pOther->m_arCenter[0] - pOther->m_arExtent[0] > m_arCenter[0] - m_arExtent[0]) &&
        (pOther->m_arCenter[0] + pOther->m_arExtent[0] < m_arCenter[0] + m_arExtent[0]) &&
        (pOther->m_arCenter[1] - pOther->m_arExtent[1] > m_arCenter[1] - m_arExtent[1]) &&
        (pOther->m_arCenter[1] + pOther->m_arExtent[1] < m_arCenter[1] + m_arExtent[1]) &&
        (pOther->m_arCenter[2] - pOther->m_arExtent[2] > m_arCenter[2] - m_arExtent[2]) &&
        (pOther->m_arCenter[2] + pOther->m_arExtent[2] < m_arCenter[2] + m_arExtent[2]))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}