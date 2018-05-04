
/************************************************************************
*    FILE NAME:       gamesave.h
*
*    DESCRIPTION:     Singleton class for saving game data
************************************************************************/

#ifndef __game_save_h__
#define __game_save_h__

// Game lib dependencies
#include <common/defs.h>

// Forward declaration(s)
struct SDL_RWops;

class CGameSave
{
public:
    
    struct SSaveData
    {
        bool m_playLobbyMusic = true;
        bool m_playStopSounds = true;
        bool m_playSpinMusic = true;
        uint m_totalCredits = 5000;
    };
    
    // Get the instance of the singleton class
    static CGameSave & Instance()
    {
        static CGameSave gameSave;
        return gameSave;
    }
    
    // Load the game data
    void load();
    
    // Save/Save/Close the game data
    void openSaveClose();
    
    // Set/Get play lobby music
    void setPlayLobbyMusic( bool value );
    bool getPlayLobbyMusic() const;
    
    // Set/Get play stop sounds
    void setPlayStopSounds( bool value );
    bool getPlayStopSounds() const;
    
    // Set/Get play spin music
    void setPlaySpinMusic( bool value );
    bool getPlaySpinMusic() const;
    
    // Set/Get total credits
    void setTotalCredits( uint value );
    uint getTotalCredits() const;
    
private:

    // Constructor
    CGameSave();

    // Destructor
    ~CGameSave();
    
    // Open the game data
    bool open();
    
    // Save the game data
    void save();
    
    // Close the game data file
    void close();
    
private:

    // Save file
    SDL_RWops * m_file;
    
    // Save Data
    SSaveData m_saveData;
};

#endif  // __game_save_h__
