
/************************************************************************
*    FILE NAME:       sprite2d.h
*
*    DESCRIPTION:     2D sprite class
************************************************************************/

#ifndef __sprite_2d_h__
#define __sprite_2d_h__

// Physical component dependency
#include <common/isprite.h>

// Game lib dependencies
#include <2d/visualcomponent2d.h>
#include <physics/physicscomponent2d.h>
#include <script/scriptcomponent.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <string>
#include <map>
#include <memory>

// Forward declaration(s)
class CObjectData2D;
class CColor;
class CSpriteData;
class iAIBase;
struct XMLNode;

class CSprite2D : public iSprite, boost::noncopyable
{
public:

    // Constructor
    CSprite2D( const CObjectData2D & objectData, int id = defs_SPRITE_DEFAULT_ID );

    // Destructor
    virtual ~CSprite2D();
    
    // Init the script functions
    void initScriptFunctions( const XMLNode & node );
    
    // Prepare the script function to run
    bool prepareFuncId( const std::string & scriptFuncId, bool forceUpdate = false );
    void prepare(
        const std::string & funcName,
        const std::vector<CScriptParam> & paramVec = std::vector<CScriptParam>(),
        bool forceUpdate = false );
    
    // Load the sprite data
    void load( const XMLNode & node );
    void load( const CSpriteData & spriteData );
    
    // Init the sprite
    void init() override;
    
    // Clean up the sprite
    void cleanUp() override;

    // Get the object data
    const CObjectData2D & getObjectData() const;

    // Get the visual component
    CVisualComponent2D & getVisualComponent();
    iVisualComponent * getVisualInterface();

    // Get the physics component
    CPhysicsComponent2D & getPhysicsComponent();
    iPhysicsComponent * getPhysicsInterface();

    // Get the scripting component
    CScriptComponent & getScriptComponent();
    
    // Init the physics
    void initPhysics() override;
    
    // React to what the player is doing
    void handleEvent( const SDL_Event & rEvent ) override;

    // Update the sprite 
    void update() override;
    
    // Update the physics 
    void physicsUpdate() override;

    // do the render
    void render( const CCamera & camera ) override;
    void render( const CMatrix & matrix ) override;
    
    // Set/Get the AI pointer
    void setAI( iAIBase * pAIBase ) override;
    
    // Copy over the script functions
    void copyScriptFunctions( const std::map<std::string, std::string> & scriptFunctionMap );

    // Get the frame count
    uint getFrameCount() const override;

    // Set the texture ID from index
    void setFrame( uint index ) override;
    
    // Get the current frame
    uint getCurrentFrame() const override;

protected:

    // The object data
    const CObjectData2D & m_rObjectData;
    
    // The visual part of the 2d sprite
    CVisualComponent2D m_visualComponent;
    
    // The script part of the 2d sprite
    CScriptComponent m_scriptComponent;

    // The physics part of the 2d sprite
    CPhysicsComponent2D m_physicsComponent;
    
    // Base AI scoped pointer
    std::unique_ptr<iAIBase> m_upAI;
    
    // Script function map. Tie events to script functions
    std::map<const std::string, std::string> m_scriptFunctionMap;

};

#endif  // __sprite_2d_h__


