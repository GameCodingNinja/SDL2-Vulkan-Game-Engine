
/************************************************************************
*    FILE NAME:       commonstate.h
*
*    DESCRIPTION:     CCommonState Class State
************************************************************************/

#ifndef __common_state_h__
#define __common_state_h__

// Physical component dependency
#include "igamestate.h"

// Standard lib dependencies
#include <string>
#include <map>

class CCommonState : public iGameState
{
public:

    // Constructor
    CCommonState( const NGameDefs::EGameState, const NGameDefs::EGameState );

    // Destructor
    virtual ~CCommonState();

    // Handle events
    virtual void handleEvent( const SDL_Event & rEvent ) override;

    // Update objects that require them
    virtual void update() override;

    // Transform the game objects
    virtual void transform() override;
    
    // 2D/3D Render of game content
    virtual void preRender() override;

    // 2D/3D Render of game content
    virtual void postRender() override;
    
protected:
    
    // Get the load state
    NGameDefs::EGameState getLoadState( void * pVoid );
    
private:
        
    // game state map
    std::map<std::string, NGameDefs::EGameState> m_gameStateMap;

};

#endif  // __common_state_h__


