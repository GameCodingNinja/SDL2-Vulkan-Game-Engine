
/************************************************************************
*    FILE NAME:       gamesave.cpp
*
*    DESCRIPTION:     Singleton class for saving game data
************************************************************************/

// Physical component dependency
#include "gamesave.h"

// Standard lib dependencies

// Boost lib dependencies

// Game lib dependencies
#include <utilities/exceptionhandling.h>

// SDL/OpenGL lib dependencies
#include <SDL.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CGameSave::CGameSave() :
    m_file(nullptr)
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CGameSave::~CGameSave()
{
    close();
}


/************************************************************************
*    DESC:  Load the game data
************************************************************************/
void CGameSave::load()
{
    if( open() )
    {
        save();
    }
    else
    {
        // File was opened so read the data
        if( SDL_RWread( m_file, &m_saveData, 1, sizeof(m_saveData)) == 0 )
            throw NExcept::CCriticalException("Error reading game save data!", "No data was read in!" );
    }

    close();
}


/************************************************************************
*    DESC:  Save/Save/Close the game data
************************************************************************/
void CGameSave::openSaveClose()
{
    open();
    save();
    close();
}


/************************************************************************
*    DESC:  Open the game data
************************************************************************/
bool CGameSave::open()
{
    bool result(false);

    if( m_file == nullptr )
    {
        std::string filePath;

        #if defined(__ANDROID__)
            char * pChar = SDL_GetPrefPath("HugesWhoProduction", "HugesWhoSlots");
            filePath = pChar;
            filePath.append( "gamesave.dat" );
            SDL_free( pChar );
        #else
            filePath = "data/settings/gamesave.dat";
        #endif

        // Does the save file exist?
        if( (m_file = SDL_RWFromFile( filePath.c_str(), "rb+" )) == nullptr )
        {
            // Create a new one from scratch
            if( (m_file = SDL_RWFromFile( filePath.c_str(), "wb+" )) == nullptr )
                throw NExcept::CCriticalException("Error creating save file!", SDL_GetError() );

            result = true;
        }
    }
    else
    {
        throw NExcept::CCriticalException("Game Save File Error!", "Game save file has already been opened." );
    }

    return result;
}


/************************************************************************
*    DESC:  Save the game data
************************************************************************/
void CGameSave::save()
{
    if( m_file == nullptr )
        throw NExcept::CCriticalException("Game Save File Error!", "Game save file has NOT been loaded!" );

    SDL_RWseek( m_file, 0, RW_SEEK_SET );

    if( SDL_RWwrite( m_file, &m_saveData, 1, sizeof(m_saveData)) != sizeof(m_saveData) )
        throw NExcept::CCriticalException("Game Save File Error!", "Not all the data was written to file!" );
}


/************************************************************************
*    DESC:  Close the game data file
************************************************************************/
void CGameSave::close()
{
    if( m_file != nullptr )
    {
        SDL_RWclose(m_file);
        m_file = nullptr;
    }
}


/************************************************************************
*    DESC:  Set/Get play lobby music
************************************************************************/
void CGameSave::setPlayLobbyMusic( bool value )
{
    m_saveData.m_playLobbyMusic = value;
}

bool CGameSave::getPlayLobbyMusic() const
{
    return m_saveData.m_playLobbyMusic;
}


/************************************************************************
*    DESC:  Set/Get play stop sounds
************************************************************************/
void CGameSave::setPlayStopSounds( bool value )
{
    m_saveData.m_playStopSounds = value;
}

bool CGameSave::getPlayStopSounds() const
{
    return m_saveData.m_playStopSounds;
}


/************************************************************************
*    DESC:  Set/Get play spin music
************************************************************************/
void CGameSave::setPlaySpinMusic( bool value )
{
    m_saveData.m_playSpinMusic = value;
}

bool CGameSave::getPlaySpinMusic() const
{
    return m_saveData.m_playSpinMusic;
}


/************************************************************************
*    DESC:  Set/Get total credits
************************************************************************/
void CGameSave::setTotalCredits( uint value )
{
    m_saveData.m_totalCredits = value;
}

uint CGameSave::getTotalCredits() const
{
    return m_saveData.m_totalCredits;
}
