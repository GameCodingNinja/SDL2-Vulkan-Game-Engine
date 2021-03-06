
/************************************************************************
*    FILE NAME:       istrategy.h
*
*    DESCRIPTION:     Strategy Interface Class
************************************************************************/

#ifndef __i_strategy_h__
#define __i_strategy_h__

// Standard lib dependencies
#include <string>
#include <vector>

// Vulkan lib dependencies
#include <system/vulkan.h>

// Forward Declarations
class iNode;
class CCamera;

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

    // Get the pointer to the node
    virtual iNode * getNode( const std::string & instanceName ){ return nullptr; };
    virtual iNode * getNode( const std::string & instanceName, size_t sector ){ return nullptr; };

    // activate/deactivate node
    virtual iNode * activateNode( const std::string & instanceName ){ return nullptr; };
    virtual void deactivateNode( const std::string & instanceName ){};

    // Clear all nodes
    virtual void clear(){};
    
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
