
/************************************************************************
*    FILE NAME:       game.h
*
*    DESCRIPTION:     CGame class
************************************************************************/

#pragma once

// Standard lib dependencies
#include <memory>

// Forward declaration(s)
class iGameState;
union SDL_Event;

class CGame
{
public:

    // Constructor
    CGame();

    // Destructor
    virtual ~CGame();
    
    // Create the game
    void create();
    
    // Main game loop
    bool gameLoop();

    // Display error massage
    void displayErrorMsg( const std::string & title, const std::string & msg );

private:

    // Handle events
    bool handleEvent( const SDL_Event & rEvent );

    // Handle the state change
    void doStateChange();

    // Record the command buffer vector in the device
    // for all the sprite objects that are to be rendered
    void recordCommandBuffer( uint32_t cmdBufIndex );
    
    // Callback for the state string
    void statStringCallBack( const std::string & statStr );

    // Poll for game events
    void pollEvents();

private:
    
    // flag to indicate the game is running
    bool m_gameRunning;

    // scoped pointer Game State
    std::unique_ptr<iGameState> upGameState;
};

bool FilterEvents( void * userdata, SDL_Event * pEvent );
