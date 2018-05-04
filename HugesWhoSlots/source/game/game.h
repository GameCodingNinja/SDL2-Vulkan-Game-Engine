
/************************************************************************
*    FILE NAME:       gametemplate.h
*
*    DESCRIPTION:     CGame class
************************************************************************/

#ifndef __game_h__
#define __game_h__

// Physical component dependency
#include <system/basegame.h>

// Standard lib dependencies
#include <memory>

// Forward declaration(s)
class CSprite2D;
class CUIControl;
class iGameState;
class CMenu;
class CActorSprite2D;
class CShaderData;

class CGame : public CBaseGame
{
public:

    // Constructor
    CGame();

    // Destructor
    virtual ~CGame();

protected:

    // Game start init
    void init() override;

    // Handle events
    bool handleEvent( const SDL_Event & rEvent ) override;

    // Handle the state change
    void doStateChange() override;
    
    // Handle any misc processing before the real work is started
    void miscProcess() override;

    // Handle the physics
    void physics() override;

    // Update animations, Move sprites, Check for collision
    void update() override;

    // Transform game objects
    void transform() override;

    // 2D/3D Render of game content
    void render() override;

private:

    // Callback for when a smart gui control is created
    void smartGuiControlCreateCallBack( CUIControl * pUIControl );
    
    // Callback for when a smart menu is created
    void smartMenuCreateCallBack( CMenu * pMenu );
    
    // Callback for shader init
    void shaderInitCallBack( const std::string & shaderId );
    
    // Game settings call back fuctions
    void lobbyMusicCallBack( CUIControl * pUIControl );
    void spinMusicCallBack( CUIControl * pUIControl );
    void stopSoundsCallBack( CUIControl * pUIControl );
    
    // Callback for the state string
    void statStringCallBack( const std::string & statStr );
    
    // Dev debug settings call back fuctions
    void devDebugVSyncCallBack( CUIControl * pUIControl );
    void devDebugStringCallBack( CUIControl * pUIControl );

protected:

    // scoped pointer Game State
    std::unique_ptr<iGameState> upGameState;
    
    // Debug output
    std::unique_ptr<CSprite2D> upDebugDisplay;
};

int FilterEvents( void * userdata, SDL_Event * pEvent );

#endif  // __game_h__


