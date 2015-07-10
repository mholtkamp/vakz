#include "VTime.h"

#if defined(WINDOWS)
#include <Windows.h>
#elif defined(ANDROID)
#include <sys/time.h>
#endif

static float s_fInitTime;

void SetInitTime()
{
#if defined (WINDOWS)
    s_fInitTime = static_cast<float>(GetTickCount())/1000.0f;
#elif defined(ANDROID)
    struct timeval tvTime;
    gettimeofday(&startTime, 0);
    s_fInitTime = static_cast<float>(tvTime.tv_sec) + static_cast<float>(tvTime.tv_usec)/1000000.0f;
#endif
}

float GetTime()
{
#if defined (WINDOWS)
    return (static_cast<float>(GetTickCount())/1000.0f) - s_fInitTime;
#elif defined(ANDROID)
    float fSeconds;
    struct timeval tvTime;
    gettimeofday(&startTime, 0);
    fSeconds = static_cast<float>(tvTime.tv_sec) + static_cast<float>(tvTime.tv_usec)/1000000.0f;
    return fSeconds - s_fInitTime;
#endif
}