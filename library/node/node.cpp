
/************************************************************************
*    FILE NAME:       node.cpp
*
*    DESCRIPTION:     Node class
*                     NOTE: Building the tree involves finding the parent
*                           via it's id. Each node has it's id and it's
*                           parent id
************************************************************************/

// Physical component dependency
#include <node/node.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CNode::CNode( int id, int parentId ) :
    m_id(id),
    m_parentId(parentId)
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CNode::~CNode()
{
}


/************************************************************************
*    DESC:  Get the next node
************************************************************************/
iNode * CNode::next()
{
    iNode * pResult = nullptr;

    // Get the next node
    if( m_Iter != m_nodeVec.end() )
    {
        pResult = *m_Iter;
        ++m_Iter;
    }

    return pResult;
}


/************************************************************************
*    DESC:  Add a node
************************************************************************/
bool CNode::addNode( iNode * pNode, const std::string & nodeName )
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
        if( m_id == pSearchNode->getParentId() )
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
*    DESC:  Reset the iterator
************************************************************************/
void CNode::reset()
{
    if( !m_nodeVec.empty() )
        m_Iter = m_nodeVec.begin();
}


/************************************************************************
*    DESC:  Get the parent id
************************************************************************/
int CNode::getParentId() const
{
    return m_parentId;
}
