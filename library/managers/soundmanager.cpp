
/************************************************************************
*    FILE NAME:       soundmanager.cpp
*
*    DESCRIPTION:     Sound Manager class singleton
************************************************************************/

// Physical component dependency
#include <managers/soundmanager.h>

// Game lib dependencies
#include <utilities/xmlParser.h>
#include <utilities/exceptionhandling.h>
#include <utilities/genfunc.h>
#include <utilities/settings.h>

// SDL lib dependencies
#include <SDL_mixer.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Standard lib dependencies
#include <cstring>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CSoundMgr::CSoundMgr() :
    m_mixChannel(0),
    m_maxMixChannels(MIX_CHANNELS)
{
    // Init for the OGG compressed file format
    if( Mix_Init(MIX_INIT_OGG) == 0 )
    {
        NGenFunc::PostDebugMsg( boost::str( boost::format("Sound mixer init error (%s).\n\n%s\nLine: %s")
                    % SDL_GetError() % __FUNCTION__ % __LINE__ ) );
    }

    // Setup the audio format
    // High frenquency plus low chunk size = low latency audio playback
    if( Mix_OpenAudio(
        CSettings::Instance().getFrequency(),     // Usually 22050 or 44100
        MIX_DEFAULT_FORMAT,
        CSettings::Instance().getSoundChannels(), // mono, stero, quad, suround, etc
        CSettings::Instance().getChunkSize() ) == 0 )
    {
        NGenFunc::PostDebugMsg( boost::str( boost::format("Sound mixer open error (%s).\n\n%s\nLine: %s")
                        % SDL_GetError() % __FUNCTION__ % __LINE__ ) );
    }

    if( CSettings::Instance().getMixChannels() != m_maxMixChannels )
        m_maxMixChannels = Mix_AllocateChannels( CSettings::Instance().getMixChannels() );
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CSoundMgr::~CSoundMgr()
{
    // Free all sounds in all groups
    for( auto & mapMapIter : m_soundMapMap )
    {
        for( auto & mapIter : mapMapIter.second )
        {
            mapIter.second.free();
        }
    }
}


/************************************************************************
*    DESC:  Load all of the sounds of a specific group
*
*    param: string & group - specified group of scripts to load
************************************************************************/
void CSoundMgr::loadGroup( const std::string & group )
{
    // Make sure the group we are looking has been defined in the list table file
    auto listTableIter = m_listTableMap.find( group );
    if( listTableIter == m_listTableMap.end() )
        throw NExcept::CCriticalException("Sound List Load Group Data Error!",
            boost::str( boost::format("Sound list group name can't be found (%s).\n\n%s\nLine: %s")
                % group % __FUNCTION__ % __LINE__ ));

    // Load the group data if it doesn't already exist
    if( m_soundMapMap.find( group ) == m_soundMapMap.end() )
    {
        for( auto & iter : listTableIter->second )
            load( group, iter );
    }
    else
    {
        throw NExcept::CCriticalException("Sound Data List 2D load Error!",
            boost::str( boost::format("Sound data list group has alread been loaded (%s).\n\n%s\nLine: %s")
                % group % __FUNCTION__ % __LINE__ ));
    }
}


/************************************************************************
*    DESC:  Load all object information from an xml
*
*	 param:	string & filePath - file path of the object data list XML
************************************************************************/
void CSoundMgr::load( const std::string & group, const std::string & filePath )
{
    // Open and parse the XML file:
    const XMLNode mainNode = XMLNode::openFileHelper( filePath.c_str(), "soundList" );

    // Create a new map inside of our map and get an iterator into it
    auto soundMapIter = m_soundMapMap.emplace( group, std::map<const std::string, CSound>() ).first;

    // Get the node to the sound files
    const XMLNode soundFilesNode = mainNode.getChildNode( "soundFiles" );

    const CSound::ESoundType typeEnumArry[] = { CSound::EST_LOADED, CSound::EST_STREAM };
    const char typeCharArry[CSound::EST_MAX][10] = { "load", "stream" };

    for( uint i = 0; i < CSound::EST_MAX; ++i )
    {
        // Get the number of load children
        int count = soundFilesNode.nChildNode( typeCharArry[i] );

        for( int j = 0; j < count; ++j )
        {
            // Get the object data node
            const XMLNode loadNode = soundFilesNode.getChildNode(typeCharArry[i], j);

            // Get the id
            const std::string id = loadNode.getAttribute( "id" );

            // Add the sound data to the map
            auto iter = soundMapIter->second.emplace( id, typeEnumArry[i] );

            // Check for duplicate id names
            if( !iter.second )
            {
                throw NExcept::CCriticalException("Sound Data Load Group Error!",
                    boost::str( boost::format("Duplicate sound ID (%s - %s).\n\n%s\nLine: %s")
                        % id % group % __FUNCTION__ % __LINE__ ));
            }

            // Now try to load the sound
            iter.first->second.loadFromNode( loadNode );
        }
    }

    // Get the node to the play lists
    const XMLNode playListSetNode = mainNode.getChildNode( "playlistSet" );
    if( !playListSetNode.isEmpty() )
    {
        // Create a new map inside of our map and get an iterator into it
        auto playListMapIter = m_playListMapMap.emplace( group, std::map<const std::string, CPlayList>() ).first;

        for( int i = 0; i < playListSetNode.nChildNode(); ++i )
        {
            // Get the play list node
            const XMLNode playListNode = playListSetNode.getChildNode(i);

            // Get the id
            const std::string id = playListNode.getAttribute( "id" );

            // Add the playlist data to the map
            auto iter = playListMapIter->second.emplace( id, std::string(playListNode.getAttribute( "playtype" )) );

            // Check for duplicate names
            if( !iter.second )
            {
                throw NExcept::CCriticalException("Playlist Data Group Load Error!",
                    boost::str( boost::format("Duplicate playlist name! (%s - %s).\n\n%s\nLine: %s")
                        % id % group % __FUNCTION__ % __LINE__ ));
            }

            iter.first->second.loadFromNode( playListNode, group, soundMapIter->second );
        }
    }
}


/************************************************************************
*    DESC:  Free a sound group
************************************************************************/
void CSoundMgr::freeGroup( const std::string & group )
{
    // Free the sound group if it exists
    auto soundMapIter = m_soundMapMap.find( group );
    if( soundMapIter != m_soundMapMap.end() )
    {
        // Free all the sounds in this group
        for( auto & mapIter : soundMapIter->second )
            mapIter.second.free();

        // Erase this group
        m_soundMapMap.erase( soundMapIter );
    }

    // Free the playlist group if it exists
    auto playLstMapIter = m_playListMapMap.find( group );
    if( playLstMapIter != m_playListMapMap.end() )
        m_playListMapMap.erase( playLstMapIter );
}


/************************************************************************
*    DESC:  Get the sound
************************************************************************/
CSound & CSoundMgr::getSound( const std::string & group, const std::string & soundID )
{
    // Check if this is a playlist sound ID
    auto playListMapIter = m_playListMapMap.find( group );
    if( playListMapIter != m_playListMapMap.end() )
    {
        auto iter = playListMapIter->second.find( soundID );
        if( iter != playListMapIter->second.end() )
            return iter->second.getSound();
    }

    auto soundMapIter = m_soundMapMap.find( group );
    if( soundMapIter == m_soundMapMap.end() )
    {
        NGenFunc::PostDebugMsg( boost::str( boost::format("Sound group can't be found (%s).") % group ) );
        return m_null_sound;
    }

    auto iter = soundMapIter->second.find( soundID );
    if( iter == soundMapIter->second.end() )
    {
        NGenFunc::PostDebugMsg( boost::str( boost::format("Sound ID can't be found (%s - %s).") % group % soundID ) );
        return m_null_sound;
    }

    return iter->second;
}


/************************************************************************
*    DESC:  Get the playlist
************************************************************************/
CPlayList & CSoundMgr::getPlayList( const std::string & group, const std::string & playLstID )
{
    auto playListMapIter = m_playListMapMap.find( group );
    if( playListMapIter != m_playListMapMap.end() )
    {
        auto iter = playListMapIter->second.find( playLstID );
        if( iter != playListMapIter->second.end() )
            return iter->second;
    }

    return m_null_playLst;
}


/************************************************************************
*    DESC:  Get the next channel
************************************************************************/
int CSoundMgr::getNextChannel()
{
    m_mixChannel = (m_mixChannel + 1) % m_maxMixChannels;
    return m_mixChannel;
}


/************************************************************************
*    DESC:  Play a sound
************************************************************************/
void CSoundMgr::play( const std::string & group, const std::string & soundID, int loopCount )
{
    getSound( group, soundID ).play( getNextChannel(), loopCount );
}


/************************************************************************
*    DESC:  Pause a sound
************************************************************************/
void CSoundMgr::pause( const std::string & group, const std::string & soundID )
{
    getSound( group, soundID ).pause();
}


/************************************************************************
*    DESC:  Resume a sound
************************************************************************/
void CSoundMgr::resume( const std::string & group, const std::string & soundID )
{
    getSound( group, soundID ).resume();
}


/************************************************************************
*    DESC:  Resume a sound
************************************************************************/
void CSoundMgr::stop( const std::string & group, const std::string & soundID )
{
    getSound( group, soundID ).stop();
}


/************************************************************************
*    DESC: Set/Get the volume for music or channel
************************************************************************/
void CSoundMgr::setVolume( const std::string & group, const std::string & soundID, int volume )
{
    getSound( group, soundID ).setVolume( volume );
}

int CSoundMgr::getVolume( const std::string & group, const std::string & soundID )
{
    return getSound( group, soundID ).getVolume();
}


/************************************************************************
*    DESC:  Is music or channel playing?
************************************************************************/
bool CSoundMgr::isPlaying( const std::string & group, const std::string & soundID )
{
    return getSound( group, soundID ).isPlaying();
}


/************************************************************************
*    DESC:  Is music or channel paused?
************************************************************************/
bool CSoundMgr::isPaused( const std::string & group, const std::string & soundID )
{
    return getSound( group, soundID ).isPaused();
}


/************************************************************************
*    DESC:  Stop all playing sound
************************************************************************/
void CSoundMgr::stopAllSound()
{
    for( auto & mapMapIter : m_soundMapMap )
    {
        for( auto & mapIter : mapMapIter.second )
        {
            mapIter.second.stop();
        }
    }
}


/************************************************************************
*    DESC:  Is the stream playing?
************************************************************************/
bool CSoundMgr::isStreamPlaying()
{
    return Mix_PlayingMusic();
}


/************************************************************************
*    DESC:  Is the stream paused?
************************************************************************/
bool CSoundMgr::isStreamPaused()
{
    return Mix_PausedMusic();
}


/************************************************************************
*    DESC:  Stop the stream
************************************************************************/
void CSoundMgr::stopStream()
{
    Mix_HaltMusic();
}


/************************************************************************
*    DESC:  Pause the music
************************************************************************/
void CSoundMgr::pauseStream()
{
    if( Mix_PlayingMusic() )
        Mix_PauseMusic();
}
