
/************************************************************************
*    FILE NAME:       loadscreenanimationinfo.h
*
*    DESCRIPTION:     Load screen animation info
************************************************************************/

#ifndef __load_screen_animation_info_h__
#define __load_screen_animation_info_h__

// Standard lib dependencies
#include <memory>

// Game lib dependencies
#include <common/point.h>

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
        m_fps(0)
    {}
      
    void AllocateSprite( const CObjectData2D & objectData )
    { m_spSprite2D = std::make_shared<CSprite2D>( objectData ); }
    
    std::shared_ptr<CSprite2D> & GetSprite()
    { return m_spSprite2D; }
    
    void SetFadeTime( int fadeTime )
    { m_fadeTime = fadeTime; }
    
    int GetFadeTime()
    { return m_fadeTime; }

    // Delay of displaying the art
    void SetDisplayDelay( int displayDelay )
    { m_displayDelay = displayDelay; }
    
    int GetDisplayDelay()
    { return m_displayDelay; }
    
    // Delay after animations
    void SetEndDelay( int spaceDelay )
    { m_endDelay = spaceDelay; }
    
    int GetEndDelay()
    { return m_endDelay; }
    
    
    void SetFrameCount( int frameCount )
    { m_frameCount = frameCount; }
    
    int GetFrameCount()
    { return m_frameCount; }
    
    
    void SetFPS( int fps )
    { m_fps = fps; }
    
    int GetFPS()
    { return m_fps; }

private:
    
    std::shared_ptr<CSprite2D> m_spSprite2D;

    int m_fadeTime;
    int m_displayDelay;
    int m_endDelay;
    int m_frameCount;
    float m_fps;
};

#endif  // __load_screen_animation_info_h__


