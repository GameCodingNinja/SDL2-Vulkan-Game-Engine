
/************************************************************************
*    FILE NAME:       timer.h
*
*    DESCRIPTION:     Timer Class
************************************************************************/

#pragma once

// Boost lib dependencies
#include <boost/noncopyable.hpp>

class CTimer : public boost::noncopyable
{
public:

    enum
    {
        START_EXPIRED = 1,
        RESTART_ON_EXPIRE = 1
    };

    // Constructor
    CTimer();
    CTimer( double interval, bool startExpired = false );

    // Destructor
    virtual ~CTimer();

    // Set the time to have expired
    void setExpired();

    // Has the timer expired
    bool expired( bool resetOnExpire = false );

    // Reset the timer to start over
    void reset();

    // Set the timer interval
    void set( double interval );

    // Set the timer interval
    void disable( bool disabled = true );

    // Set the value returned by Expired when the timer is disabled
    void setDisableValue( bool disableValue );

private:

    // Expired time
    double m_expiredTime;

    // time interval
    double m_timeInterval;

    // Disabled flag
    bool m_disabled;

    // Disabled return value.
    // This value allows a disabled timer to act as expired or not
    bool m_disableValue;
};
