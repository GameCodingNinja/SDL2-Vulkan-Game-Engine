/************************************************************************
*    FILE NAME:       statcounter.h
*
*    DESCRIPTION:     3D stat counter class singleton
************************************************************************/  

#ifndef __stat_counter_h__
#define __stat_counter_h__

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
    typedef boost::signals2::signal<void (const std::string &)> StatCounterSignal;

    // Get the instance of the singleton class
    static CStatCounter & Instance()
    {
        static CStatCounter statCounter;
        return statCounter;
    }

    // Have we run through one cycle.
    void incCycle();

    // Inc the display counter
    void incDisplayCounter( size_t value = 1 );
    
    // Inc the physics objects counter
    void incPhysicsObjectsCounter();

    // Inc the script contex counter
    void incScriptContexCounter();
    void incActiveScriptContexCounter();
    
    // Connect/Disconnect to the signal
    void connect( const StatCounterSignal::slot_type & slot );
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
    size_t m_vObjCounter;
    
    // Counter for physics objects
    size_t m_physicsObjCounter;

    // Elapsed time counter
    double m_elapsedFPSCounter;

    // cycle counter. This counter is never reset
    size_t m_cycleCounter;

    // Angle Script contex counter
    size_t m_scriptContexCounter;
    size_t m_activeContexCounter;

    // Stat string
    std::string m_statStr;

    // Stats display timer
    CTimer m_statsDisplayTimer;
    
    // Stat counter signal
    StatCounterSignal m_statCounterSignal;

};

#endif  // __stat_counter_h__
