
/************************************************************************
*    FILE NAME:       startupstate.h
*
*    DESCRIPTION:     CStartUp Class State
************************************************************************/

#ifndef __start_up_state_h__
#define __start_up_state_h__

// Physical component dependency
#include "commonstate.h"

class CStartUpState : public CCommonState
{
public:

    // Constructor
    CStartUpState();

    // Destructor
    virtual ~CStartUpState();
    
    // Do any pre-load init's
    void init() override;
    
    // Handle events
    void handleEvent( const SDL_Event & rEvent ) override;

    // Is the state done
    bool doStateChange() override;

private:
    
    // Load the assets
    void assetLoad();
    
private:
    
    // Error string messages
    std::string m_errorTitle;
    std::string m_errorMsg;
};

#endif


