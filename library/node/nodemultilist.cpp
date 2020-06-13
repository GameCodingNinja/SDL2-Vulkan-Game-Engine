
/************************************************************************
*    FILE NAME:       nodemultilist.cpp
*
*    DESCRIPTION:     Node multi link list class
************************************************************************/

// Physical component dependency
#include <node/nodemultilist.h>

// Game lib dependencies
#include <utilities/deletefuncs.h>
#include <utilities/exceptionhandling.h>
#include <sprite/sprite.h>
#include <common/objecttransform.h>
#include <gui/uicontrol.h>

// Boost lib dependencies
#include <boost/format.hpp>

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
    NDelFunc::DeleteMapPointers( m_allNodeMap );
}

/***************************************************************************
*    DESC:  Update the nodes.
****************************************************************************/
void CNodeMultiLst::update()
{
    update( this );
}

void CNodeMultiLst::update( iNode * pNode )
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
                if( pNextNode->getType() == NDefs::ENT_SPRITE )
                {
                    pNextNode->getSprite()->physicsUpdate();
                    pNextNode->getSprite()->update();
                }
                else if( pNextNode->getType() == NDefs::ENT_UI_CONTROL )
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
void CNodeMultiLst::transform()
{
    transform( this );
}

void CNodeMultiLst::transform( iNode * pNode )
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
                if( pNextNode->getType() == NDefs::ENT_UI_CONTROL )
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
void CNodeMultiLst::recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer, const CCamera & camera )
{
    recordCommandBuffer( this, index, cmdBuffer, camera );
}

void CNodeMultiLst::recordCommandBuffer( iNode * pNode, uint32_t index, VkCommandBuffer cmdBuffer, const CCamera & camera )
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
                if( pNextNode->getType() == NDefs::ENT_SPRITE )
                    pNextNode->getSprite()->recordCommandBuffer( index, cmdBuffer, camera );

                else if( pNextNode->getType() == NDefs::ENT_UI_CONTROL )
                    pNextNode->getControl()->recordCommandBuffer( index, cmdBuffer, camera );

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
bool CNodeMultiLst::addNode( iNode * pNode, const std::string & nodeName )
{
    std::string name = nodeName;
    
    // If a name is not given, generate one for the map
    if( name.empty() )
        name = "blank_" + std::to_string(m_allNodeMap.size());
    
    // Check for duplicate names
    if( !m_allNodeMap.emplace( name, pNode ).second )
        throw NExcept::CCriticalException("Node create Error!",
                boost::str( boost::format("Duplicate node name (%s).\n\n%s\nLine: %s")
                    % nodeName % __FUNCTION__ % __LINE__ ));

    const bool result = CNode::addNode( pNode );

    return result;
}

/************************************************************************
*    DESC:  Get the child node
************************************************************************/
iNode * CNodeMultiLst::getChildNode( const std::string & nodeName )
{
    // Make sure the node we are looking for is available
    auto mapIter = m_allNodeMap.find( nodeName );
    if( mapIter == m_allNodeMap.end() )
        throw NExcept::CCriticalException("Get child node Error!",
            boost::str( boost::format("Child node can't be found (%s).\n\n%s\nLine: %s")
                % nodeName % __FUNCTION__ % __LINE__ ));
    
    return mapIter->second;
}
