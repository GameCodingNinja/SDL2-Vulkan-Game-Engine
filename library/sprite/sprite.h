
/************************************************************************
*    FILE NAME:       sprite.h
*
*    DESCRIPTION:     Sprite class
************************************************************************/

#ifndef __sprite_h__
#define __sprite_h__

// Game lib dependencies
#include <common/defs.h>
#include <common/size.h>
#include <script/scriptcomponent.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <string>
#include <map>
#include <memory>
#include <tuple>

// Vulkan lib dependencies
#include <system/vulkan.h>

// Forward declaration(s)
class CObject2D;
class iObjectData;
class iVisualComponent;
class iPhysicsComponent;
class CObjectData2D;
class CObjectData3D;
class SpriteData;
class CMatrix;
class CSpriteData;
class CCamera;
class btTransform;
struct XMLNode;
union SDL_Event;

class CSprite : public boost::noncopyable
{
public:

    // Constructor
    CSprite( const iObjectData & objectData, int id = defs_DEFAULT_ID );

    // Destructor
    virtual ~CSprite();
    
    // Load the sprite data
    void load( const XMLNode & node );
    
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
    
    // Init the script functions
    void initScriptFunctions( const XMLNode & node );
    
    // Prepare the script Id function to run
    bool prepare( const std::string & scriptFuncId, bool forceUpdate = false );

    // Stop the script and recycle the context
    bool stopAndRecycle( const std::string & scriptFuncId );

    // Stop the script, recycle and start the execution
    bool stopAndRestart( const std::string & scriptFuncId, bool forceUpdate = false );
    
    // Copy over the script functions
    void copyScriptFunctions( const std::map<std::string, std::tuple<std::string, std::string, bool>> & scriptFunctionMap );
    
    // Get the reference to the object
    CObject2D * getObject();
    const CObject2D * getObject() const;
    
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
    
    // Get the unique id number
    int getId() const;

    // Does this sprite have script functions
    bool hasScriptFunctions();
    
private:
    
    // Unique Id number
    int m_id;
    
    // The object data
    const iObjectData & m_rObjectData;
    
    // Object unique pointer
    std::unique_ptr<CObject2D> m_upObject;
    
    // The visual part of the sprite
    std::unique_ptr<iVisualComponent> m_upVisualComponent;
    
    // The physics part of the sprite
    std::unique_ptr<iPhysicsComponent> m_upPhysicsComponent;
    
    // The script part of the sprite
    CScriptComponent m_scriptComponent;
    
    // Script function map. Execute scripts with an id
    std::map<const std::string, std::tuple<std::string, std::string, bool>> m_scriptFunctionMap;
};

#endif
