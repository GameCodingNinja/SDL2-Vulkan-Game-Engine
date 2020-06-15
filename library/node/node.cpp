
/************************************************************************
*    FILE NAME:       node.cpp
*
*    DESCRIPTION:     Class for creating a general tree.
*                     Each node can have arbitrary number of children.
*                     NOTE: Building the tree involves finding the parent
*                           via it's id. Each node has it's id and it's
*                           parent id
************************************************************************/

// Physical component dependency
#include <node/node.h>

// Game lib dependencies
#include <utilities/deletefuncs.h>
#include <utilities/genfunc.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CNode::CNode( uint8_t nodeId, uint8_t parentId ) :
    iNode( nodeId, parentId )
{
}

/************************************************************************
*    DESC:  destructor
************************************************************************/
CNode::~CNode()
{
    NDelFunc::DeleteVectorPointers( m_nodeVec );
}

/************************************************************************
*    DESC:  Get the next node
************************************************************************/
iNode * CNode::next(nodeVecIter_t & rIter)
{
    iNode * pResult = nullptr;

    // Get the next node
    if( rIter != m_nodeVec.end() )
    {
        pResult = *rIter;
        ++rIter;
    }

    return pResult;
}

/************************************************************************
*    DESC:  Add a node
************************************************************************/
bool CNode::addNode( iNode * pNode )
{
    // Call a recursive function to find the parent node
    iNode * pParentNode = findParent( pNode );

    // Add the node
    if( pParentNode != nullptr )
        pParentNode->pushBackNode( pNode );

    else
        return false;

    return true;
}

/************************************************************************
*    DESC:  Push back node into vector
************************************************************************/
void CNode::pushBackNode( iNode * pNode )
{
    m_nodeVec.push_back( pNode );
}

/************************************************************************
*    DESC:  Find the parent
*           NOTE: This is a recursive function
************************************************************************/
iNode * CNode::findParent( iNode * pSearchNode )
{
    iNode * pResult = nullptr;

    if( pSearchNode != nullptr )
    {
        if( m_nodeId == pSearchNode->getParentId() )
        {
            pResult = this;
        }
        else
        {
            iNode * pNextNode;
            auto nodeIter = m_nodeVec.begin();

            do
            {
                // get the next node
                pNextNode = next(nodeIter);

                if( pNextNode != nullptr )
                {
                    // Call a recursive function to find the parent node
                    pResult = pNextNode->findParent( pSearchNode );
                }
            }
            while( (pNextNode != nullptr) && (pResult == nullptr)  );
        }
    }

    return pResult;
}

/************************************************************************
*    DESC:  get the child node
************************************************************************/
iNode * CNode::getChildNode( const std::string & nodeName )
{
    return findChild( NGenFunc::CalcCRC16( nodeName ) );
}

/************************************************************************
*    DESC:  get the child node
*           NOTE: This is a recursive function
************************************************************************/
iNode * CNode::findChild( const uint16_t crcValue )
{
    iNode * pResult = nullptr;

    if( crcValue == m_crcUserId )
    {
        pResult = this;
    }
    else
    {
        iNode * pNextNode;
        auto nodeIter = m_nodeVec.begin();

        do
        {
            // get the next node
            pNextNode = next(nodeIter);

            if( pNextNode != nullptr )
            {
                // Call a recursive function to find the parent node
                pResult = pNextNode->findChild( crcValue );
            }
        }
        while( (pNextNode != nullptr) && (pResult == nullptr) );
    }

    return pResult;
}