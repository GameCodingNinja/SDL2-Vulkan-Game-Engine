
/************************************************************************
*    FILE NAME:       startupstate.h
*
*    DESCRIPTION:     CStartUp Class State
************************************************************************/

#ifndef __start_up_state_h__
#define __start_up_state_h__

// Physical component dependency
#include "igamestate.h"

// Standard lib dependencies
#include <memory>
#include <vector>

// Vulkan lib dependencies
#include <system/vulkan.h>

// Forward Declarations
class iNode;

class CStartUpState : public iGameState
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
    
    // Update objects that require them
    void update() override;

    // Transform the game objects
    void transform() override;

    // Record the command buffer vector in the device
    // for all the sprite objects that are to be rendered
    void recordCommandBuffer( uint32_t index ) override;

private:
    
    // Load the assets
    void assetLoad();
    
private:
    
    // Command buffer
    std::vector<VkCommandBuffer> m_commandBufVec;
    
    // Node pointer to logo
    iNode * m_pLogo;
};

#endif


