
/************************************************************************
*    FILE NAME:       spriteheadnodemultilist.cpp
*
*    DESCRIPTION:     Sprite head node multi link list class
************************************************************************/

// Physical component dependency
#include <node/spritenodemultilist.h>

// Game lib dependencies
#include <utilities/deletefuncs.h>
#include <2d/object2d.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CSpriteNodeMultiLst::CSpriteNodeMultiLst(
    const CObjectData2D & objectData,
    int spriteId,
    int nodeId,
    int parentId ) :
        CNode(nodeId, parentId),
        m_sprite(objectData, spriteId)
{
    m_type = NDefs::ENT_SPRITE;
}

CSpriteNodeMultiLst::CSpriteNodeMultiLst(
    const CObjectData3D & objectData,
    int spriteId,
    int nodeId,
    int parentId ) :
        CNode(nodeId, parentId),
        m_sprite(objectData, spriteId)
{
    m_type = NDefs::ENT_SPRITE;
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CSpriteNodeMultiLst::~CSpriteNodeMultiLst()
{
    NDelFunc::DeleteVectorPointers( m_allNodeVec );
}


/***************************************************************************
*    DESC:  Update the nodes.
****************************************************************************/
void CSpriteNodeMultiLst::update()
{
    m_sprite.update();
    m_sprite.physicsUpdate();
        
    update( this );
    
    resetIterators();
}

void CSpriteNodeMultiLst::update( iNode * pNode )
    {
    if( pNode != nullptr )
    {
        iNode * pNextNode;

        do
        {
            // get the next node
            pNextNode = pNode->next();

            if( pNextNode != nullptr )
            {
                // Cast the child node
                auto * pChildNode = dynamic_cast<CSpriteNodeMultiLst *>(pNextNode);
                
                // Update the children
                pChildNode->getSprite().update();
                pChildNode->getSprite().physicsUpdate();
                
                // Call a recursive function again
                update( pNextNode );
            }
        }
        while( pNextNode != nullptr );
    }
}


/***************************************************************************
*    DESC:  Update the nodes
****************************************************************************/
void CSpriteNodeMultiLst::transform()
{
    m_sprite.getObject()->transform();

    transform( this );
    
    resetIterators();
}

void CSpriteNodeMultiLst::transform( iNode * pNode )
{
    if( pNode != nullptr )
    {
        iNode * pNextNode;

        do
        {
            // get the next node
            pNextNode = pNode->next();

            if( pNextNode != nullptr )
            {
                // Cast the parent and child nodes
                auto * pParentNode = dynamic_cast<CSpriteNodeMultiLst *>(pNode);
                auto * pChildNode = dynamic_cast<CSpriteNodeMultiLst *>(pNextNode);

                // Transform the child node
                pChildNode->getSprite().getObject()->transform(
                    pParentNode->getSprite().getObject()->getMatrix(),
                    pParentNode->getSprite().getObject()->wasWorldPosTranformed() );

                // Call a recursive function again
                transform( pNextNode );
            }
        }
        while( pNextNode != nullptr );
    }
}


/***************************************************************************
*    DESC:  Record the command buffer vector in the device
*           for all the sprite objects that are to be rendered
****************************************************************************/
void CSpriteNodeMultiLst::recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer, const CMatrix & viewProj )
{
    m_sprite.recordCommandBuffers( index, cmdBuffer, viewProj );
    
    recordCommandBuffer( this, index, cmdBuffer, viewProj );
    
    resetIterators();
}

void CSpriteNodeMultiLst::recordCommandBuffer( iNode * pNode, uint32_t index, VkCommandBuffer cmdBuffer, const CMatrix & viewProj )
{
    if( pNode != nullptr )
    {
        iNode * pNextNode;

        do
        {
            // get the next node
            pNextNode = pNode->next();

            if( pNextNode != nullptr )
            {
                // Cast the child node
                auto * pChildNode = dynamic_cast<CSpriteNodeMultiLst *>(pNextNode);
                
                // Record the command buffer
                pChildNode->getSprite().recordCommandBuffers( index, cmdBuffer, viewProj );

                // Call a recursive function again
                recordCommandBuffer( pNextNode, index, cmdBuffer, viewProj );
            }
        }
        while( pNextNode != nullptr );
    }
}


/************************************************************************
*    DESC:  Add a node
************************************************************************/
bool CSpriteNodeMultiLst::addNode( iNode * pNode )
{
    m_allNodeVec.push_back( dynamic_cast<CSpriteNodeMultiLst *>(pNode) );

    const bool result = CNode::addNode( pNode );

    resetIterators();

    return result;
}


/************************************************************************
*    DESC:  Reset the iterators
************************************************************************/
void CSpriteNodeMultiLst::resetIterators()
{
    reset();

    for( auto iter : m_allNodeVec )
        iter->reset();
}


/************************************************************************
*    DESC:  Get the sprite
************************************************************************/
CSprite & CSpriteNodeMultiLst::getSprite()
{
    return m_sprite;
}


/************************************************************************
*    DESC:  Get the sprite id number
************************************************************************/
int CSpriteNodeMultiLst::getId() const
{
    return m_sprite.getId();
}
