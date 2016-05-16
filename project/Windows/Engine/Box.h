#ifndef BOX_H
#define BOX_H

class Box
{
public:

    Box();
    ~Box();

    int Intersects(Box* pOther);
    int Contains(Box* pOther);

    float m_arCenter[3];
    float m_arExtent[3];
};

#endif