
/************************************************************************
*    FILE NAME:       statemessage.h
*
*    DESCRIPTION:     This class is custom per game project
************************************************************************/

#ifndef __state_message_h__
#define __state_message_h__

// Game dependencies
#include "statedefs.h"

class CStateMessage
{
public:
    
    NStateDefs::EGameState getLoadState()
    { return m_loadState; }
    
    NStateDefs::EGameState getUnloadState()
    { return m_unloadState; }
    
    void setMsg( NStateDefs::EGameState loadState, NStateDefs::EGameState unloadState )
    {
        m_loadState = loadState;
        m_unloadState = unloadState;
    }
    
private:

    NStateDefs::EGameState m_loadState   : 16;
    NStateDefs::EGameState m_unloadState : 16;
};

#endif
