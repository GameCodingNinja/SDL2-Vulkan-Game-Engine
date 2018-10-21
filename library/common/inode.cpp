
/************************************************************************
*    FILE NAME:       inode.cpp
*
*    DESCRIPTION:     Node interface class
*                     NOTE: Building the tree involves finding the parent
*                           via it's id. Each node has it's id and it's
*                           parent id
************************************************************************/

// Physical component dependency
#include <common/inode.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
iNode::iNode( int id, int parentId ) : nodeIter(nullptr), m_id(id), m_parentId(parentId)
{
    //nodeIter = m_nodeVec.begin();
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
iNode::~iNode()
{
}


/************************************************************************
*    DESC:  Get the next node
************************************************************************/
iNode * iNode::next()
{
    iNode * pResult = nullptr;

    // Get the next node
    if( nodeIter != m_nodeVec.end() )
    {
        pResult = *nodeIter;
        ++nodeIter;
    }

    return pResult;
}


/************************************************************************
*    DESC:  Add a node
************************************************************************/
bool iNode::addNode( iNode * pNode )
{
    // Call a recursive function to find the parent node
    iNode * pParentNode = findParent( pNode );

    // Add the node
    if( pParentNode != nullptr )
        pParentNode->m_nodeVec.push_back( pNode );

    else
        return false;

    return true;
}


/************************************************************************
*    DESC:  Find the parent
*           NOTE: This is a recursive function
************************************************************************/
iNode * iNode::findParent( iNode * pSearchNode )
{
    iNode * pResult = nullptr;

    if( pSearchNode != nullptr )
    {
        if( m_id == pSearchNode->m_parentId )
        {
            pResult = this;
        }
        else
        {
            iNode * pNextNode;

            do
            {
                // get the next node
                pNextNode = next();

                if( pNextNode != nullptr )
                {
                    // Call a recursive function to find the parent node
                    pResult = pNextNode->findParent( pSearchNode );
                }
            }
            while( pNextNode != nullptr );
        }
    }

    return pResult;
}


/************************************************************************
*    DESC:  Get the node type
************************************************************************/
NDefs::ENodeType iNode::getType() const
{
    return m_type;
}


/************************************************************************
*    DESC:  Reset the iterator
************************************************************************/
void iNode::reset()
{
    if( !m_nodeVec.empty() )
        nodeIter = m_nodeVec.begin();
}
