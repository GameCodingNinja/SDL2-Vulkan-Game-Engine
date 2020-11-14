
/************************************************************************
*    FILE NAME:       timepoint.h
*
*    DESCRIPTION:     Class time point
************************************************************************/

#pragma once

// Game lib dependencies
#include <utilities/timeduration.h>

// Standard lib dependencies
#include <chrono>

class CTimePoint
{
public:

    // Constructor
    CTimePoint() :
        m_timePoint( std::chrono::high_resolution_clock::now() )
    {}

    CTimePoint(const CTimePoint & tp) :
        m_timePoint( tp.m_timePoint )
    {}

    CTimePoint(const CTimeDuration & td) :
        m_timePoint( std::chrono::high_resolution_clock::now() + td.getDuration() )
    {}

    CTimePoint(int64_t value) :
        m_timePoint( std::chrono::high_resolution_clock::now() + std::chrono::nanoseconds(value) )
    {}

    CTimePoint(const std::chrono::high_resolution_clock::time_point & tp) :
        m_timePoint( tp )
    {}

    // Init the time point to now
    void now()
    {
        m_timePoint = std::chrono::high_resolution_clock::now();
    }

    void now(const CTimeDuration & td)
    {
        m_timePoint = std::chrono::high_resolution_clock::now() + td.getDuration();
    }

    // Overloaded operators
    CTimeDuration operator - ( const CTimePoint & tp ) const
    {
        CTimeDuration tmp( m_timePoint - tp.m_timePoint);
        return tmp;
    }

    CTimePoint operator - ( const CTimeDuration & td ) const
    {
        CTimePoint tmp( m_timePoint - td.getDuration());
        return tmp;
    }

    CTimePoint operator + ( const CTimeDuration & td ) const
    {
        CTimePoint tmp( m_timePoint + td.getDuration());
        return tmp;
    }

    CTimePoint operator += ( const CTimeDuration & td )
    {
        m_timePoint += td.getDuration();

        return *this;
    }

    CTimePoint operator -= ( const CTimeDuration & td )
    {
        m_timePoint -= td.getDuration();

        return *this;
    }

    bool operator == ( const CTimePoint & tp ) const
    {
        return m_timePoint == tp.m_timePoint;
    }

    bool operator != ( const CTimePoint & tp ) const
    {
        return m_timePoint != tp.m_timePoint;
    }

    bool operator > ( const CTimePoint & tp ) const
    {
        return m_timePoint > tp.m_timePoint;
    }

    bool operator < ( const CTimePoint & tp ) const
    {
        return m_timePoint < tp.m_timePoint;
    }

    bool operator <= ( const CTimePoint & tp ) const
    {
        return m_timePoint <= tp.m_timePoint;
    }

    bool operator >= ( const CTimePoint & tp ) const
    {
        return m_timePoint >= tp.m_timePoint;
    }

private:

    // Time point of this class
    std::chrono::high_resolution_clock::time_point m_timePoint;
};
