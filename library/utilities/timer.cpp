
/************************************************************************
*    FILE NAME:       timer.cpp
*
*    DESCRIPTION:     Timer Class
************************************************************************/

// Physical component dependency
#include <utilities/timer.h>

// Game lib dependencies
#include <utilities/highresolutiontimer.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CTimer::CTimer(double interval, bool startExpired)
    : m_timeInterval(interval),
      m_disabled(false),
      m_disableValue(false)
{
    if( startExpired )
        setExpired();
    else
        reset();
}

CTimer::CTimer()
    : m_expiredTime(CHighResTimer::Instance().getTime()),
      m_disabled(false),
      m_disableValue(false)
{
}

/************************************************************************
*    DESC:  destructor
************************************************************************/
CTimer::~CTimer()
{
}

/***************************************************************************
*   DESC:  Reset the timer to start over
****************************************************************************/
void CTimer::reset()
{
    m_expiredTime = m_timeInterval + CHighResTimer::Instance().getTime();
    m_disabled = false;
}

/***************************************************************************
*   DESC:  Set the time to have expired
****************************************************************************/
void CTimer::setExpired()
{
    m_expiredTime = CHighResTimer::Instance().getTime();
}

/***************************************************************************
*   DESC:  Set the timer interval
****************************************************************************/
void CTimer::set( double interval )
{
    m_timeInterval = interval;
    reset();
}

/***************************************************************************
*   DESC:  Has the timer expired
****************************************************************************/
bool CTimer::expired( bool resetOnExpire )
{
    // Has the timer been disabled
    if( m_disabled )
        return m_disableValue;

    bool result(false);

    if( CHighResTimer::Instance().getTime() > m_expiredTime )
    {
        result = true;

        if( resetOnExpire )
            reset();
    }

    return result;
}

/***************************************************************************
*   DESC:  Disable this timer
****************************************************************************/
void CTimer::disable( bool disabled )
{
    m_disabled = disabled;
}

/***************************************************************************
*   DESC:  Set the value returned by Expired when the timer is disabled
****************************************************************************/
void CTimer::setDisableValue( bool disableValue )
{
    m_disableValue = disableValue;
}
