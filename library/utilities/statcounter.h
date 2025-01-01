/************************************************************************
*    FILE NAME:       statcounter.h
*
*    DESCRIPTION:     3D stat counter class singleton
************************************************************************/  

#pragma once

// Game lib dependencies
#include <utilities/timer.h>

// Boost lib dependencies
#include <boost/signals2.hpp>

// Standard lib dependencies
#include <string>

class CStatCounter
{
public:

    // Boost signal defination
    typedef boost::signals2::signal<void (const std::string &)> statCounterSignal_t;

    // Get the instance of the singleton class
    static CStatCounter & Instance()
    {
        static CStatCounter statCounter;
        return statCounter;
    }

    // Have we run through one cycle.
    void incCycle();

    // Inc the display counter
    void incDisplayCounter( int value = 1 );
    
    // Inc the physics objects counter
    void incPhysicsObjectsCounter();

    // Set the contex counters
    void setPoolContexCounter( size_t value );
    void setActiveContexCounter( int value );
    
    // Connect/Disconnect to the signal
    void connect( const statCounterSignal_t::slot_type & slot );
    void disconnect();

private:

    // Reset the counter
    void resetCounters();

    // Format the stat string
    void formatStatString();

    CStatCounter();
    ~CStatCounter();

private:

    // Counter for visual objects
    int m_vObjCounter;
    
    // Counter for physics objects
    int m_physicsObjCounter;

    // Elapsed time counter
    double m_elapsedFPSCounter;

    // cycle counter. This counter is never reset
    uint m_cycleCounter;

    // Angle Script contex counter
    size_t m_poolContexCounter;
    int m_activeContexCounter;

    // Stat string
    std::string m_statStr;

    // Stats display timer
    CTimer m_statsDisplayTimer;
    
    // Stat counter signal
    statCounterSignal_t m_statCounterSignal;
};
