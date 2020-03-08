
/************************************************************************
*    FILE NAME:       sound.cpp
*
*    DESCRIPTION:     Class to hold the sound pointer and type
************************************************************************/

// Physical component dependency
#include <sound/sound.h>

// Game lib dependencies
#include <utilities/exceptionhandling.h>
#include <utilities/genfunc.h>
#include <utilities/xmlParser.h>

// Boost lib dependencies
#include <boost/format.hpp>

// SDL lib dependencies
#include <SDL2/SDL_mixer.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CSound::CSound( ESoundType type ) :
    m_type(type),
    m_pVoid(nullptr),
    m_channel(-1),
    m_volume(MIX_MAX_VOLUME)
{
}

CSound::CSound()
{
}


/************************************************************************
*    DESC:  Copy Constructor
************************************************************************/
CSound::CSound( const CSound & sound ) :
    m_type(sound.m_type),
    m_pVoid(sound.m_pVoid),
    m_channel(sound.m_channel),
    m_volume(sound.m_volume)
{
}


/************************************************************************
*    DESC:  destructor                                                             
************************************************************************/
CSound::~CSound()
{
    // Don't free the sound here because this destructor get's
    // called when the reference is passed around.
}


/************************************************************************
*    DESC:  Free the sound                                                             
************************************************************************/
void CSound::free()
{
    stop();
    
    if( m_type == EST_LOADED )
        Mix_FreeChunk( (Mix_Chunk *)m_pVoid );

    else if( m_type == EST_STREAM )
        Mix_FreeMusic( (Mix_Music *)m_pVoid );
}


/************************************************************************
*    DESC:  Load the sound                                                             
************************************************************************/
void CSound::loadFromNode( const XMLNode & node )
{
    auto file = node.getAttribute( "file" );
            
    if( m_type == EST_LOADED )
        m_pVoid = Mix_LoadWAV( file );

    else if( m_type == EST_STREAM )
        m_pVoid = Mix_LoadMUS( file );
    
    // Set the volume if defined
    if( node.isAttributeSet("volume") )
        setVolume( std::atoi(node.getAttribute( "volume" )) );

    if( m_pVoid == nullptr )
        throw NExcept::CCriticalException("Sound load Error!",
            boost::str( boost::format("Error loading sound (%s)(%s).\n\n%s\nLine: %s")
                % SDL_GetError() % file % __FUNCTION__ % __LINE__ ));
}


/************************************************************************
*    DESC:  Play the sound
*    NOTE: Loop and channel default to -1
************************************************************************/
void CSound::play( int channel, int loopCount )
{
    if( m_type == EST_LOADED )
    {
        m_channel = Mix_PlayChannel( channel, (Mix_Chunk *)m_pVoid, loopCount );
        Mix_Volume( m_channel, m_volume );
    }
    else if( m_type == EST_STREAM )
        Mix_PlayMusic( (Mix_Music *)m_pVoid, loopCount );
}


/************************************************************************
*    DESC:  Stop the sound
************************************************************************/
void CSound::stop()
{
    if( m_type == EST_LOADED )
    {
        if( (m_channel > -1) && Mix_Playing( m_channel ) )
            Mix_HaltChannel( m_channel );
    }
    else if( m_type == EST_STREAM )
    {
        if( Mix_PlayingMusic() )
            Mix_HaltMusic();
    }
}


/************************************************************************
*    DESC:  Pause the sound
************************************************************************/
void CSound::pause()
{
    if( m_type == EST_LOADED )
    {
        if( (m_channel > -1) && Mix_Playing( m_channel ) )
            Mix_Pause( m_channel );
    }
    else if( m_type == EST_STREAM )
    {
        if( Mix_PlayingMusic() )
            Mix_PauseMusic();
    }
}


/************************************************************************
*    DESC:  Resume the sound
************************************************************************/
void CSound::resume()
{
    if( m_type == EST_LOADED )
    {
        if( (m_channel > -1) && Mix_Paused( m_channel ) )
            Mix_Resume( m_channel );
    }
    else if( m_type == EST_STREAM )
    {
        if( Mix_PausedMusic() )
            Mix_ResumeMusic();
    }
}


/************************************************************************
*    DESC: Set/Get the volume for music or channel
************************************************************************/
void CSound::setVolume( int volume )
{
    if( m_type == EST_STREAM )
        Mix_VolumeMusic( volume );

    m_volume = volume;
}

int CSound::getVolume()
{
    if( m_type == EST_LOADED )
        m_volume = Mix_Volume( m_channel, -1 );

    else if( m_type == EST_STREAM )
        m_volume = Mix_VolumeMusic( -1 );
    
    return m_volume;
}


/************************************************************************
*    DESC:  Is music or channel playing?
************************************************************************/
bool CSound::isPlaying()
{
    if( m_type == EST_LOADED )
    {
        if( (m_channel > -1) && Mix_Playing( m_channel ) )
            return true;
    }
    else if( m_type == EST_STREAM )
    {
        if( Mix_PlayingMusic() )
            return true;
    }
    
    return false;
}


/************************************************************************
*    DESC:  Is music or channel paused?
************************************************************************/
bool CSound::isPaused()
{
    if( m_type == EST_LOADED )
    {
        if( (m_channel > -1) && Mix_Paused( m_channel ) )
            return true;
    }
    else if( m_type == EST_STREAM )
    {
        if( Mix_PausedMusic() )
            return true;
    }
    
    return false;
}


/************************************************************************
*    DESC:  Find an open channel and set the class member
************************************************************************/
void CSound::setOpenChannel()
{
    int i;
    for( i = 0; i < MIX_CHANNELS; ++i )
    {
        if( !Mix_Playing(i) )
            break;
    }

    if( i == MIX_CHANNELS )
    {
        NGenFunc::PostDebugMsg( "No free channels available" );
    }
    else
    {
        m_channel = i;
    }
}

/************************************************************************
*    DESC:  The equality operator
************************************************************************/
bool CSound::operator == ( const CSound & sound ) const
{
    return (m_pVoid == sound.m_pVoid);
}

/************************************************************************
*    DESC:  The inequality operator
************************************************************************/
bool CSound::operator != ( const CSound & sound ) const
{
    return (m_pVoid != sound.m_pVoid);
}
