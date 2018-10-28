
/************************************************************************
*    FILE NAME:       sprite.h
*
*    DESCRIPTION:     Sprite class
************************************************************************/

#ifndef __sprite_h__
#define __sprite_h__

// Game lib dependencies
#include <common/defs.h>
#include <script/scriptcomponent.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <string>
#include <map>
#include <memory>

// Vulkan lib dependencies
#include <system/vulkan.h>

// Forward declaration(s)
class CObject2D;
class iObjectData;
class iVisualComponent;
class iPhysicsComponent;
class iAIBase;
class CObjectData2D;
class CObjectData3D;
class SpriteData;
class CMatrix;
class CSpriteData;
class btTransform;
struct XMLNode;
union SDL_Event;

class CSprite : public boost::noncopyable
{
public:

    // Constructor
    CSprite( const CObjectData2D & objectData, int id = defs_SPRITE_DEFAULT_ID );
    CSprite( const CObjectData3D & objectData, int id = defs_SPRITE_DEFAULT_ID );

    // Destructor
    virtual ~CSprite();
    
    // Load the sprite data
    void load( const XMLNode & node );
    void load( const CSpriteData & spriteData );
    
    // Init the sprite
    void init();
    
    // Init the physics
    void initPhysics();
    
    // React to what the player is doing
    void handleEvent( const SDL_Event & rEvent );

    // Update the sprite 
    void update();
    
    // Update the physics 
    void physicsUpdate();

    // Record the command buffers
    void recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer, const CMatrix & viewProj );
    void recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer, const CMatrix & rotMatrix, const CMatrix & viewProj );
    
    // Init the script functions
    void initScriptFunctions( const XMLNode & node );
    
    // Prepare the script Id function to run
    bool prepare( const std::string & scriptFuncId, bool forceUpdate = false );
    
    // Copy over the script functions
    void copyScriptFunctions( const std::map<std::string, std::string> & scriptFunctionMap );
    
    // Get the reference to the object
    CObject2D * getObject();
    const CObject2D * getObject() const;
    
    // Get the object data
    const iObjectData & getObjectData() const;
    
    // Get the visual component
    iVisualComponent * getVisualComponent();
    
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
    
    // Base AI scoped pointer
    std::unique_ptr<iAIBase> m_upAI;
    
    // Script function map. Execute scripts with an id
    std::map<const std::string, std::string> m_scriptFunctionMap;

};

#endif


