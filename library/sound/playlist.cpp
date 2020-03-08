
/************************************************************************
*    FILE NAME:       playlist.cpp
*
*    DESCRIPTION:     Play list class
************************************************************************/

// Physical component dependency
#include <sound/playlist.h>

// Game lib dependencies
#include <utilities/xmlParser.h>
#include <utilities/exceptionhandling.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Standard lib dependencies
#include <random>
#include <algorithm>
#include <cstring>

CSound CPlayList::m_null_Sound;

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CPlayList::CPlayList( const std::string strType ) :
    m_counter(0),
    m_current(0),
    m_type( (strType == "random") ? EST_RANDOM : EST_SEQUENTIAL )
      
{
}

CPlayList::CPlayList() :
    m_counter(0),
    m_current(0),
    m_type(EST_NULL)
      
{
}


/************************************************************************
*    DESC:  Copy Constructor
************************************************************************/
CPlayList::CPlayList( const CPlayList & playLst ) :
    m_counter(playLst.m_counter),
    m_current(playLst.m_current),
    m_type(playLst.m_type),
    m_soundVec(playLst.m_soundVec)
{
}


/************************************************************************
*    DESC:  destructor                                                             
************************************************************************/
CPlayList::~CPlayList()
{
}


/************************************************************************
*    DESC:  Load  the playlist from node                                                            
************************************************************************/
void CPlayList::loadFromNode(
    const XMLNode & node,
    const std::string & group,
    std::map< const std::string, CSound > & soundMap )
{
    for( int i = 0; i < node.nChildNode(); ++i )
    {
        // Get the sound list node
        const XMLNode soundNode = node.getChildNode(i);

        // Get the id
        const std::string id = soundNode.getAttribute( "id" );

        // Copy the sound to the playlist
        auto soundIter = soundMap.find( id );
        if( soundIter != soundMap.end() )
        {
            m_soundVec.push_back( soundIter->second );

            // Set the volume if defined
            if( soundNode.isAttributeSet("volume") )
                m_soundVec.back().setVolume( std::atoi(soundNode.getAttribute( "volume" )) );
        }
        else
        {
            throw NExcept::CCriticalException("Playlist Data Group Load Error!",
                boost::str( boost::format("Playlist sound Id does not exist! (%s - %s).\n\n%s\nLine: %s")
                    % id % group % __FUNCTION__ % __LINE__ ));
        }
    } 
}


/************************************************************************
*    DESC:  Get the sound for the playlist                                                             
************************************************************************/
CSound & CPlayList::getSound()
{
    if( !m_soundVec.empty() )
    {
        // Is it time to shuffle?
        if( (m_type == EST_RANDOM) && (m_counter == 0) )
            shuffle();

        m_current = m_counter;
        m_counter = (m_counter + 1) % m_soundVec.size();

        return m_soundVec[m_current];
    }
    
    return m_null_Sound;
}


/************************************************************************
*    DESC:  Play the play list
************************************************************************/
void CPlayList::play( int channel, int loopCount )
{
    if( !m_soundVec.empty() )
    {
        if( (m_type == EST_RANDOM) && (m_counter == 0) )
            shuffle();

        m_current = m_counter;
        m_soundVec[m_current].play( channel, loopCount );
        m_counter = (m_counter + 1) % m_soundVec.size();
    }
}


/************************************************************************
*    DESC:  Stop the sound
************************************************************************/
void CPlayList::stop()
{
    if( !m_soundVec.empty() )
        m_soundVec[m_current].stop();
}


/************************************************************************
*    DESC:  Pause the sound
************************************************************************/
void CPlayList::pause()
{
    if( !m_soundVec.empty() )
        m_soundVec[m_current].pause();
}


/************************************************************************
*    DESC:  Resume the sound
************************************************************************/
void CPlayList::resume()
{
    if( !m_soundVec.empty() )
        m_soundVec[m_current].resume();
}


/************************************************************************
*    DESC: Set/Get the volume for music or channel
************************************************************************/
void CPlayList::setVolume( int volume )
{
    if( !m_soundVec.empty() )
        m_soundVec[m_current].setVolume( volume );
}

int CPlayList::getVolume()
{
    if( !m_soundVec.empty() )
        return m_soundVec[m_current].getVolume();
    
    return m_null_Sound.getVolume();
}


/************************************************************************
*    DESC:  Is music or channel playing?
************************************************************************/
bool CPlayList::isPlaying()
{
    if( !m_soundVec.empty() )
        return m_soundVec[m_current].isPlaying();
    
    return m_null_Sound.isPlaying();
}


/************************************************************************
*    DESC:  Is music or channel paused?
************************************************************************/
bool CPlayList::isPaused()
{
    if( !m_soundVec.empty() )
        return m_soundVec[m_current].isPaused();
    
    return m_null_Sound.isPaused();
}


/************************************************************************
*    DESC:  Play the play list
************************************************************************/
void CPlayList::shuffle()
{
    if( m_soundVec.size() > 2 )
    {
        // Get the last sound that was just played
        CSound oldLastSound(m_soundVec.back());
        
        // Shuffle
        std::random_device rd;
        std::default_random_engine g(rd());
        std::shuffle( m_soundVec.begin(), m_soundVec.end(), g );
        
        // Make sure the new first sound is not the old last sound.
        // Don't want the same two sounds playing back to back
        // If it is just stick it in the middle
        if( oldLastSound == m_soundVec.front() )
        {
            int midPos = m_soundVec.size() / 2;
            m_soundVec[0] = m_soundVec[midPos];
            m_soundVec[midPos] = oldLastSound;
        }
    }
}
