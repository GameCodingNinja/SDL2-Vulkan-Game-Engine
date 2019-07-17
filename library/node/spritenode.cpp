
/************************************************************************
*    FILE NAME:       spritenode.cpp
*
*    DESCRIPTION:     Sprite node class for rendering just one sprite
************************************************************************/

// Physical component dependency
#include <node/spritenode.h>

// Game lib dependencies
#include <utilities/deletefuncs.h>
#include <2d/object2d.h>


/************************************************************************
*    DESC:  Constructor
************************************************************************/
CSpriteNode::CSpriteNode( const iObjectData & objectData, int spriteId ) :
    m_sprite(objectData, spriteId)
{
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
    m_sprite.update();
    m_sprite.physicsUpdate();
}


/***************************************************************************
*    DESC:  Transform the sprite
****************************************************************************/
void CSpriteNode::transform()
{
    m_sprite.getObject()->transform();
}

// Used to transform object on a sector
void CSpriteNode::transform( const CObject2D & object )
{
    m_sprite.getObject()->transform( object );
}


/***************************************************************************
*    DESC:  Record the command buffer vector in the device
*           for all the sprite objects that are to be rendered
****************************************************************************/
void CSpriteNode::recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer, const CCamera & camera )
{
    m_sprite.recordCommandBuffer( index, cmdBuffer, camera );
}


/************************************************************************
*    DESC:  Get the unique head node id number
************************************************************************/
int CSpriteNode::getId() const
{
    return m_sprite.getId();
}


/************************************************************************
*    DESC:  Get the sprite
************************************************************************/
CSprite * CSpriteNode::getSprite()
{
    return &m_sprite;
}
