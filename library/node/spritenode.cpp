
/************************************************************************
*    FILE NAME:       spritedata.cpp
*
*    DESCRIPTION:     Sprite Data Class
************************************************************************/

// Physical component dependency
#include <node/spritenode.h>

// Game lib dependencies
#include <utilities/deletefuncs.h>
#include <2d/object2d.h>


/************************************************************************
*    DESC:  Constructor
************************************************************************/
CSpriteHeadNode::CSpriteHeadNode(
    int id,
    const CObjectData2D & objectData,
    int nodeId,
    int parentId,
    int spriteId ) :
        CSpriteNode(objectData, nodeId, parentId, spriteId),
        m_id(id)
{
}

CSpriteHeadNode::CSpriteHeadNode(
    int id,
    const CObjectData3D & objectData,
    int nodeId,
    int parentId,
    int spriteId ) :
        CSpriteNode(objectData, nodeId, parentId, spriteId),
        m_id(id)
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CSpriteHeadNode::~CSpriteHeadNode()
{
    NDelFunc::DeleteVectorPointers( m_nodeVec );
}


/***************************************************************************
*    DESC:  Update the nodes.
****************************************************************************/
void CSpriteHeadNode::update()
{
    for( auto iter : m_nodeVec )
    {
        iter->getSprite().update();
        iter->getSprite().physicsUpdate();
    }
}


/***************************************************************************
*    DESC:  Update the nodes
****************************************************************************/
void CSpriteHeadNode::transform()
{
    m_sprite.getObject()->transform();

    transform( this );
}

void CSpriteHeadNode::transform( iNode * pNode )
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
                auto * pParentNode = dynamic_cast<CSpriteNode *>(pNode);
                auto * pChildNode = dynamic_cast<CSpriteNode *>(pNextNode);

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
void CSpriteHeadNode::recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer, const CMatrix & viewProj )
{
    m_sprite.recordCommandBuffers( index, cmdBuffer, viewProj );
    
    recordCommandBuffer( this, index, cmdBuffer, viewProj );
}

void CSpriteHeadNode::recordCommandBuffer( iNode * pNode, uint32_t index, VkCommandBuffer cmdBuffer, const CMatrix & viewProj )
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
                auto * pChildNode = dynamic_cast<CSpriteNode *>(pNextNode);
                
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
bool CSpriteHeadNode::addNode( iNode * pNode )
{
    m_nodeVec.push_back( dynamic_cast<CSpriteNode *>(pNode) );

    bool result = CSpriteNode::addNode( pNode );

    resetIterators();

    return result;
}


/************************************************************************
*    DESC:  Reset the iterators
************************************************************************/
void CSpriteHeadNode::resetIterators()
{
    reset();

    for( auto iter : m_nodeVec )
        iter->reset();
}


/************************************************************************
*    DESC:  Get the unique head node id number
************************************************************************/
int CSpriteHeadNode::getId() const
{
    return m_id;
}
