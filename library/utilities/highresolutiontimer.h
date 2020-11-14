/************************************************************************
*    FILE NAME:       highresolutiontimer.h
*
*    DESCRIPTION:     high resolution timer class
************************************************************************/

#pragma once

// Standard lib dependencies
#include <cstdint>

class CHighResTimer
{
public:

    // Get the instance of the singleton class
    static CHighResTimer & Instance()
    {
        static CHighResTimer highResTimer;
        return highResTimer;
    }

    // Calc the elapsed time
    void calcElapsedTime();

    // Get the elapsed time
    double getElapsedTime();
    
    // Simple timer start
    void timerStart();
    
    // Simple timer stop
    float timerStop();

    // Get the FPS
    float getFPS();

    // Get the time
    double getTime();

private:

    // Constructor
    CHighResTimer();

    // Destructor
    ~CHighResTimer();
    
    // the inverse of the timer frenquency - multipling is faster
    double m_inverseTimerFrequency;

    // Last recorded time
    uint64_t m_lastTime;
    
    // Timer time
    uint64_t m_timer;

    // The amount of time that has elapsed between frames
    double m_elapsedTime;

    // The frames per second
    float m_fps;

};
