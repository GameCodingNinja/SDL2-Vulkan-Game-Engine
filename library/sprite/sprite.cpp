
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
#include <2d/visualcomponentquad.h>
#include <2d/visualcomponentspritesheet.h>
#include <2d/visualcomponentscaledframe.h>
#include <2d/visualcomponentfont.h>
#include <2d/visualcomponentnull.h>
#include <3d/visualcomponent3d.h>
#include <physics/physicscomponent2d.h>
#include <physics/physicscomponent3d.h>
#include <utilities/xmlParser.h>
#include <utilities/matrix.h>
#include <utilities/xmlparsehelper.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CSprite::CSprite( const iObjectData & objectData ) :
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
    }
    else if( objectData.is3D() )
    {
        // Create the visual component
        if( objectData.getVisualData().isActive() )
            m_upVisualComponent.reset( new CVisualComponent3D( objectData ) );

        // Create the physics component
        if( objectData.getPhysicsData().isActive() )
            m_upPhysicsComponent.reset( new CPhysicsComponent3D( objectData ) );
    }

    // If there's no visual data, set the hide flag
    setVisible( objectData.getVisualData().isActive() );
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
    loadTransFromNode( node );

    // Init the script functions
    initScriptFunctions( node );

    // Load the font properties from XML node
    if( m_upVisualComponent->isFontSprite() )
        m_upVisualComponent->loadFontPropFromNode( node );
}

/************************************************************************
*    DESC:  Update the physics transforms
************************************************************************/
void CSprite::loadTransforms( const XMLNode & node )
{
    // Load the transform data
    loadTransFromNode( node );
    
    if( m_upPhysicsComponent )
        m_upPhysicsComponent->setTransform(m_pos.x, m_pos.y, m_rot.z);
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

    // Prepare any script functions that have been flagged
    for( auto & iter : m_scriptFunctionMap )
        if( std::get<2>(iter.second) )
            m_scriptComponent.prepare( std::get<0>(iter.second), std::get<1>(iter.second), {this} );
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
*    DESC:  Stop the script and recycle the context
************************************************************************/
bool CSprite::stopAndRecycle( const std::string & scriptFuncId )
{
    auto iter = m_scriptFunctionMap.find( scriptFuncId );
    if( iter != m_scriptFunctionMap.end() )
    {
        m_scriptComponent.stopAndRecycle( std::get<1>(iter->second) );

        return true;
    }

    return false;
}

/************************************************************************
*    DESC:  Stop the script, recycle and start the execution
************************************************************************/
bool CSprite::stopAndRestart( const std::string & scriptFuncId, bool forceUpdate )
{
    auto iter = m_scriptFunctionMap.find( scriptFuncId );
    if( iter != m_scriptFunctionMap.end() )
    {
        m_scriptComponent.stopAndRestart( std::get<0>(iter->second), std::get<1>(iter->second), {this} );

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
void CSprite::copyScriptFunctions( const std::map<std::string, std::tuple<std::string, std::string, bool>> & scriptFunctionMap )
{
    for( auto & iter : scriptFunctionMap )
        m_scriptFunctionMap.emplace( iter );
}

/************************************************************************
*    DESC:  Update the sprite
************************************************************************/
void CSprite::update()
{
    m_scriptComponent.update();
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
    if( isVisible() )
        m_upVisualComponent->recordCommandBuffer( index, cmdBuf, this, camera );
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
            setCropOffset( m_upVisualComponent->getCropOffset( index ) );
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
*    DESC:  Get the object data
************************************************************************/
const iObjectData & CSprite::getObjectData() const
{
    return m_rObjectData;
}

/************************************************************************
*    DESC:  Does this sprite have script functions
************************************************************************/
bool CSprite::hasScriptFunctions()
{
    return !m_scriptFunctionMap.empty();
}

/************************************************************************
*    DESC:  Apply the rotation
************************************************************************/
void CSprite::applyRotation( CMatrix & matrix )
{
    // 3D light calculations require a rotation matrix without scale
    if( m_rObjectData.is3D() )
    {
        // Add in the center point prior to rotation
        if( m_parameters.isSet( NDefs::CENTER_POINT ) )
            matrix.translate( m_centerPos );
        
        // Add in the rotation if this is NOT a physics transformation
        if( !m_parameters.isSet( NDefs::PHYSICS_TRANSFORM ) )
        {
            m_upVisualComponent->getRotMatrix().initilizeMatrix();
            m_upVisualComponent->getRotMatrix().rotate( m_rot );
        }
        
        // Since the rotation has already been done, multiply it into the matrix
        matrix.multiply3x3( m_upVisualComponent->getRotMatrix() );
        
        // Subtract the center point after rotation to put back in original position
        if( m_parameters.isSet( NDefs::CENTER_POINT ) )
            matrix.translate( -m_centerPos );
    }
    else
    {
        CObjectTransform::applyRotation( matrix );
    }
}

/************************************************************************
*    DESC:  Use a point to set a column - used for 3d physics
************************************************************************/
void CSprite::setRotMatrixColumn( const int col, const float x, const float y, const float z )
{
    m_upVisualComponent->setRotMatrixColumn( col, x, y, z );
}
