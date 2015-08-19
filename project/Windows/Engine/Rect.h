#ifndef RECTANGLE_H
#define RECTANGLE_H

class Rect
{
public:

    //## **********************************************************************
    //## Constructor
    //## **********************************************************************
    Rect();

    //## **********************************************************************
    //## Destructor
    //## **********************************************************************
    ~Rect();

    //## **********************************************************************
    //## Overlaps
    //## 
    //## Checks if two axis aligned rectangles overlap.
    //## 
    //## Input:
    //##   pOther - pointer to the other rectangle.
    //##
    //## Returns:
    //##   int - '1' if this and pOther overlap
    //##         '0' otherwise.
    //## **********************************************************************
    int Overlaps(Rect* pOther);

    //## **********************************************************************
    //## Contains
    //## 
    //## Checks to see if the rectangle contains a single point
    //## 
    //## Input:
    //##   fX - x coordinate of point to test.
    //##   fY - y coordinate of point to test.
    //## 
    //## Returns:
    //##   int - '1' if point is contained in this rectangle.
    //##         '0' otherwise.
    //## **********************************************************************
    int Contains(float fX,
                 float fY);

    //## Spatial qualities
    float m_fX;
    float m_fY;
    float m_fWidth;
    float m_fHeight;
};

#endif
