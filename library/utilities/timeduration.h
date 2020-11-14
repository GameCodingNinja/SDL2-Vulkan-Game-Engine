
/************************************************************************
*    FILE NAME:       timeduration.h
*
*    DESCRIPTION:     Class timeduration
************************************************************************/

#pragma once

// Standard lib dependencies
#include <chrono>
#include <iomanip>
#include <sstream>

class CTimeDuration
{
public:

    // Constructor
    CTimeDuration()
    {
    }

    CTimeDuration(const CTimeDuration & td)
    {
        m_duration = td.m_duration;
    }

    CTimeDuration(const std::chrono::nanoseconds & duration)
    {
        m_duration = duration;
    }

    CTimeDuration(int64_t value)
    {
        m_duration = std::chrono::nanoseconds(value);
    }

    CTimeDuration(int64_t h, int64_t m, int64_t s, int64_t mil)
    {
        set(h, m, s, mil);
    }

    // Set the duration
    void set(int64_t h, int64_t m, int64_t s, int64_t mil)
    {
        m_duration =
            std::chrono::hours(h) +
            std::chrono::minutes(m) +
            std::chrono::seconds(s) +
            std::chrono::milliseconds(mil);
    }

    // Set the duration in nanoseconds
    void setNanoseconds(int64_t value)
    {
        m_duration = std::chrono::nanoseconds(value);
    }

    // Set the duration in microseconds
    void setMicroseconds(int64_t value)
    {
        m_duration = std::chrono::microseconds(value);
    }

    // Set the duration in miliseconds
    void setMilliseconds(int64_t value)
    {
        m_duration = std::chrono::milliseconds(value);
    }

    // Set the duration in miliseconds
    void setSeconds(int64_t value)
    {
        m_duration = std::chrono::seconds(value);
    }

    // Set the duration in minutes
    void setMinutes(int64_t value)
    {
        m_duration = std::chrono::minutes(value);
    }

    // Set the duration in hours
    void setHours(int64_t value)
    {
        m_duration = std::chrono::hours(value);
    }

    // Add the duration in nanoseconds
    void addNanoseconds(int64_t value)
    {
        m_duration += std::chrono::nanoseconds(value);
    }

    // Add the duration in microseconds
    void addMicroseconds(int64_t value)
    {
        m_duration += std::chrono::microseconds(value);
    }

    // Add the duration in miliseconds
    void addMilliseconds(int64_t value)
    {
        m_duration += std::chrono::milliseconds(value);
    }

    // Add the duration in miliseconds
    void addSeconds(int64_t value)
    {
        m_duration += std::chrono::seconds(value);
    }

    // Add the duration in minutes
    void addMinutes(int64_t value)
    {
        m_duration += std::chrono::minutes(value);
    }

    // Add the duration in hours
    void addHours(int64_t value)
    {
        m_duration += std::chrono::hours(value);
    }

    // Get duration
    const std::chrono::nanoseconds & getDuration() const
    {
        return m_duration;
    }

    // Get duration in nanoseconds
    int64_t getNanoseconds()
    {
        return m_duration.count();
    }

    // Get duration in microseconds
    int64_t getMicroseconds()
    {
        return std::chrono::duration_cast<std::chrono::microseconds>(m_duration).count();
    }

    // Get duration in miliseconds
    int64_t getMilliseconds()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(m_duration).count();
    }

    // Get duration in seconds
    int64_t getSeconds()
    {
        return std::chrono::duration_cast<std::chrono::seconds>(m_duration).count();
    }

    // Get duration in minutes
    int64_t getMinutes()
    {
        return std::chrono::duration_cast<std::chrono::minutes>(m_duration).count();
    }

    // Get duration in hours
    int64_t getHours()
    {
        return std::chrono::duration_cast<std::chrono::hours>(m_duration).count();
    }

    // Overloaded operators
    CTimeDuration operator - ( const CTimeDuration & td ) const
    {
        CTimeDuration tmp( m_duration - td.m_duration);
        return tmp;
    }

    CTimeDuration operator + ( const CTimeDuration & td ) const
    {
        CTimeDuration tmp( m_duration + td.m_duration);
        return tmp;
    }

    CTimeDuration operator += ( const CTimeDuration & td )
    {
        m_duration += td.m_duration;

        return *this;
    }

    CTimeDuration operator -= ( const CTimeDuration & td )
    {
        m_duration -= td.m_duration;

        return *this;
    }

    bool operator == ( const CTimeDuration & td ) const
    {
        return m_duration == td.m_duration;
    }

    bool operator != ( const CTimeDuration & td ) const
    {
        return m_duration != td.m_duration;
    }

    bool operator > ( const CTimeDuration & td ) const
    {
        return m_duration > td.m_duration;
    }

    bool operator < ( const CTimeDuration & td ) const
    {
        return m_duration < td.m_duration;
    }

    bool operator <= ( const CTimeDuration & td ) const
    {
        return m_duration <= td.m_duration;
    }

    bool operator >= ( const CTimeDuration & td ) const
    {
        return m_duration >= td.m_duration;
    }

private:

    std::chrono::nanoseconds m_duration;
};
