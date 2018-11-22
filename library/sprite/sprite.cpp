
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
#include <common/iaibase.h>
#include <sprite/spritedata.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CSprite::CSprite( const CObjectData2D & objectData, int id ) :
    m_id( id ),
    m_rObjectData( objectData ),
    m_upObject( new CObject2D )
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

    // If there's no visual data, set the hide flag
    m_upObject->setVisible( objectData.getVisualData().isActive() );
}

CSprite::CSprite( const CObjectData3D & objectData, int id ) :
    m_id( id ),
    m_rObjectData( objectData ),
    m_upObject( new CObject3D )
{
    // Create the visual component
    if( objectData.getVisualData().isActive() )
        m_upVisualComponent.reset( new CVisualComponent3D( objectData ) );

    // Create the physics component
    if( objectData.getPhysicsData().isActive() )
        m_upPhysicsComponent.reset( new CPhysicsComponent3D( objectData ) );

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

void CSprite::load( const CSpriteData & spriteData )
{
    // Copy over the transform
    m_upObject->copyTransform( &spriteData );

    // Copy over the script functions
    copyScriptFunctions( spriteData.getScriptFunctions() );

    // See if this sprite is used for rendering a font string
    if( m_upVisualComponent->isFontSprite() && (spriteData.getFontData() != nullptr) )
        m_upVisualComponent->setFontData( *spriteData.getFontData() );
}


/************************************************************************
*    DESC:  Init the sprite
*           NOTE: Do not call from a constructor!
************************************************************************/
void CSprite::init()
{
    if( m_upVisualComponent->isFontSprite() )
        m_upVisualComponent->createFontString();

    prepare( "init", true );
}


/************************************************************************
*    DESC:  Init the script functions and add them to the map
*           This function loads the attribute info reguardless of what it is
************************************************************************/
void CSprite::initScriptFunctions( const XMLNode & node )
{
    // Check for scripting - Add an empty string for scripts not defined
    XMLNode scriptLstNode = node.getChildNode( "scriptLst" );
    if( !scriptLstNode.isEmpty() )
    {
        for( int i = 0; i < scriptLstNode.nChildNode(); ++i )
        {
            const XMLNode scriptNode = scriptLstNode.getChildNode(i);

            // Only the first attribute is used
            const XMLAttribute attribute = scriptNode.getAttribute(0);
            const std::string attrName = attribute.lpszName;
            const std::string attrValue = attribute.lpszValue;

            // Add the attribute name and value to the map
            if( !attrValue.empty() )
            {
                // Get the group for this script. Default it to the object data group
                std::string group = m_rObjectData.getGroup();
                if( scriptNode.isAttributeSet( "group" ) )
                {
                    group = scriptNode.getAttribute( "group" );
                    if( group.empty() )
                        group = m_rObjectData.getGroup();
                }
            
                m_scriptFunctionMap.emplace( attrName, std::forward_as_tuple(group, attrValue) );

                if( attrName == "update" )
                    m_upObject->getParameters().add( NDefs::SCRIPT_UPDATE );
            }
        }
    }
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
    {
        m_scriptFunctionMap.emplace( iter );

        if( iter.first == "update" )
            m_upObject->getParameters().add( NDefs::SCRIPT_UPDATE );
    }
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

    if( m_upObject->getParameters().isSet( NDefs::SCRIPT_UPDATE ) )
        prepare( "update" );

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
