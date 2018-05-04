
/************************************************************************
*    FILE NAME:       loadstate.h
*
*    DESCRIPTION:     All this state does is unload/load and
*                     does a state change.
************************************************************************/

#ifndef __load_state_h__
#define __load_state_h__

// Physical component dependency
#include "igamestate.h"

// Standard lib dependencies
#include <string>
#include <memory>

// Forward declaration(s)
class CSprite2D;

class CLoadState : public iGameState
{
public:

    // Constructor
    CLoadState( const CStateMessage & stateMsg );

    // Destructor
    virtual ~CLoadState();
    
    // Do any pre-load init's
    void init() override;

    // Is the state done
    bool doStateChange() override;

    // Get the next state to load
    NGameDefs::EGameState getNextState() override;

private:

    // Load during the startup screen
    void animate();

    // Load the assets
    void objectDataLoad();
    void assetsLoad();
    void criticalLoad();
    void criticalInit();
    
private:
    
    // Load animation
    std::unique_ptr<CSprite2D> m_upSprite;
    
    // Flag to indicate the thread load is finished
    bool m_threadActive;
    
    // Animation members
    float m_time;
    int m_frame;
    
    // Exception text
    std::string m_errorTitle;
    std::string m_errorMsg;
};

#endif  // __game_load_state_h__


