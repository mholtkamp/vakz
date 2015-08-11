#ifndef RECTANGLE_H
#define RECTANGLE_H

class Rect
{
public:

    Rect();

    ~Rect();

    int Overlaps(Rect* pOther);

    int Contains(float fX,
                 float fY);

    float m_fX;
    float m_fY;
    float m_fWidth;
    float m_fHeight;
};

#endif
