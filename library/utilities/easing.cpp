//
//  FILE NAME:       easing.cpp
//
//  DESCRIPTION:     Class for holding easing function
//  Reference:       https://github.com/nicolausYes/easing-functions/blob/master/src/easing.cpp
//

// Physical component dependency
#include <utilities/easing.h>

// Game lib dependencies
#include <utilities/highresolutiontimer.h>

// Standard lib dependencies
#include <math.h>

#define M_PI  3.14159265358979323846

namespace NEasing
{
	// 
	//  linear
	//
	float linear(float t)
	{
		return t;
	}

    // 
    //  Quad functions
    //
    float inQuad( float t )
    {
        return t * t;
    }

    float outQuad( float t )
    { 
        return t * (2 - t);
    }

    float inOutQuad( float t )
    {
        return t < 0.5 ? 2 * t * t : t * (4 - 2 * t) - 1;
    }

    // 
    //  Cube functions
    //
    float inCubic( float t )
    {
        return t * t * t;
    }

    float outCubic( float t )
    {
        return 1 + (--t) * t * t;
    }

    float inOutCubic( float t )
    {
        return t < 0.5 ? 4 * t * t * t : 1 + (--t) * (2 * (--t)) * (2 * t);
    }

    // 
    //  Quart functions
    //
    float inQuart( float t )
    {
        t *= t;
        return t * t;
    }

    float outQuart( float t )
    {
        t = (--t) * t;
        return 1 - t * t;
    }

    float inOutQuart( float t )
    {
        if( t < 0.5 )
        {
            t *= t;
            return 8 * t * t;
        }
        else
        {
            t = (--t) * t;
            return 1 - 8 * t * t;
        }
    }

    // 
    // Sine functions
    //
    float inSine( float t )
    {
        return sin( 1.5707963 * t );
    }

    float outSine( float t )
    {
        return 1 + sin( 1.5707963 * (--t) );
    }

    float inOutSine( float t )
    {
        return 0.5 * (1 + sin( 3.1415926 * (t - 0.5) ) );
    }

    // 
    //  Back functions
    //
    float inBack( float t )
    {
        return t * t * (2.70158 * t - 1.70158);
    }

    float outBack( float t )
    {
        return 1 + (--t) * t * (2.70158 * t + 1.70158);
    }

    float inOutBack( float t )
    {
        if( t < 0.5 )
        {
            return t * t * (7 * t - 2.5) * 2;
        }
        else
        {
            return 1 + (--t) * t * 2 * (7 * t + 2.5);
        }
    }

    // 
    //  Circle functions
    //
    float inCirc( float t )
    {
        return 1 - sqrt( 1 - t );
    }

    float outCirc( float t )
    {
        return sqrt( t );
    }

    float inOutCirc( float t )
    {
        if( t < 0.5 )
        {
            return (1 - sqrt( 1 - 2 * t )) * 0.5;
        }
        else
        {
            return (1 + sqrt( 2 * t - 1 )) * 0.5;
        }
    }

    // 
    //  Bounce functions
    //
    float inBounce( float t )
    {
        return pow( 2, 6 * (t - 1) ) * abs( sin( t * M_PI * 3.5 ) );
    }

    float outBounce( float t )
    {
        return 1 - pow( 2, -6 * t ) * abs( cos( t * M_PI * 3.5 ) );
    }

    float inOutBounce( float t )
    {
        if( t < 0.5 )
        {
            return 8 * pow( 2, 8 * (t - 1) ) * abs( sin( t * M_PI * 7 ) );
        }
        else
        {
            return 1 - 8 * pow( 2, -8 * t ) * abs( sin( t * M_PI * 7 ) );
        }
    }

    // 
    //  Elastic functions 
    //
    float inElastic( float t )
    {
        float t2 = t * t;
        return t2 * t2 * sin( t * M_PI * 4.5 );
    }

    float outElastic( float t )
    {
        float t2 = (t - 1) * (t - 1);
        return 1 - t2 * t2 * cos( t * M_PI * 4.5 );
    }

    float inOutElastic( float t )
    {
        float t2;
        if( t < 0.45 )
        {
            t2 = t * t;
            return 8 * t2 * t2 * sin( t * M_PI * 9 );
        }
        else if( t < 0.55 )
        {
            return 0.5 + 0.75 * sin( t * M_PI * 4 );
        }
        else
        {
            t2 = (t - 1) * (t - 1);
            return 1 - 8 * t2 * t2 * sin( t * M_PI * 9 );
        }
    }
}

//
//  Constructor
//
CEasing::CEasing() :
    m_start(0.f),
    m_end(0.f),
    m_current(0.f),
    m_time(0.f),
    m_totalTime(0.f),
    m_dif(0.f),
    m_finished(false),
    m_easingFunc(nullptr)
{
}

//
//  Destructor
//
CEasing::~CEasing()
{
}

// 
//  Init the class for use
//
void CEasing::init( float start, float end, float totalTime, easingFuncPtr easingFunc, bool resetCurrent )
{
    if(resetCurrent)
        m_current = 0.f;
    else
        m_current = start;

    m_start = start;
    m_end = end;
    m_time = 0.f;
    m_totalTime = totalTime * 1000.f;
    m_dif = end - start;
    m_finished = false;
    m_easingFunc = easingFunc;
}

// 
//  Execute this class object
//
void CEasing::execute()
{
    m_time += CHighResTimer::Instance().getElapsedTime();

    if( m_time < m_totalTime )
    {
        m_current = (m_dif * m_easingFunc( m_time / m_totalTime )) + m_start;
    }
    else
    {
        m_finished = true;
    }
}

// 
//  Set back to a pre-init state
//
void CEasing::clear(bool clearEasingFunc)
{
    m_start = 0.f;
    m_end = 0.f;
    m_current = 0.0f;
    m_time = 0.0f;
    m_totalTime = 0.f;
    m_dif = 0.f;
    m_finished = false;

    if(clearEasingFunc)
        m_easingFunc = nullptr;
}

// 
//  Get the current value
//
float CEasing::getValue()
{
    if( m_finished )
        return m_end;

    else
        return m_current;
}

// 
//  Force the easing to finish
//
void CEasing::finish()
{
    m_time = m_totalTime;
    m_finished = true;
    m_current = m_end;
}
    
// 
//  Finished access function
//
bool CEasing::isFinished()
{
    return m_finished;
}

// 
//  Is this class initialized
//
bool CEasing::isInitialized()
{
    return (m_easingFunc != nullptr);
}
