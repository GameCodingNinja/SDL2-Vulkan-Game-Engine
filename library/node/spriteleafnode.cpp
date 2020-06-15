
/************************************************************************
*    FILE NAME:       spriteleafnode.cpp
*
*    DESCRIPTION:     Sprite node class for handling a sprite with
*                     no children to keep the overhead low
************************************************************************/

// Physical component dependency
#include <node/spriteleafnode.h>

// Game lib dependencies
#include <common/objecttransform.h>
#include <node/nodedata.h>
#include <objectdata/objectdatamanager.h>
#include <utilities/genfunc.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CSpriteLeafNode::CSpriteLeafNode( const CNodeData & rNodeData ) :
        iNode( rNodeData.getNodeId(), rNodeData.getParentNodeId() ),
        CSprite( CObjectDataMgr::Instance().getData( rNodeData.getGroup(), rNodeData.getObjectName() ) )
{
    m_userId = rNodeData.getUserId();
    m_type = NDefs::ENT_SPRITE;

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

/***************************************************************************
*    DESC:  Update the sprite.
****************************************************************************/
void CSpriteLeafNode::update()
{
    CSprite::update();
    CSprite::physicsUpdate();
}

/***************************************************************************
*    DESC:  Transform the sprite
****************************************************************************/
void CSpriteLeafNode::transform()
{
    CSprite::transform();
}

// Used to transform object on a sector
void CSpriteLeafNode::transform( const CObjectTransform & object )
{
    CSprite::transform( object );
}

/***************************************************************************
*    DESC:  Record the command buffer vector in the device
*           for all the sprite objects that are to be rendered
****************************************************************************/
void CSpriteLeafNode::recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer, const CCamera & camera )
{
    CSprite::recordCommandBuffer( index, cmdBuffer, camera );
}

/************************************************************************
*    DESC:  Get the sprite
************************************************************************/
CSprite * CSpriteLeafNode::getSprite()
{
    return static_cast<CSprite *>(this);
}

/************************************************************************
*    DESC:  Get the object
************************************************************************/
CObjectTransform * CSpriteLeafNode::getObject()
{
    return static_cast<CObjectTransform *>(this);
}
