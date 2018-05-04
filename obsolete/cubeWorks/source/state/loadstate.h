
/************************************************************************
*    FILE NAME:       loadstate.h
*
*    DESCRIPTION:     CRunState Class State
************************************************************************/

#ifndef __load_state_h__
#define __load_state_h__

// Physical component dependency
#include "igamestate.h"

// Standard lib dependencies
#include <string>
#include <mutex>

// Forward declaration(s)
class CMatrix;
class CLoadScrnAnim;
struct XMLNode;

class CLoadState : public iGameState
{
public:

    // Constructor
    CLoadState( const CStateMessage & stateMsg );

    // Destructor
    virtual ~CLoadState(){};

    // Is the state done
    bool DoStateChange();

    // Get the next state to load
    virtual EGameState GetNextState();

private:

    // Load during the startup screen
    void Animate();

    // Load during the loading screen
    void Load();
    
    // Init the animation load data
    void LoadAnimData(
        const std::string & group,
        std::vector<CLoadScrnAnim> & animDataVec,
        const XMLNode & node );
    
private:
    
    // Flag to indicate it's time to leave the thread
    bool m_exitThread;
    
    // mutex for critical section
    std::mutex m_mutex;

};

#endif  // __game_load_state_h__


