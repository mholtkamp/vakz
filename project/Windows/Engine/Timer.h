#ifndef TIMER_H
#define TIMER_H

#if defined(ANDROID)
#include <sys/time.h>
#elif defined (WINDOWS)
#include <Windows.h>

#endif

class Timer
{

public:

    //## **********************************************************************
    //## Constructor
    //## **********************************************************************
    Timer();

    //## **********************************************************************
    //## Destructor
    //## **********************************************************************
    ~Timer();

    //## **********************************************************************
    //## Start
    //## 
    //## Begins the timer, resetting the starting time.
    //## **********************************************************************
    void Start();

    //## **********************************************************************
    //## Stop
    //## 
    //## Stops the timer, recording the current time when stopped.
    //## **********************************************************************
    void Stop();

    //## **********************************************************************
    //## Time
    //##
    //## Returns the difference in seconds between the time at start, and the
    //## time when stopped. This should only be called after a Start() and
    //## Stop() call.
    //##
    //## Returns:
    //##   float - time that has elapsed from start to end.
    //## **********************************************************************
    float Time();

private:

#if defined(WINDOWS)
    LARGE_INTEGER m_liStart;
    LARGE_INTEGER m_liEnd;
    LARGE_INTEGER m_liFrequency;

#elif defined(ANDROID)
    struct timeval m_tvStart;
    struct timeval m_tvEnd;
#endif
};


#endif
