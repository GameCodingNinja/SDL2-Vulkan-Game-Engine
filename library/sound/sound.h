/************************************************************************
*    FILE NAME:       sound.h
*
*    DESCRIPTION:     Class to hold the sound pointer and type
************************************************************************/  

#pragma once

// Standard lib dependencies
#include <string>
#include <cstdint>

// Forward declaration(s)
struct XMLNode;

class CSound
{
public:

    enum ESoundType
    {
        EST_NULL=0,
        EST_LOADED,
        EST_STREAM,
        EST_MAX,
    };

    CSound( ESoundType type );
    CSound( const CSound & sound );
    CSound();
    ~CSound();

    // Load the sound
    void loadFromNode( const XMLNode & node );

    // Play the sound
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

    // Find an open channel and set the class member
    void setOpenChannel();

    // Free the sound
    void free();
    
    // equality/inequality operator
    bool operator == ( const CSound & sound ) const;
    bool operator != ( const CSound & sound ) const;

private:
    
    // Sound type - loaded or stream
    ESoundType m_type;
    
    // Voided pointer to hold the different sound type
    void * m_pVoid;

    // Channel the sound is currently running on
    int16_t m_channel;
    
    // Sounds current volume
    int16_t m_volume;
};
