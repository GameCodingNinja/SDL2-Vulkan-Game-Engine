
/************************************************************************
*    FILE NAME:       spriteheadnodemultilist.cpp
*
*    DESCRIPTION:     Sprite head node multi link list class
************************************************************************/

// Physical component dependency
#include <node/spriteheadnodemultilist.h>

// Game lib dependencies
#include <utilities/deletefuncs.h>
#include <2d/object2d.h>


/************************************************************************
*    DESC:  Constructor
************************************************************************/
CSpriteHeadNodeMultiLst::CSpriteHeadNodeMultiLst(
    const CObjectData2D & objectData,
    int spriteId,
    int nodeId,
    int parentId ) :
        CSpriteNodeMultiLst(objectData, spriteId, nodeId, parentId)
{
}

CSpriteHeadNodeMultiLst::CSpriteHeadNodeMultiLst(
    const CObjectData3D & objectData,
    int spriteId,
    int nodeId,
    int parentId ) :
        CSpriteNodeMultiLst(objectData, spriteId, nodeId, parentId)
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CSpriteHeadNodeMultiLst::~CSpriteHeadNodeMultiLst()
{
    NDelFunc::DeleteVectorPointers( m_nodeVec );
}


/***************************************************************************
*    DESC:  Update the nodes.
****************************************************************************/
void CSpriteHeadNodeMultiLst::update()
{
    m_sprite.update();
    m_sprite.physicsUpdate();
        
    for( auto iter : m_nodeVec )
    {
        iter->getSprite().update();
        iter->getSprite().physicsUpdate();
    }
}


/***************************************************************************
*    DESC:  Update the nodes
****************************************************************************/
void CSpriteHeadNodeMultiLst::transform()
{
    m_sprite.getObject()->transform();

    transform( this );
    
    resetIterators();
}

void CSpriteHeadNodeMultiLst::transform( iNode * pNode )
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
void CSpriteHeadNodeMultiLst::recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer, const CMatrix & viewProj )
{
    m_sprite.recordCommandBuffers( index, cmdBuffer, viewProj );
    
    recordCommandBuffer( this, index, cmdBuffer, viewProj );
    
    resetIterators();
}

void CSpriteHeadNodeMultiLst::recordCommandBuffer( iNode * pNode, uint32_t index, VkCommandBuffer cmdBuffer, const CMatrix & viewProj )
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
bool CSpriteHeadNodeMultiLst::addNode( iNode * pNode )
{
    m_nodeVec.push_back( dynamic_cast<CSpriteNodeMultiLst *>(pNode) );

    bool result = CSpriteNodeMultiLst::addNode( pNode );

    resetIterators();

    return result;
}


/************************************************************************
*    DESC:  Reset the iterators
************************************************************************/
void CSpriteHeadNodeMultiLst::resetIterators()
{
    reset();

    for( auto iter : m_nodeVec )
        iter->reset();
}
