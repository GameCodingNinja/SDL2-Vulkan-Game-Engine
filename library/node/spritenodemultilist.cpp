
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
        CSprite(objectData)
{
    m_id = rNodeData.getId();
    m_type = NDefs::ENT_SPRITE;
}

/***************************************************************************
*    DESC:  Update the nodes.
****************************************************************************/
void CSpriteNodeMultiLst::update()
{
    CSprite::physicsUpdate();
    CSprite::update();
    
    // Call the parent but it has to be last
    CNodeMultiLst::update();
}

/***************************************************************************
*    DESC:  Translate the nodes
****************************************************************************/
void CSpriteNodeMultiLst::transform()
{
    CSprite::transform();

    // Call the parent but it has to be last
    CNodeMultiLst::transform();
}

// Used to transform object on a sector
void CSpriteNodeMultiLst::transform( const CObjectTransform & object )
{
    CSprite::transform( object );

    // Call the parent but it has to be last
    CNodeMultiLst::transform();
}

/***************************************************************************
*    DESC:  Record the command buffer vector in the device
*           for all the sprite objects that are to be rendered
****************************************************************************/
void CSpriteNodeMultiLst::recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer, const CCamera & camera )
{
    CSprite::recordCommandBuffer( index, cmdBuffer, camera );
    
    // Call the parent but it has to be last
    CNodeMultiLst::recordCommandBuffer( index, cmdBuffer, camera );
}

/************************************************************************
*    DESC:  Get the sprite
************************************************************************/
CSprite * CSpriteNodeMultiLst::getSprite()
{
    return static_cast<CSprite *>(this);
}

/************************************************************************
*    DESC:  Get the object
************************************************************************/
CObjectTransform * CSpriteNodeMultiLst::getObject()
{
    return static_cast<CObjectTransform *>(this);
}
