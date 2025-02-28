/************************************************************************
*    FILE NAME:       highresolutiontimer.h
*
*    DESCRIPTION:     high resolution timer class
************************************************************************/

// Physical component dependency
#include <utilities/highresolutiontimer.h>

// SDL lib dependencies
#include <SDL3/SDL.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CHighResTimer::CHighResTimer()
    : m_inverseTimerFrequency(0.0),
      m_lastTime(uint64_t(0.0)),
      m_elapsedTime(0.0),
      m_fps(0.0f)
{
    // inverse it so that we can do a simple multiplication instead of division
    m_inverseTimerFrequency = 1000.0 / static_cast<double>(SDL_GetPerformanceFrequency());

    // Init the lastTime variable for the first runthrough
    m_lastTime = SDL_GetPerformanceCounter();
}


/************************************************************************
*    DESC:  Destructor
************************************************************************/
CHighResTimer::~CHighResTimer()
{
}


/***************************************************************************
*    DESC:  Simple timer start
****************************************************************************/
void CHighResTimer::timerStart()
{
    // Get the current performance time
    m_timer = SDL_GetPerformanceCounter();
}


/***************************************************************************
*    DESC:  Simple timer duration
****************************************************************************/
float CHighResTimer::timerStop()
{
    // Get the current performance time
    uint64_t time = SDL_GetPerformanceCounter();

    // Get the elapsed time
    return (float)((time - m_timer) * m_inverseTimerFrequency);
}


/***************************************************************************
*    DESC:  Calc the elapsed time
****************************************************************************/
void CHighResTimer::calcElapsedTime()
{
    // Get the current performance time
    uint64_t time = SDL_GetPerformanceCounter();

    // Set the elapsed time
    m_elapsedTime = static_cast<double>(time - m_lastTime) * m_inverseTimerFrequency;

    // Set the fps
    m_fps = static_cast<float>(1000.0f / m_elapsedTime);

    // Catch any hickups - cap to about 10 fps
    // Elapsed time is not expected to get this
    // high in  a game which is why it's capped
    if( m_elapsedTime > 100.0f )
        m_elapsedTime = 100.0f;

    // Reset the last time
    m_lastTime = time;
}


/***************************************************************************
*    DESC:  Get the elapsed time
****************************************************************************/
double CHighResTimer::getElapsedTime()
{
    return m_elapsedTime;
}


/***************************************************************************
*    DESC:  get the elapsed time
****************************************************************************/
float CHighResTimer::getFPS()
{
    return m_fps;
}


/***************************************************************************
*    DESC:  get the raw time
****************************************************************************/
double CHighResTimer::getTime()
{
    // Get the current performance time
    uint64_t time = SDL_GetPerformanceCounter();

    return (double)(time * m_inverseTimerFrequency);
}
