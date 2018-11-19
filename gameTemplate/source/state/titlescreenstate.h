
/************************************************************************
*    FILE NAME:       titlescreenstate.h
*
*    DESCRIPTION:     CRunState Class State
************************************************************************/

#ifndef __title_screen_state_h__
#define __title_screen_state_h__

// Physical component dependency
#include "commonstate.h"

// Forward Declarations
class iStrategy;

class CTitleScreenState : public CCommonState
{
public:

    // Constructor
    CTitleScreenState();

    // Destructor
    virtual ~CTitleScreenState();
    
    // Do any pre-game loop init's
    void init() override;
    
    // Handle events
    void handleEvent( const SDL_Event & rEvent ) override;
    
    // Update objects that require them
    void update() override;
    
    // Static load function
    static void load();
    
private:
        
    // Cube strategy
    iStrategy * pCubeStrategy;
};

#endif
