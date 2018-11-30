
/************************************************************************
*    FILE NAME:       nodemultilist.cpp
*
*    DESCRIPTION:     Node multi link list class
************************************************************************/

// Physical component dependency
#include <node/nodemultilist.h>

// Game lib dependencies
#include <utilities/deletefuncs.h>
#include <2d/object2d.h>
#include <sprite/sprite.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CNodeMultiLst::CNodeMultiLst( int nodeId, int parentId ) :
    CNode(nodeId, parentId)
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CNodeMultiLst::~CNodeMultiLst()
{
    NDelFunc::DeleteVectorPointers( m_allNodeVec );
}


/***************************************************************************
*    DESC:  Update the nodes.
****************************************************************************/
void CNodeMultiLst::update()
{
    update( this );
    
    resetIterators();
}

void CNodeMultiLst::update( iNode * pNode )
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
                if( pNextNode->getSprite() != nullptr )
                {
                    pNextNode->getSprite()->physicsUpdate();
                    pNextNode->getSprite()->update();
                }
                
                // Call a recursive function again
                update( pNextNode );
            }
        }
        while( pNextNode != nullptr );
    }
}


/***************************************************************************
*    DESC:  Translate the nodes
****************************************************************************/
void CNodeMultiLst::transform()
{
    transform( this );
    
    resetIterators();
}

void CNodeMultiLst::transform( const CMatrix & matrix, bool tranformWorldPos )
{
    transform( this );
    
    resetIterators();
}

void CNodeMultiLst::transform( iNode * pNode )
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
                CObject2D * pNextObj(nullptr);
                CObject2D * pObj(nullptr);
                
                if( pNextNode->getSprite() != nullptr )
                    pNextObj = pNextNode->getSprite()->getObject();
                
                else if( pNextNode->getObject() != nullptr )
                    pNextObj = pNextNode->getObject();
                
                if( pNode->getSprite() != nullptr )
                    pObj = pNode->getSprite()->getObject();
                
                else if( pNode->getObject() != nullptr )
                    pObj = pNode->getObject();
                
                // Transform the child node
                pNextObj->transform(
                    pObj->getMatrix(),
                    pObj->wasWorldPosTranformed() );

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
void CNodeMultiLst::recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer, const CCamera & camera )
{
    recordCommandBuffer( this, index, cmdBuffer, camera );
    
    resetIterators();
}

void CNodeMultiLst::recordCommandBuffer( iNode * pNode, uint32_t index, VkCommandBuffer cmdBuffer, const CCamera & camera )
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
                if( pNextNode->getSprite() != nullptr )
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
bool CNodeMultiLst::addNode( iNode * pNode )
{
    m_allNodeVec.push_back( pNode );

    const bool result = CNode::addNode( pNode );

    resetIterators();

    return result;
}


/************************************************************************
*    DESC:  Reset the iterators
************************************************************************/
void CNodeMultiLst::resetIterators()
{
    reset();

    for( auto iter : m_allNodeVec )
        iter->reset();
}
