
/************************************************************************
*    FILE NAME:       rendernode.cpp
*
*    DESCRIPTION:     Class specifically handles resursion of
*                     game loop render functions
************************************************************************/

// Physical component dependency
#include <node/rendernode.h>

// Game lib dependencies
#include <utilities/exceptionhandling.h>
#include <sprite/sprite.h>
#include <common/object.h>
#include <gui/uicontrol.h>
#include <physics/iphysicscomponent.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    DESC:  Constructor / Destructor
************************************************************************/
CRenderNode::CRenderNode( uint8_t nodeId, uint8_t parentId ) :
    CNode(nodeId, parentId)
{
}

CRenderNode::~CRenderNode()
{}

/***************************************************************************
*    DESC:  Update the nodes.
****************************************************************************/
void CRenderNode::update()
{
    update( this );
}

void CRenderNode::update( iNode * pNode )
    {
    if( pNode != nullptr )
    {
        iNode * pNextNode;
        auto nodeIter = pNode->getNodeIter();

        do
        {
            // get the next node
            pNextNode = pNode->next(nodeIter);

            if( pNextNode != nullptr )
            {
                // Update the children
                if( pNextNode->getType() == ENodeType::SPRITE )
                {
                    pNextNode->getSprite()->physicsUpdate();
                    pNextNode->getSprite()->update();
                }
                else if( pNextNode->getType() == ENodeType::UI_CONTROL )
                {
                    pNextNode->getControl()->update();
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
void CRenderNode::transform()
{
    transform( this );
}

void CRenderNode::transform( iNode * pNode )
{
    if( pNode != nullptr )
    {
        iNode * pNextNode;
        auto nodeIter = pNode->getNodeIter();

        do
        {
            // get the next node
            pNextNode = pNode->next(nodeIter);

            if( pNextNode != nullptr )
            {
                // Transform the object
                if( pNextNode->getType() == ENodeType::UI_CONTROL )
                    pNextNode->getControl()->transform( *pNode->getObject() );
                    
                else
                    pNextNode->getObject()->transform( *pNode->getObject() );

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
void CRenderNode::recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer, const CCamera & camera )
{
    recordCommandBuffer( this, index, cmdBuffer, camera );
}

void CRenderNode::recordCommandBuffer( iNode * pNode, uint32_t index, VkCommandBuffer cmdBuffer, const CCamera & camera )
{
    if( pNode != nullptr )
    {
        iNode * pNextNode;
        auto nodeIter = pNode->getNodeIter();

        do
        {
            // get the next node
            pNextNode = pNode->next(nodeIter);

            if( pNextNode != nullptr )
            {
                // Record the command buffer
                if( pNextNode->getType() == ENodeType::SPRITE )
                    pNextNode->getSprite()->recordCommandBuffer( index, cmdBuffer, camera );

                else if( pNextNode->getType() == ENodeType::UI_CONTROL )
                    pNextNode->getControl()->recordCommandBuffer( index, cmdBuffer, camera );

                // Call a recursive function again
                recordCommandBuffer( pNextNode, index, cmdBuffer, camera );
            }
        }
        while( pNextNode != nullptr );
    }
}


/***************************************************************************
*    DESC:  Destroy the physics
****************************************************************************/
void CRenderNode::destroyPhysics()
{
    destroyPhysics( this );
}

void CRenderNode::destroyPhysics( iNode * pNode )
{
    if( pNode != nullptr )
    {
        iNode * pNextNode;
        auto nodeIter = pNode->getNodeIter();

        do
        {
            // get the next node
            pNextNode = pNode->next(nodeIter);

            if( pNextNode != nullptr )
            {
                // If this is a sprite, destroy the physics
                if( pNextNode->getType() == ENodeType::SPRITE )
                    pNextNode->getSprite()->destroyPhysics();
                
                // Call a recursive function again
                destroyPhysics( pNextNode );
            }
        }
        while( pNextNode != nullptr );
    }
}