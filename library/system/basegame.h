
/************************************************************************
*    FILE NAME:       basegame.h
*
*    DESCRIPTION:     base game class
************************************************************************/

#ifndef __base_game_h__
#define __base_game_h__

// Game lib dependencies
#include <common/defs.h>

// Standard lib dependencies
#include <string>

// SDL lib dependencies
#include <SDL.h>

class CBaseGame
{
public:

    // Constructor
    CBaseGame();

    // Destructor
    virtual ~CBaseGame();

    // Create the game Window
    void create();

    // Main game loop
    bool gameLoop();

    // Display error massage
    void displayErrorMsg( const std::string & title, const std::string & msg );

    // Is the game running?
    bool isGameRunning() const;

protected:

    // Game start init
    virtual void init();

    // Handle events
    virtual bool handleEvent( const SDL_Event & event ) = 0;

    // Handle the state change
    virtual void doStateChange() = 0;
    
    // Handle any misc processing before the real work is started
    virtual void miscProcess() = 0;

    // Handle the physics
    virtual void physics() = 0;

    // Update animations, Move sprites, Check for collision
    virtual void update() = 0;

    // Transform game objects
    virtual void transform() = 0;

    // Render of game content
    virtual void render() = 0;

    // Start the game
    void startGame();

    // Stop the game
    void stopGame();

    // Poll for game events
    void pollEvents();

protected:

    // flag to indicate the game is running
    bool m_gameRunning;

    // Clear buffer mask
    uint m_clearBufferMask;

};

#endif  // __base_game_h__


