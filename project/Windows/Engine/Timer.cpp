#include "Timer.h"

#if defined (WINDOWS)
#include <Windows.h>
#endif

//*****************************************************************************
// Constructor
//*****************************************************************************
Timer::Timer()
{
#if defined(WINDOWS)
    m_llStart = 0;
    m_llEnd = 0;
#endif
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
    m_llStart = GetTickCount();
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
    m_llEnd = GetTickCount();
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
    return static_cast<float>(m_llEnd - m_llStart)/1000.0f;
#elif defined(ANDROID)
    float fSeconds = 0.0f;
    fSeconds = static_cast<float>(m_tvEnd.tv_sec - m_tvStart.tv_sec);
    fSeconds += static_cast<float>(m_tvEnd.tv_usec - m_tvStart.tv_usec)/1000000.0f;
    return fSeconds;
#else
    return 0.0f;
#endif
}
