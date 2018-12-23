
/************************************************************************
*    FILE NAME:       spritenodemultilist.cpp
*
*    DESCRIPTION:     Sprite node multi link list class
************************************************************************/

// Physical component dependency
#include <node/spritenodemultilist.h>

// Game lib dependencies
#include <2d/object2d.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CSpriteNodeMultiLst::CSpriteNodeMultiLst(
    const CObjectData2D & objectData,
    int spriteId,
    int nodeId,
    int parentId ) :
        CNodeMultiLst(nodeId, parentId),
        m_sprite(objectData, spriteId)
{
    m_type = NDefs::ENT_SPRITE_MULTI_LIST;
}

CSpriteNodeMultiLst::CSpriteNodeMultiLst(
    const CObjectData3D & objectData,
    int spriteId,
    int nodeId,
    int parentId ) :
        CNodeMultiLst(nodeId, parentId),
        m_sprite(objectData, spriteId)
{
    m_type = NDefs::ENT_SPRITE_MULTI_LIST;
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
    m_sprite.getObject()->transform();

    // Call the parent but it has to be last
    CNodeMultiLst::transform();
}

void CSpriteNodeMultiLst::transform( const CMatrix & matrix, bool tranformWorldPos )
{
    m_sprite.getObject()->transform( matrix, tranformWorldPos );

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
