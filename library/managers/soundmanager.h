
/************************************************************************
*    FILE NAME:       soundmanager.h
*
*    DESCRIPTION:     Sound Manager class singleton
************************************************************************/

#ifndef __sound_manager_h__
#define __sound_manager_h__

// Physical component dependency
#include <managers/managerbase.h>

// Game lib dependencies
#include <common/sound.h>
#include <common/playlist.h>

class CSoundMgr : public CManagerBase
{
public:

    // Get the instance of the singleton class
    static CSoundMgr & Instance()
    {
        static CSoundMgr soundMgr;
        return soundMgr;
    }

    // Load all of the sounds of a specific group
    void loadGroup( const std::string & group );
    
    // Free a sound group
    void freeGroup( const std::string & group );
    
    // Get the next channel
    int getNextChannel();

    // Play a sound
    void play( const std::string & group, const std::string & soundID, int loopCount = 0 );

    // Pause a sound
    void pause( const std::string & group, const std::string & soundID );

    // Resume a sound
    void resume( const std::string & group, const std::string & soundID );

    // Resume a sound
    void stop( const std::string & group, const std::string & soundID );
    
    // Set volume for music or channel
    void setVolume( const std::string & group, const std::string & soundID, int volume );
    int getVolume( const std::string & group, const std::string & soundID );
    
    // Is music or channel playing?
    bool isPlaying( const std::string & group, const std::string & soundID );
    
    // Is music or channel paused?
    bool isPaused( const std::string & group, const std::string & soundID );

    // Get the sound
    CSound & getSound( const std::string & group, const std::string & soundID );
    
    // Get the playlist
    CPlayList & getPlayList( const std::string & group, const std::string & playLstID );
    
    // Stop all playing sound
    void stopAllSound();
    
    // Is the stream playing?
    bool isStreamPlaying();
    
    // Is the stream paused?
    bool isStreamPaused();
    
    // Stop the stream
    void stopStream();
    
    // Pause the stream
    void pauseStream();

private:

    // Constructor
    CSoundMgr();

    // Destructor
    virtual ~CSoundMgr();

    // Load all object information from an xml
    void load( const std::string & group, const std::string & filePath );

private:

    // Map containing a group of sound ID's
    std::map< const std::string, std::map< const std::string, CSound > > m_soundMapMap;

    // Map containing a group of play list ID's
    // Do not free the sounds copied to the play list
    std::map< const std::string, std::map< const std::string, CPlayList > > m_playListMapMap;

    // mix channel for the next sound
    int m_mixChannel;

    // max mix channels
    int m_maxMixChannels;
    
    // Null members
    CPlayList m_null_playLst;
    CSound m_null_sound;

};

#endif  // __sound_manager_h__
