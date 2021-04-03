
/************************************************************************
*    FILE NAME:       sprite.h
*
*    DESCRIPTION:     Sprite class
************************************************************************/

#pragma once

// Physical component dependency
#include <common/object.h>

// Game lib dependencies
#include <common/defs.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <string>
#include <memory>

// Vulkan lib dependencies
#include <system/vulkan.h>

// Forward declaration(s)
class iObjectData;
class iVisualComponent;
class iPhysicsComponent;
class CMatrix;
class CCamera;
struct XMLNode;
union SDL_Event;

class CSprite : public CObject, boost::noncopyable
{
public:

    // Constructor
    CSprite( const iObjectData & objectData );

    // Destructor
    virtual ~CSprite();
    
    // Load the sprite data
    void load( const XMLNode & node );

    // Reload the sprite node
    // NOTE:  This function is for reloading the sprite after it was initially
    //        created. One use case is that a basic sprite is created by
    //        the strategy to then be reloaded by the strategy loader
    void reload( const XMLNode & node );
    
    // Init the physics
    void initPhysics();
    
    // Init the sprite
    void init();
    
    // React to what the player is doing
    void handleEvent( const SDL_Event & rEvent );

    // Update the sprite 
    void update();
    
    // Update the physics 
    void physicsUpdate();

    // Record the command buffers
    void recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuf, const CCamera & camera );

    // Destroy the physics
    void destroyPhysics();
    
    // Load the script functions from node and add them to the map
    void loadScriptFromNode( const XMLNode & node );
    
    // Get the object data
    const iObjectData & getObjectData() const;
    
    // Get the visual component
    iVisualComponent * getVisualComponent();
    const iVisualComponent * getVisualComponent() const;
    
    // Get the physics component
    iPhysicsComponent * getPhysicsComponent();
    
    // Get the script component
    CScriptComponent & getScriptComponent();
    
    // Set the texture ID from index
    void setFrame( uint index );
    
    // Get the frame count
    uint getFrameCount() const;
    
    // Get the current frame
    uint getCurrentFrame() const;

    // Use a point to set a column - used for 3d physics
    void setRotMatrixColumn( const int col, const float x, const float y, const float z ) final;

protected:
    
    // Apply the rotation
    void applyRotation( CMatrix & matrix ) final;
    
private:
    
    // The object data
    const iObjectData & m_rObjectData;
    
    // The visual part of the sprite
    std::unique_ptr<iVisualComponent> m_upVisualComponent;
    
    // The physics part of the sprite
    std::unique_ptr<iPhysicsComponent> m_upPhysicsComponent;
};
