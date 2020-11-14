
/************************************************************************
*    FILE NAME:       playlist.h
*
*    DESCRIPTION:     Play list class
************************************************************************/

#pragma once

// Game lib dependencies
#include <sound/sound.h>
#include <common/defs.h>

// Standard lib dependencies
#include <vector>
#include <map>
#include <string>
#include <cstdint>

// Forward declaration(s)
struct XMLNode;

class CPlayList
{
public:

    enum EPlayListType
    {
        EST_NULL=0,
        EST_RANDOM,
        EST_SEQUENTIAL
    };

    // Constructor
    CPlayList( const std::string strType );
    CPlayList( const CPlayList & playLst );
    CPlayList();

    // Destructor
    virtual ~CPlayList();
    
    // Load  the playlist from node
    void loadFromNode(
        const XMLNode & node,
        const std::string & group,
        std::map< const std::string, CSound > & soundMap );

    // Set the sound for the playlist
    CSound & getSound();

    // Play the play list
    void play( int channel = -1, int loopCount = 0 );
    
    // Stop the sound
    void stop();

    // Pause the sound
    void pause();

    // Resume the sound
    void resume();
    
    // Set/Get the volume for music or channel
    void setVolume( int volume );
    int getVolume();
    
    // Is music or channel playing?
    bool isPlaying();
    
    // Is music or channel paused?
    bool isPaused();

private:

    // Play the play list
    void shuffle();
    
private:
    
    // Counter
    int16_t m_counter;
    
    // current index
    int16_t m_current;

    // playlist type - random or sequential
    EPlayListType m_type;

    // vector of sounds
    std::vector<CSound> m_soundVec;
    
    // null sound
    static CSound m_null_Sound;
};
