
/************************************************************************
*    FILE NAME:       ispritestrategy.h
*
*    DESCRIPTION:     Strategy Interface Class
************************************************************************/

#ifndef __i_strategy_h__
#define __i_strategy_h__

// Game lib dependencies
#include <common/point.h>
#include <common/worldvalue.h>

// Standard lib dependencies
#include <string>
#include <vector>

// Vulkan lib dependencies
#include <system/vulkan.h>

// Forward Declarations
class iNode;
class CMatrix;
class CCamera;
union SDL_Event;

class iStrategy
{
public:

    // Constructor
    iStrategy();

    // Destructor
    virtual ~iStrategy(){}

    // Load the data from file
    virtual void loadFromFile( const std::string & file ){}
    
    // Set to Destroy the sprite
    virtual void setToDestroy( int spriteIndex ){}
    
    // Set to create the sprite
    virtual void setToCreate( const std::string & name ){}
    
    // Set to create the sprite
    void setCamera( CCamera * pCamera );
    CCamera * getCamera();
    
    // Set the command buffers
    virtual void setCommandBuffers( std::vector<VkCommandBuffer> & commandBufVec ) = 0;
    
    // Create the sprite
    virtual iNode * create(
        const std::string & name,
        const CPoint<CWorldValue> & pos,
        const CPoint<float> & rot,
        const CPoint<float> & scale ){ return nullptr; }

    virtual iNode * create(
        const std::string & name ){ return nullptr; }
    
    // Do any init
    virtual void init(){}
    
    // Load the data from file
    virtual void miscProcess(){}

    // Update the sprite
    virtual void update(){}

    // Transform the sprite
    virtual void transform(){}
    
    // Record the command buffer for all the sprite objects that are to be rendered
    virtual void recordCommandBuffer( uint32_t index, const CCamera & camera ){}
    
    // Clear the sprite Id counter
    static void clearSpriteCounter();
    
    // Customize sprite id generation by defining an offset
    void setIdOffset( int offset );
    
    // Customize sprite id generation by defining a direction
    void setIdDir( int dir );
    
    // Enable this strategy
    void enable( bool enable = true );
    
protected:
    
    // Delete any sprites scheduled to die
    virtual void handleDelete(){}
    
protected:
    
    // ID Offset for this strategy 
    int m_idOffset;
    
    // ID Direction
    int m_idDir;
    
    // Enable strategy
    bool m_enable;
    
    // Id increment member
    static int m_idInc;
    
    // Camera pointer
    CCamera * m_pCamera;
};

#endif
