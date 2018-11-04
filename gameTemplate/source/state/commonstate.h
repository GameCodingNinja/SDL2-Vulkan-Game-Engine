
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
#include <vector>

class CCommonState : public iGameState
{
public:

    // Constructor
    CCommonState( const NGameDefs::EGameState, const NGameDefs::EGameState );

    // Destructor
    virtual ~CCommonState();

    // Handle events
    virtual void handleEvent( const SDL_Event & rEvent ) override;
    
    // Handle any misc processing before the real work is started
    void miscProcess() override;

    // Update objects that require them
    void update() override;

    // Transform the game objects
    void transform() override;
    
    // Record the command buffer vector in the device
    // for all the sprite objects that are to be rendered
    void recordCommandBuffer( uint32_t cmdBufIndex ) override;
    
protected:
    
    // Get the load state
    NGameDefs::EGameState getLoadState( void * pVoid );
    
private:
        
    // game state map
    std::map<std::string, NGameDefs::EGameState> m_gameStateMap;
};

#endif  // __common_state_h__


