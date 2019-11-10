
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
    void setCamera( const std::string & cameraId );
    CCamera & getCamera();
    
    // Create the sprite
    virtual iNode * create(
        const std::string & dataName,
        const std::string & instanceName = "",
        bool makeActive = true,
        const std::string & group = std::string() ){ return nullptr; }
    
    // Create the node
    virtual void destroy( int id ){};
    
    // Set the command buffers
    void setCommandBuffers( std::vector<VkCommandBuffer> & commandBufVec );

    // Update the sprite
    virtual void update(){}

    // Transform the sprite
    virtual void transform(){}
    
    // Record the command buffer for all the sprite objects that are to be rendered
    virtual void recordCommandBuffer( uint32_t index ){}
    
protected:
    
    // Id increment member
    static int m_idInc;
    
    // Camera pointer
    CCamera * m_pCamera;
    
    // Command buffer
    // NOTE: command buffers don't to be freed because
    //       they are freed by deleting the pool they belong to
    //       and the pool will be freed at the end of the state
    std::vector<VkCommandBuffer> m_commandBufVec;
};

#endif
