
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

// Forward declaration(s)
class CObject2D;
class iObjectData;
class iVisualComponent;
class iPhysicsComponent;
class iAIBase;
class CObjectData2D;
class CObjectData3D;
class CCamera;
class CMatrix;
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
    
    // Init the physics
    void initPhysics();
    
    // React to what the player is doing
    void handleEvent( const SDL_Event & rEvent );

    // Update the sprite 
    void update();
    
    // Update the physics 
    void physicsUpdate();

    // do the render
    void render( const CCamera & camera );
    void render( const CMatrix & matrix );
    
    // Init the script functions
    void initScriptFunctions( const XMLNode & node );
    
    // Prepare the script Id function to run
    bool prepare( const std::string & scriptFuncId, bool forceUpdate = false );
    
    // Copy over the script functions
    void copyScriptFunctions( const std::map<std::string, std::string> & scriptFunctionMap );
    
    // Get the reference to the object
    CObject2D * getObject();
    
    // Get the visual component
    iVisualComponent * getVisualComponent();
    
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

#endif  // __sprite_h__


