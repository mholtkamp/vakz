#ifndef RECTANGLE_H
#define RECTANGLE_H

class Rectangle
{
public:

    Rectangle();

    ~Rectangle();

    int Overlaps(Rectangle* pOther);

    int Contains(float fX,
                 float fY);

    float m_fX;
    float m_fY;
    float m_fWidth;
    float m_fHeight;
};

#endif
