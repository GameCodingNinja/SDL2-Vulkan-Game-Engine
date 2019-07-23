
/************************************************************************
*    FILE NAME:       sprite.cpp
*
*    DESCRIPTION:     Sprite class
************************************************************************/

// Physical component dependency
#include <sprite/sprite.h>

// Game lib dependencies
#include <objectdata/objectdata2d.h>
#include <objectdata/objectdata3d.h>
#include <2d/object2d.h>
#include <2d/visualcomponentquad.h>
#include <2d/visualcomponentspritesheet.h>
#include <2d/visualcomponentscaledframe.h>
#include <2d/visualcomponentfont.h>
#include <2d/visualcomponentnull.h>
#include <3d/object3d.h>
#include <3d/visualcomponent3d.h>
#include <physics/physicscomponent2d.h>
#include <physics/physicscomponent3d.h>
#include <utilities/xmlParser.h>
#include <utilities/matrix.h>
#include <utilities/exceptionhandling.h>
#include <utilities/xmlparsehelper.h>
#include <common/iaibase.h>
#include <sprite/spritedata.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CSprite::CSprite( const iObjectData & objectData, int id ) :
    m_id( id ),
    m_rObjectData( objectData )
{
    if( objectData.is2D() )
    {
        // Create the visual component
        if( objectData.getVisualData().getGenerationType() == NDefs::EGT_QUAD )
            m_upVisualComponent.reset( new CVisualComponentQuad( objectData ) );

        else if( objectData.getVisualData().getGenerationType() == NDefs::EGT_SPRITE_SHEET )
            m_upVisualComponent.reset( new CVisualComponentSpriteSheet( objectData ) );

        else if( objectData.getVisualData().getGenerationType() == NDefs::EGT_SCALED_FRAME )
            m_upVisualComponent.reset( new CVisualComponentScaledFrame( objectData ) );

        else if( objectData.getVisualData().getGenerationType() == NDefs::EGT_FONT )
            m_upVisualComponent.reset( new CVisualComponentFont( objectData ) );
        
        else if( objectData.getVisualData().getGenerationType() == NDefs::EGT_NULL )
            m_upVisualComponent.reset( new CVisualComponentNull( objectData ) );

        // Create the physics component
        if( objectData.getPhysicsData().isActive() )
            m_upPhysicsComponent.reset( new CPhysicsComponent2D( objectData ) );
        
        m_upObject.reset( new CObject2D );
    }
    else if( objectData.is3D() )
    {
        // Create the visual component
        if( objectData.getVisualData().isActive() )
            m_upVisualComponent.reset( new CVisualComponent3D( objectData ) );

        // Create the physics component
        if( objectData.getPhysicsData().isActive() )
            m_upPhysicsComponent.reset( new CPhysicsComponent3D( objectData ) );

        m_upObject.reset( new CObject3D );
    }

    // If there's no visual data, set the hide flag
    m_upObject->setVisible( objectData.getVisualData().isActive() );
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CSprite::~CSprite()
{
}


/************************************************************************
*    DESC:  Load the sprite data
************************************************************************/
void CSprite::load( const XMLNode & node )
{
    // Load the transform data
    m_upObject->loadTransFromNode( node );

    // Init the script functions
    initScriptFunctions( node );

    // Load the font properties from XML node
    if( m_upVisualComponent->isFontSprite() )
        m_upVisualComponent->loadFontPropFromNode( node );
}


/************************************************************************
*    DESC:  Init the physics
************************************************************************/
void CSprite::initPhysics()
{
    if( m_upPhysicsComponent )
        m_upPhysicsComponent->init(*this);
}


/************************************************************************
*    DESC:  Init the sprite
*           NOTE: Do not call from a constructor!
************************************************************************/
void CSprite::init()
{
    if( m_upVisualComponent->isFontSprite() )
        m_upVisualComponent->createFontString();

    // Execute the init now
    prepare( "init", true );
    
    // This assume a looping update script that will kick in next frame
    prepare( "update", false );
}


/************************************************************************
*    DESC:  Init the script functions and add them to the map
*           This function loads the attribute info reguardless of what it is
************************************************************************/
void CSprite::initScriptFunctions( const XMLNode & node )
{
    NParseHelper::initScriptFunctions( node, m_scriptFunctionMap, m_rObjectData.getGroup() );
}


/************************************************************************
*    DESC:  Prepare the script function Id to run
************************************************************************/
bool CSprite::prepare( const std::string & scriptFuncId, bool forceUpdate )
{
    auto iter = m_scriptFunctionMap.find( scriptFuncId );
    if( iter != m_scriptFunctionMap.end() )
    {
        m_scriptComponent.prepare( std::get<0>(iter->second), std::get<1>(iter->second), {this} );

        // Allow the script to execute and return it's context to the queue
        // for the scripts that don't animate
        if( forceUpdate )
            m_scriptComponent.update();

        return true;
    }

    return false;
}


/************************************************************************
*    DESC:  Copy over the script functions
************************************************************************/
void CSprite::copyScriptFunctions( const std::map<std::string, std::tuple<std::string, std::string>> & scriptFunctionMap )
{
    for( auto & iter : scriptFunctionMap )
        m_scriptFunctionMap.emplace( iter );
}


/************************************************************************
*    DESC:  React to what the player is doing
************************************************************************/
void CSprite::handleEvent( const SDL_Event & rEvent )
{
    if( m_upAI )
        m_upAI->handleEvent( rEvent );
}


/************************************************************************
*    DESC:  Update the sprite
************************************************************************/
void CSprite::update()
{
    m_scriptComponent.update();

    if( m_upAI )
        m_upAI->update();
}


/************************************************************************
*    DESC:  Update the physics
************************************************************************/
void CSprite::physicsUpdate()
{
    if( m_upPhysicsComponent )
        m_upPhysicsComponent->update( this );
}


/************************************************************************
*    DESC:  Record the command buffer
************************************************************************/
void CSprite::recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuf, const CCamera & camera )
{
    if( m_upObject->isVisible() )
        m_upVisualComponent->recordCommandBuffer( index, cmdBuf, m_upObject.get(), camera );
}


