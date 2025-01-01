
/************************************************************************
*    FILE NAME:       statcounter.cpp
*
*    DESCRIPTION:     3D stat counter class singleton
************************************************************************/

// Physical component dependency
#include <utilities/statcounter.h>

// Game lib dependencies
#include <utilities/settings.h>
#include <utilities/highresolutiontimer.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CStatCounter::CStatCounter() :
    m_vObjCounter(0),
    m_physicsObjCounter(0),
    m_elapsedFPSCounter(0),
    m_cycleCounter(0),
    m_poolContexCounter(0),
    m_activeContexCounter(0),
    m_statsDisplayTimer(2000)
{
    resetCounters();
}


/************************************************************************
*    DESC:  Destructor
************************************************************************/
CStatCounter::~CStatCounter()
{
}


/************************************************************************
*    DESC:  Connect to the signal
************************************************************************/
void CStatCounter::connect( const statCounterSignal_t::slot_type & slot )
{
    m_statCounterSignal.connect(slot);
}

/************************************************************************
*    DESC:  Disconnect all slots
************************************************************************/
void CStatCounter::disconnect()
{
    m_statCounterSignal.disconnect_all_slots();
}


/************************************************************************
*    DESC:  Have we run through one cycle.
*
*	 ret: bool - true if one cycle has been completed
************************************************************************/
void CStatCounter::incCycle()
{
    // These counters are incremeented each game loop cycle so they can
    // be placed here in this function because this function is also called
    // each game loop cycle
    m_elapsedFPSCounter += CHighResTimer::Instance().getFPS();

    ++m_cycleCounter;

    // update the stats every 1000 miliseconds
    if( m_statsDisplayTimer.expired(CTimer::RESTART_ON_EXPIRE) )
    {
        formatStatString();
        m_statCounterSignal( m_statStr );

        // Now that the stats are displayed, we can reset out counters.
        resetCounters();
    }
}


/************************************************************************
*    DESC:  Reset the counter
************************************************************************/
void CStatCounter::resetCounters()
{
    m_vObjCounter = 0;
    m_physicsObjCounter = 0;
    m_elapsedFPSCounter = 0.0;
    m_cycleCounter = 0;
}


/************************************************************************
*    DESC:  Format the stat string
************************************************************************/
void CStatCounter::formatStatString()
{
    m_statStr = boost::str( boost::format("fps: %d - sca: %d - scp: %d - vis: %d - phy: %d - res: %d x %d")
        % ((int)(m_elapsedFPSCounter / (double)m_cycleCounter))
        % m_activeContexCounter
        % m_poolContexCounter
        % (m_vObjCounter / m_cycleCounter)
        % (m_physicsObjCounter / m_cycleCounter)
        % CSettings::Instance().getSize().w
        % CSettings::Instance().getSize().h
        //% (playerPos.x)
        //% (playerPos.y)
        );
}


/************************************************************************
*    DESC:  Inc the display counter
************************************************************************/
void CStatCounter::incDisplayCounter( int value )
{
    m_vObjCounter += value;
}


/************************************************************************
*    DESC:  Inc the physics objects counter
************************************************************************/
void CStatCounter::incPhysicsObjectsCounter()
{
    ++m_physicsObjCounter;
}


/************************************************************************
*    DESC:  Set the pool contex counter
************************************************************************/
void CStatCounter::setPoolContexCounter( size_t value )
{
    m_poolContexCounter = value;
}


/************************************************************************
*    DESC:  Set the active contex counter
************************************************************************/
void CStatCounter::setActiveContexCounter( int value )
{
    m_activeContexCounter = value;
}
