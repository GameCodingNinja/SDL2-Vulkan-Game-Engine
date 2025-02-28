
/************************************************************************
*    FILE NAME:       game.h
*
*    DESCRIPTION:     CGame class
************************************************************************/

#pragma once

// Standard lib dependencies
#include <string>
#include <cstdint>

// Forward declaration(s)
class CUIControl;
class iGameState;
class CMenu;
class iSprite;
union SDL_Event;

class CGame
{
public:

    // Constructor
    CGame();

    // Destructor
    virtual ~CGame();
    
    // Create the game Window
    void create();
    
    // Display error massage
    void displayErrorMsg( const std::string & title, const std::string & msg );

private:
    
    // Game start init
    void init();

    // Register game functions
    void registerGameFunc();
    
    // Poll for game events
    void pollEvents();

    // Handle events
    bool handleEvent( const SDL_Event & rEvent );
    
    // Callback for the state string
    void statStringCallBack( const std::string & statStr );
    
    // Record the command buffer vector in the device
    // for all the sprite objects that are to be rendered
    void recordCommandBuffer( const uint32_t cmdBufIndex );
};

bool FilterEvents( void * userdata, SDL_Event * pEvent );
