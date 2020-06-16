
/************************************************************************
*    FILE NAME:       spritenode.cpp
*
*    DESCRIPTION:     Sprite node that allows for children
************************************************************************/

// Physical component dependency
#include <node/spritenode.h>

// Game lib dependencies
#include <common/objecttransform.h>
#include <node/nodedata.h>
#include <objectdata/objectdatamanager.h>
#include <utilities/genfunc.h>

/************************************************************************
*    DESC:  Constructor / Destructor
************************************************************************/
CSpriteNode::CSpriteNode( const CNodeData & rNodeData ) :
        CRenderNode( rNodeData.getNodeId(), rNodeData.getParentNodeId() ),
        CSprite( CObjectDataMgr::Instance().getData( rNodeData.getGroup(), rNodeData.getObjectName() ) )
{
    m_userId = rNodeData.getUserId();
    m_type = ENodeType::SPRITE;

    // Create a CRC16 of the node name
    if( !rNodeData.getNodeName().empty() )
        m_crcUserId = NGenFunc::CalcCRC16( rNodeData.getNodeName() );

    // Load the rest from XML node
    CSprite::load( rNodeData.getXMLNode() );

    // Init the physics
    CSprite::initPhysics();

    // Init the sprite
    CSprite::init();
}

CSpriteNode::~CSpriteNode()
{}

/***************************************************************************
*    DESC:  Update the nodes
*           NOTE: Only gets called if this is the head node
****************************************************************************/
void CSpriteNode::update()
{
    CSprite::physicsUpdate();
    CSprite::update();
    
    // Call inherited for recursion of children
    CRenderNode::update();
}

/***************************************************************************
*    DESC:  Translate the nodes
*           NOTE: Only gets called if this is the head node
****************************************************************************/
void CSpriteNode::transform()
{
    CSprite::transform();

    // Call inherited for recursion of children
    CRenderNode::transform();
}

void CSpriteNode::transform( const CObjectTransform & object )
{
    CSprite::transform( object );

    CRenderNode::transform();
}

/***************************************************************************
*    DESC:  Record the command buffer vector in the device
*           for all the sprite objects that are to be rendered
*           NOTE: Only gets called if this is the head node
****************************************************************************/
void CSpriteNode::recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer, const CCamera & camera )
{
    CSprite::recordCommandBuffer( index, cmdBuffer, camera );
    
    // Call inherited for recursion of children
    CRenderNode::recordCommandBuffer( index, cmdBuffer, camera );
}

/************************************************************************
*    DESC:  Get the sprite
************************************************************************/
CSprite * CSpriteNode::getSprite()
{
    return static_cast<CSprite *>(this);
}

/************************************************************************
*    DESC:  Get the object
************************************************************************/
CObjectTransform * CSpriteNode::getObject()
{
    return static_cast<CObjectTransform *>(this);
}
