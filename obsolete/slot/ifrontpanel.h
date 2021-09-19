
/************************************************************************
*    FILE NAME:       ifrontpanel.h
*
*    DESCRIPTION:     Interface container for buttons and meters
************************************************************************/

#ifndef __i_front_panel_h__
#define __i_front_panel_h__

// Game lib dependencies
#include <common/defs.h>

// Standard lib dependencies
#include <vector>

// Forward declaration(s)
class CUIButton;
class CUIMeter;
class CUIControl;

class iFrontPanel
{
public:
    
    // Init a new game
    virtual void initGame( uint credits ) = 0;
    
    // Start the bang up
    virtual void startBangUp( uint credits, uint win ) = 0;
    
    // Are the meters banging
    virtual bool isBanging() = 0;
    
    // Start the fast bang
    virtual void fastBang() = 0;
    
    // Enable the buttons
    virtual void enableButtons( bool allowPlay ) = 0;
};

#endif
