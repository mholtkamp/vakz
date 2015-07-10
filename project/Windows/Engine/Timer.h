#ifndef TIMER_H
#define TIMER_H

#if defined(ANDROID)
#include <sys/time.h>
#endif

class Timer
{

public:

    Timer();

    ~Timer();

    void Start();

    void Stop();

    float Time();

private:

#if defined(WINDOWS)
    long long m_llStart;
    long long m_llEnd;
#elif defined(ANDROID)
    struct timeval m_tvStart;
    struct timeval m_tvEnd;
#endif
};


#endif
