
/************************************************************************
*    FILE NAME:       startupstate.h
*
*    DESCRIPTION:     CStartUp Class State
************************************************************************/

#ifndef __start_up_state_h__
#define __start_up_state_h__

// Physical component dependenc
#include "igamestate.h"

// Standard lib dependencies
#include <memory>

// Forward declaration(s)
class CColor;
class CSprite;
class CSprite2D;

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
    
    // Update objects that require them
    void update() override;

    // Transform the game objects
    void transform() override;

    // Record the command buffer vector in the device
    // for all the sprite objects that are to be rendered
    void recordCommandBuffer( uint32_t cmdBufIndex ) override;

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
    std::unique_ptr<CSprite> m_upSpriteLogo;
};

#endif  // __start_up_state_h__


