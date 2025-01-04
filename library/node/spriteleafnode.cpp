
/************************************************************************
*    FILE NAME:       spriteleafnode.cpp
*
*    DESCRIPTION:     Sprite node class for handling a sprite with
*                     no children to keep the overhead low
************************************************************************/

// Physical component dependency
#include <node/spriteleafnode.h>

// Game lib dependencies
#include <common/object.h>
#include <common/ivisualcomponent.h>
#include <node/nodedata.h>
#include <objectdata/objectdatamanager.h>
#include <objectdata/objectdata2d.h>
#include <utilities/genfunc.h>

/************************************************************************
*    DESC:  Constructor / Destructor
************************************************************************/
CSpriteLeafNode::CSpriteLeafNode( const CNodeData & rNodeData ) :
    iNode( rNodeData.getNodeId(), rNodeData.getParentNodeId() ),
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

CSpriteLeafNode::~CSpriteLeafNode()
{}

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
void CSpriteLeafNode::transform( const CObject & object )
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

/***************************************************************************
*    DESC:  Destroy the physics
****************************************************************************/
void CSpriteLeafNode::destroyPhysics()
{
    CSprite::destroyPhysics();
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
CObject * CSpriteLeafNode::getObject()
{
    return static_cast<CObject *>(this);
}

/***************************************************************************
*    DESC:  Get the radius. Need to add in the scale of the object
****************************************************************************/
float CSpriteLeafNode::getRadius()
{
    return getSprite()->getObjectData().getRadius() * getObject()->getScale().x;
}

/***************************************************************************
*    DESC:  Get the size
****************************************************************************/
CSize<float> CSpriteLeafNode::getSize()
{
    if(m_headNode)
        return getSprite()->getVisualComponent()->getSize() * getSprite()->getScale();
    
    return getSprite()->getVisualComponent()->getSize();
}