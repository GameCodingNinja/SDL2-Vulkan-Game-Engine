
/************************************************************************
*    FILE NAME:       startupstate.h
*
*    DESCRIPTION:     CStartUp Class State
************************************************************************/

#ifndef __start_up_state_h__
#define __start_up_state_h__

// Physical component dependenc
#include "igamestate.h"

// Game lib dependencies
#include <2d/sprite2d.h>

// Standard lib dependencies
#include <memory>

// Forward declaration(s)
class CColor;

class CStartUpState : public iGameState
{
public:

    // Constructor
    CStartUpState();

    // Destructor
    virtual ~CStartUpState();
    
    // Do any pre-load init's
    void init() override;

    // Is the state done
    bool doStateChange() override;

private:

    // Load the assets
    void assetLoad();

    // Fade to color
    void fade(
        CSprite2D & sprite,
        float time,
        const CColor & cur,
        const CColor & finalColor );
    
private:
    
    // Logo to fade in and out
    std::unique_ptr<CSprite2D> m_upSpriteLogo;
};

#endif  // __start_up_state_h__


