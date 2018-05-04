
/************************************************************************
*    FILE NAME:       loadscreenanimationinfo.h
*
*    DESCRIPTION:     Load screen animation info
************************************************************************/

#ifndef __load_screen_animation_info_h__
#define __load_screen_animation_info_h__

// Game lib dependencies
#include <common/point.h>

// Standard lib dependencies
#include <memory>

// Forward declaration(s)
class CSprite2D;
class CObjectData2D;

class CLoadScrnAnim
{
public:

    CLoadScrnAnim() :
        m_fadeTime(0),
        m_displayDelay(0),
        m_endDelay(0),
        m_frameCount(0),
        m_fps(0),
        m_counter(0)
    {}
      
    void allocateSprite( const CObjectData2D & objectData )
    { m_spSprite2D = std::make_shared<CSprite2D>( objectData ); }
    
    std::shared_ptr<CSprite2D> & getSprite()
    { return m_spSprite2D; }
    
    void setFadeTime( int fadeTime )
    { m_fadeTime = fadeTime; }
    
    int getFadeTime()
    { return m_fadeTime; }

    // Delay of displaying the art
    void setDisplayDelay( int displayDelay )
    { m_displayDelay = displayDelay; }
    
    int getDisplayDelay()
    { return m_displayDelay; }
    
    // Delay after animations
    void setEndDelay( int spaceDelay )
    { m_endDelay = spaceDelay; }
    
    int getEndDelay()
    { return m_endDelay; }
    
    
    void setFrameCount( int frameCount )
    { m_frameCount = frameCount; }
    
    int getFrameCount()
    { return m_frameCount; }
    
    
    void setFPS( int fps )
    { m_fps = fps; }
    
    int getFPS()
    { return m_fps; }
    
    
    void incCounter()
    { ++m_counter; }
    
    int getCounter()
    { return m_counter; }
    
    

private:
    
    std::shared_ptr<CSprite2D> m_spSprite2D;

    int m_fadeTime;
    int m_displayDelay;
    int m_endDelay;
    int m_frameCount;
    float m_fps;
    int m_counter;
};

#endif  // __load_screen_animation_info_h__


