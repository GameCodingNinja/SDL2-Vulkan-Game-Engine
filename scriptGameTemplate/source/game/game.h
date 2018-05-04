
/************************************************************************
*    FILE NAME:       gametemplate.h
*
*    DESCRIPTION:     CGame class
************************************************************************/

#ifndef __game_h__
#define __game_h__

// Standard lib dependencies
#include <string>

// SDL lib dependencies
#include <SDL.h>

// Forward declaration(s)
class CSprite2D;
class CUIControl;
class iGameState;
class CMenu;
class iSprite;

class CGame
{
public:

    // Constructor
    CGame();

    // Destructor
    virtual ~CGame();
    
    // Create the game Window
    void create();
    
    // Main game loop for script driven games
    bool gameLoop();
    
    // Display error massage
    void displayErrorMsg( const std::string & title, const std::string & msg );
    
    // Is the game running?
    bool isGameRunning() const;

private:

    // OpenGL Init
    void openGLInit();
    
    // Game start init
    void init();
    
    // Poll for game events
    void pollEvents();

    // Handle events
    bool handleEvent( const SDL_Event & rEvent );
    
    // Start the game
    void startGame();

    // Stop the game
    void stopGame();
    
    // Callback for the state string
    void statStringCallBack( const std::string & statStr );
    
private:
    
    // The window we'll be rendering to
    SDL_Window * m_pWindow;

    // OpenGL context
    SDL_GLContext m_context;

    // flag to indicate the game is running
    bool m_gameRunning;

    // Clear buffer mask
    uint m_clearBufferMask;
};

int FilterEvents( void * userdata, SDL_Event * pEvent );

#endif  // __game_h__


