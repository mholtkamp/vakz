#include "Box.h"
#include "VGL.h"

#define VERTICES_PER_BOX 8

static unsigned char s_arBoxWireframeIndices[12 * 2] = {0, 1,
                                                        0, 2,
                                                        2, 3,
                                                        1, 3,
                                                        4, 5,
                                                        4, 6,
                                                        6, 7,
                                                        5, 7,
                                                        2, 6,
                                                        0, 4,
                                                        3, 7,
                                                        1, 5};
                                                      

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

void Box::RenderWireframe(float* arBoxVertices)
{
    GenerateVertexCoordinates(arBoxVertices);

    glDrawElements(GL_LINES, 24, GL_UNSIGNED_BYTE, s_arBoxWireframeIndices);
}


void Box::GenerateVertexCoordinates(float* arBoxVertices)
{
    int i = 0;

    for (i = 0; i < VERTICES_PER_BOX; i++)
    {
        arBoxVertices[i*3 + 0] = m_arCenter[0] + (1 - (i & 1)*2)*m_arExtent[0];
        arBoxVertices[i*3 + 1] = m_arCenter[1] + (1 - ((i & 2) >> 1) *2)*m_arExtent[1];
        arBoxVertices[i*3 + 2] = m_arCenter[2] + (1 - ((i & 4) >> 2) *2)*m_arExtent[2];
    }
}
