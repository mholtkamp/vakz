#include "Timer.h"

#if defined (WINDOWS)
#include <Windows.h>
#endif

//*****************************************************************************
// Constructor
//*****************************************************************************
Timer::Timer()
{
    QueryPerformanceFrequency(&m_liFrequency);
}

//*****************************************************************************
// Destructor
//*****************************************************************************
Timer::~Timer()
{

}

//*****************************************************************************
// Start
//*****************************************************************************
void Timer::Start()
{
#if defined (WINDOWS)
    QueryPerformanceCounter(&m_liStart);
#elif defined(ANDROID)
    gettimeofday(&m_tvStart, 0);
#endif
}

//*****************************************************************************
// Stop
//*****************************************************************************
void Timer::Stop()
{
#if defined (WINDOWS)
    QueryPerformanceCounter(&m_liEnd);
#elif defined(ANDROID)
    gettimeofday(&m_tvEnd, 0);
#endif
}

//*****************************************************************************
// Time
//*****************************************************************************
float Timer::Time()
{
#if defined (WINDOWS)
    return static_cast<float>(m_liEnd.QuadPart - m_liStart.QuadPart)/m_liFrequency.QuadPart;
#elif defined(ANDROID)
    float fSeconds = 0.0f;
    fSeconds = static_cast<float>(m_tvEnd.tv_sec - m_tvStart.tv_sec);
    fSeconds += static_cast<float>(m_tvEnd.tv_usec - m_tvStart.tv_usec)/1000000.0f;
    return fSeconds;
#else
    return 0.0f;
#endif
}
