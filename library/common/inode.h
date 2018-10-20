
/************************************************************************
*    FILE NAME:       inode.h
*
*    DESCRIPTION:     Node interface class
*                     NOTE: Building the tree involves finding the parent
*                           via it's id. Each node has it's id and it's
*                           parent id
************************************************************************/

#ifndef __i_node_h__
#define __i_node_h__

// Game lib dependencies
#include <common/defs.h>
#include <utilities/deletefuncs.h>

// Standard lib dependencies
#include <vector>

template <typename type>
class iNode
{
public:

    // Constructor
    iNode( int id, int parentId ) : m_id(id), m_parentId(parentId)
    {
        reset();
    }

    // Destructor
    virtual ~iNode()
    {
        NDelFunc::DeleteVectorPointers( m_nodeVec );
    }

    /************************************************************************
    *    DESC:  Get the next node
    ************************************************************************/
    iNode * next()
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
    bool addNode( iNode * pNode, iNode * pHeadNode )
    {
        // Call a recursive function to find the parent node
        iNode * pParentNode = FindParent( pNode, pHeadNode );

        // Add the node
        if( pParentNode != nullptr )
            pParentNode->m_nodeVec.push_back(pNode);
        else
            return false;

        return true;
    }

    /************************************************************************
    *    DESC:  Find the parent
    ************************************************************************/
    iNode * FindParent( iNode * pSearchNode )
    {
        iNode * pResult = nullptr;

        if( pSearchNode != nullptr )
        {
            if( m_parentId == pSearchNode->m_id )
            {
                pResult = pSearchNode;
            }
            else
            {
                iNode * pNextNode;

                do
                {
                    // get the next node
                    pNextNode = pSearchNode->next();

                    if( pNextNode != nullptr )
                    {
                        // Call a recursive function to find the parent node
                        pResult = FindParent( pNextNode );
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
    void reset()
    {
        nodeIter = m_nodeVec.begin();
    }

    /************************************************************************
    *    DESC:  Get the node type
    ************************************************************************/
    NDefs::ENodeType getType() const
    {
        return m_type;
    }

    /************************************************************************
    *    DESC:  Get the data
    ************************************************************************/
    virtual type & getData() = 0;

protected:

    // Node type
    NDefs::ENodeType m_type = NDefs::ENT_NULL;

private:

    // node vector
    std::vector<iNode *> m_nodeVec;

    // next node iterator
    typename std::vector<iNode *>::iterator nodeIter;

    // node id
    int m_id;

    // parent node id
    int m_parentId;
};

#endif
