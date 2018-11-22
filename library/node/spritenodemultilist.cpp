
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
                // Update the children
                pNextNode->getSprite()->update();
                pNextNode->getSprite()->physicsUpdate();
                
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

void CSpriteNodeMultiLst::transform( const CMatrix & matrix, bool tranformWorldPos )
{
    m_sprite.getObject()->transform( matrix, tranformWorldPos );

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
                // Transform the child node
                pNextNode->getSprite()->getObject()->transform(
                    pNode->getSprite()->getObject()->getMatrix(),
                    pNode->getSprite()->getObject()->wasWorldPosTranformed() );

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
void CSpriteNodeMultiLst::recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer, const CCamera & camera )
{
    m_sprite.recordCommandBuffer( index, cmdBuffer, camera );
    
    recordCommandBuffer( this, index, cmdBuffer, camera );
    
    resetIterators();
}

void CSpriteNodeMultiLst::recordCommandBuffer( iNode * pNode, uint32_t index, VkCommandBuffer cmdBuffer, const CCamera & camera )
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
                // Record the command buffer
                pNextNode->getSprite()->recordCommandBuffer( index, cmdBuffer, camera );

                // Call a recursive function again
                recordCommandBuffer( pNextNode, index, cmdBuffer, camera );
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
    m_allNodeVec.push_back( pNode );

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
