#ifndef COLLIDER_H
#define COLLIDER_H

#include "Matrix.h"

enum ColliderType
{
    COLLIDER_NO_TYPE  = -1,
    COLLIDER_BOX      =  0,
    COLLIDER_MESH     =  1
};

class Collider
{

public:

    Collider();

    ~Collider();

    int GetType();

    void SetPosition(float fX,
                     float fY,
                     float fZ);

    void SetScale(float fScaleX,
                  float fScaleY,
                  float fScaleZ);

    void SetRotation(float fRotX,
                     float fRotY,
                     float fRotZ);

    void SetColor(float fRed,
                  float fGreen,
                  float fBlue,
                  float fAlpha);

    void SetMatter(void* pMatter);

    void* GetMatter();

    virtual void Render(Matrix* pMVP) = 0;

    virtual int Overlaps(Collider* pOther) = 0;

    void EnableRendering();

    void DisableRendering();

    int IsRenderingEnabled();

protected:

    int m_nType;

    float m_fX;
    float m_fY;
    float m_fZ;

    float m_fScaleX;
    float m_fScaleY;
    float m_fScaleZ;

    float m_fRotX;
    float m_fRotY;
    float m_fRotZ;

    float m_arRenderColor[4];

    int m_nRenderEnable;

    void* m_pMatter;
};

#endif