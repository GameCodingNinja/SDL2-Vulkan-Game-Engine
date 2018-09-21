
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
#include <3d/object3d.h>
#include <3d/visualcomponent3d.h>
#include <physics/physicscomponent2d.h>
#include <physics/physicscomponent3d.h>
#include <utilities/xmlParser.h>
#include <utilities/matrix.h>
#include <common/iaibase.h>
#include <common/camera.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CSprite::CSprite( const CObjectData2D & objectData, int id ) :
    m_id( id ),
    m_rObjectData( objectData ),
    m_upObject( new CObject2D )
{
    if( objectData.getVisualData().getGenerationType() == NDefs::EGT_QUAD )
        m_upVisualComponent.reset( new CVisualComponentQuad( objectData ) );
    
    else if( objectData.getVisualData().getGenerationType() == NDefs::EGT_SPRITE_SHEET )
        m_upVisualComponent.reset( new CVisualComponentSpriteSheet( objectData ) );
    
    else if( objectData.getVisualData().getGenerationType() == NDefs::EGT_SCALED_FRAME )
        m_upVisualComponent.reset( new CVisualComponentScaledFrame( objectData ) );
    
    // If there's no visual data, set the hide flag
    m_upObject->setVisible( objectData.getVisualData().isActive() );
}

CSprite::CSprite( const CObjectData3D & objectData, int id ) :
    m_id( id ),
    m_rObjectData( objectData ),
    m_upObject( new CObject3D ),
    m_upVisualComponent( new CVisualComponent3D( objectData.getVisualData() ) )
{
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
                m_scriptFunctionMap.emplace( attrName, attrValue );
                
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
        m_scriptComponent.prepare( m_rObjectData.getGroup(), iter->second, {this});
        
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
void CSprite::copyScriptFunctions( const std::map<std::string, std::string> & scriptFunctionMap )
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
    //if( m_upPhysicsComponent )
        //m_upPhysicsComponent->init(*this);
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
    //if( m_upPhysicsComponent )
    //    m_upPhysicsComponent->update( this );
}


/************************************************************************
*    DESC:  Record the command buffers                                                           
************************************************************************/
void CSprite::recordCommandBuffers( uint32_t index, VkCommandBuffer cmdBuffer, const CMatrix & viewProj )
{
    if( m_upObject->isVisible() )
        m_upVisualComponent->recordCommandBuffers( index, cmdBuffer, m_upObject->getMatrix(), viewProj );
}

void CSprite::recordCommandBuffers( uint32_t index, VkCommandBuffer cmdBuffer, const CCamera & camera )
{
    if( m_upObject->isVisible() )
        m_upVisualComponent->recordCommandBuffers( index, cmdBuffer, m_upObject->getMatrix(), camera.getFinalMatrix() );
}


/************************************************************************
*    DESC:  Get the reference to the object
************************************************************************/
CObject2D * CSprite::getObject()
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
