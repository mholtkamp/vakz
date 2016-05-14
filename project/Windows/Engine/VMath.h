#ifndef VMATH_H
#define VMATH_H

#include <math.h>

#define DEGREES_TO_RADIANS 0.0174532925f
#define RADIANS_TO_DEGREES 57.2957795f

//## Cross Product specifically for 3-dim vectors
static inline void CrossProduct(float* arA, float* arB, float* arRes)
{
    arRes[0] = arA[1]*arB[2] - arA[2]*arB[1];
    arRes[1] = arA[2]*arB[0] - arA[0]*arB[2];
    arRes[2] = arA[0]*arB[1] - arA[1]*arB[0];
}

//## Dot Product specifically for 3-dim vectors
static inline float DotProduct(float* arA, float* arB)
{
    return arA[0]*arB[0] + arA[1]*arB[1] + arA[2]*arB[2];
}

static inline float Magnitude(float* arVec)
{
    return sqrtf(arVec[0]*arVec[0] + arVec[1]*arVec[1] + arVec[2]*arVec[2]);
}

static inline void NormalizeVector(float* arVec)
{
    float fMagnitude = Magnitude(arVec);

    if (fMagnitude != 0.0f)
    {
        arVec[0] = arVec[0]/fMagnitude;
        arVec[1] = arVec[1]/fMagnitude;
        arVec[2] = arVec[2]/fMagnitude;
    }
}


static inline float AngleBetweenVectors(float* arA, float* arB)
{
    float fMagA = Magnitude(arA);
    float fMagB = Magnitude(arB);

    float fDot = DotProduct(arA, arB);

    return acosf(fDot/(fMagA * fMagB)) * RADIANS_TO_DEGREES;
}

#endif
