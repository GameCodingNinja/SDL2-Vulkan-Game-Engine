
/************************************************************************
*    FILE NAME:       spritenodemultilist.cpp
*
*    DESCRIPTION:     Sprite node multi link list class
************************************************************************/

// Physical component dependency
#include <node/spritenodemultilist.h>

// Game lib dependencies
#include <common/objecttransform.h>
#include <node/nodedata.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CSpriteNodeMultiLst::CSpriteNodeMultiLst( const iObjectData & objectData, const CNodeData & rNodeData ) :
        CNodeMultiLst( rNodeData.getNodeId(), rNodeData.getParentNodeId() ),
        m_sprite(objectData)
{
    m_sprite.setId( rNodeData.getId() );
    m_type = NDefs::ENT_SPRITE;
}

/************************************************************************
*    DESC:  destructor
************************************************************************/
CSpriteNodeMultiLst::~CSpriteNodeMultiLst()
{
}

/***************************************************************************
*    DESC:  Update the nodes.
****************************************************************************/
void CSpriteNodeMultiLst::update()
{
    m_sprite.physicsUpdate();
    m_sprite.update();
    
    // Call the parent but it has to be last
    CNodeMultiLst::update();
}

/***************************************************************************
*    DESC:  Translate the nodes
****************************************************************************/
void CSpriteNodeMultiLst::transform()
{
    m_sprite.transform();

    // Call the parent but it has to be last
    CNodeMultiLst::transform();
}

// Used to transform object on a sector
void CSpriteNodeMultiLst::transform( const CObjectTransform & object )
{
    m_sprite.transform( object );

    // Call the parent but it has to be last
    CNodeMultiLst::transform();
}

/***************************************************************************
*    DESC:  Record the command buffer vector in the device
*           for all the sprite objects that are to be rendered
****************************************************************************/
void CSpriteNodeMultiLst::recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer, const CCamera & camera )
{
    m_sprite.recordCommandBuffer( index, cmdBuffer, camera );
    
    // Call the parent but it has to be last
    CNodeMultiLst::recordCommandBuffer( index, cmdBuffer, camera );
}

/************************************************************************
*    DESC:  Get the sprite
************************************************************************/
CSprite * CSpriteNodeMultiLst::getSprite()
{
    return &m_sprite;
}

/************************************************************************
*    DESC:  Get the sprite id number
************************************************************************/
int CSpriteNodeMultiLst::getId() const
{
    return m_sprite.getId();
}

/************************************************************************
*    DESC:  Get the object
************************************************************************/
CObjectTransform * CSpriteNodeMultiLst::getObject()
{
    return &m_sprite;
}
