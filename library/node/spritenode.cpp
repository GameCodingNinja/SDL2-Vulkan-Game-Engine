
/************************************************************************
*    FILE NAME:       spritenode.cpp
*
*    DESCRIPTION:     Sprite node class for rendering just one sprite
************************************************************************/

// Physical component dependency
#include <node/spritenode.h>

// Game lib dependencies
#include <common/objecttransform.h>
#include <node/nodedata.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CSpriteNode::CSpriteNode( const iObjectData & objectData, const CNodeData & rNodeData ) :
        iNode( rNodeData.getNodeId(), rNodeData.getParentNodeId() ),
        CSprite( objectData )
{
    m_id = rNodeData.getId();
    m_type = NDefs::ENT_SPRITE;
}

/************************************************************************
*    DESC:  destructor
************************************************************************/
CSpriteNode::~CSpriteNode()
{
}

/***************************************************************************
*    DESC:  Update the sprite.
****************************************************************************/
void CSpriteNode::update()
{
    CSprite::update();
    CSprite::physicsUpdate();
}

/***************************************************************************
*    DESC:  Transform the sprite
****************************************************************************/
void CSpriteNode::transform()
{
    CSprite::transform();
}

// Used to transform object on a sector
void CSpriteNode::transform( const CObjectTransform & object )
{
    CSprite::transform( object );
}

/***************************************************************************
*    DESC:  Record the command buffer vector in the device
*           for all the sprite objects that are to be rendered
****************************************************************************/
void CSpriteNode::recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer, const CCamera & camera )
{
    CSprite::recordCommandBuffer( index, cmdBuffer, camera );
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
