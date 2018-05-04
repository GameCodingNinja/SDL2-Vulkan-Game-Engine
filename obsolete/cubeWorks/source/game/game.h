
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

class CGame : public CBaseGame
{
public:

    // Constructor
    CGame();

    // Destructor
    virtual ~CGame();

protected:

    // Game start init
    void Init() override;

    // Handle events
    bool HandleEvent( const SDL_Event & rEvent );
    
    // Handle any misc processing before the real work is started
    void MiscProcess() override;

    // Handle the state change
    void DoStateChange() override;

    // Handle the physics
    void Physics() override;

    // Update animations, Move sprites, Check for collision
    void Update() override;

    // Transform game objects
    void Transform() override;

    // 2D/3D Render of game content
    void PreRender() override;

    // 2D/3D Render of post game content
    void PostRender() override;

private:

    // Callback for when a smart gui control is created
    void SmartGuiControlCreate( CUIControl * pUIControl );
    
    // Callback for when a smart menu is created
    void SmartMenuCreate( CMenu * pMenu );

protected:

    // scoped pointer Game State
    std::unique_ptr<iGameState> upGameState;

};

int FilterEvents( void * userdata, SDL_Event * pEvent );

#endif  // __game_h__


