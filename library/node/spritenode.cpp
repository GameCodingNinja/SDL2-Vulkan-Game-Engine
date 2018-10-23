
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
CSpriteNode::CSpriteNode( const CObjectData2D & objectData, int spriteId ) :
    m_sprite(objectData, spriteId)
{
}

CSpriteNode::CSpriteNode( const CObjectData3D & objectData, int spriteId ) :
    m_sprite(objectData, spriteId)
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CSpriteNode::~CSpriteNode()
{
}


/***************************************************************************
*    DESC:  Update the nodes.
****************************************************************************/
void CSpriteNode::update()
{
    m_sprite.update();
    m_sprite.physicsUpdate();
}


/***************************************************************************
*    DESC:  Update the nodes
****************************************************************************/
void CSpriteNode::transform()
{
    m_sprite.getObject()->transform();
}


/***************************************************************************
*    DESC:  Record the command buffer vector in the device
*           for all the sprite objects that are to be rendered
****************************************************************************/
void CSpriteNode::recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer, const CMatrix & viewProj )
{
    m_sprite.recordCommandBuffers( index, cmdBuffer, viewProj );
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
CSprite & CSpriteNode::getSprite()
{
    return m_sprite;
}
