
/************************************************************************
*    FILE NAME:       gametemplate.h
*
*    DESCRIPTION:     CGame class
************************************************************************/

#ifndef __game_h__
#define __game_h__

// Standard lib dependencies
#include <string>

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
    void recordCommandBuffer( uint32_t cmdBufIndex );
};

int FilterEvents( void * userdata, SDL_Event * pEvent );

#endif
