
/************************************************************************
*    FILE NAME:       basegamemusic.h
*
*    DESCRIPTION:     Class for handling base game music
************************************************************************/

#ifndef __simplecycleresults_h__
#define __simplecycleresults_h__

// Physical component dependency
#include <slot/igamemusic.h>

// Game lib dependencies
#include <script/scriptcomponent.h>
#include <utilities/timer.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <string>

class CBaseGameMusic : public iGameMusic, boost::noncopyable
{
public:

    // Constructor
    CBaseGameMusic(
        const std::string & group,
        const std::string & startMusicFunc,
        const std::string & stopMusicFunc,
        const std::string & fastFadeFunc,
        int musicTimeOut );
    
    // Handle update checks
    void update() override;
    
    // Start the music
    void startMusic() override;
    
    // Start the music timeout
    void setTimeOut() override;
    
    // Fast a fade down. Could be exiting the game
    void fastFadeDown();
    
    // Do we play the music
    void allowMusic( bool allow );
    
private:

    // cycle results timer
    CTimer m_musicTimer;
    
    // Script component object
    CScriptComponent m_scriptComponent;
    
    // Group string
    std::string m_group;
    
    // Spin start and stop music function calls
    std::string m_startMusicFunc;
    std::string m_stopMusicFunc;
    std::string m_fastFadeFunc;
    
    // Music time out
    const int m_musicTimeOut;
    
    // Flag to indicate spin music can be played
    bool m_allowSpinMusic;
};

#endif