/************************************************************************
*    DESC:  Get the reference to the object
************************************************************************/
CObject2D * CSprite::getObject()
{
    return m_upObject.get();
}

const CObject2D * CSprite::getObject() const
{
    return m_upObject.get();
}


/************************************************************************
*    DESC:  Get the visual component
************************************************************************/
iVisualComponent * CSprite::getVisualComponent()
{
    return m_upVisualComponent.get();
}

const iVisualComponent * CSprite::getVisualComponent() const
{
    return m_upVisualComponent.get();
}


/************************************************************************
*    DESC:  Get the physics component
************************************************************************/
iPhysicsComponent * CSprite::getPhysicsComponent()
{
    return m_upPhysicsComponent.get();
}


/************************************************************************
*    DESC:  Get the script component
************************************************************************/
CScriptComponent & CSprite::getScriptComponent()
{
    return m_scriptComponent;
}


/************************************************************************
*    DESC:  Set the texture ID from index
************************************************************************/
void CSprite::setFrame( uint index )
{
    if( m_upVisualComponent->getCurrentFrame() != index )
    {
        m_upVisualComponent->setFrame( index );

        if( m_upVisualComponent->getGenerationType() == NDefs::EGT_SPRITE_SHEET )
            m_upObject->setCropOffset( m_upVisualComponent->getCropOffset( index ) );
    }
}

/************************************************************************
*    DESC:  Get the frame count
************************************************************************/
uint CSprite::getFrameCount() const
{
    return m_rObjectData.getVisualData().getFrameCount();
}

/************************************************************************
*    DESC:  Get the current frame
************************************************************************/
uint CSprite::getCurrentFrame() const
{
    return m_upVisualComponent->getCurrentFrame();
}


/************************************************************************
*    DESC:  Get the unique id number
************************************************************************/
int CSprite::getId() const
{
    return m_id;
}


/************************************************************************
*    DESC:  Get the object data
************************************************************************/
const iObjectData & CSprite::getObjectData() const
{
    return m_rObjectData;
}


/************************************************************************
*    DESC:  Set/Get the AI pointer. This class owns the pointer
************************************************************************/
void CSprite::setAI( iAIBase * pAIBase )
{
    m_upAI.reset( pAIBase );

    // Handle any initialization in a separate function
    m_upAI->init();
}
