#ifndef TIMER_H
#define TIMER_H

#if defined(ANDROID)
#include <sys/time.h>
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
    long long m_llStart;
    long long m_llEnd;
#elif defined(ANDROID)
    struct timeval m_tvStart;
    struct timeval m_tvEnd;
#endif
};


#endif
