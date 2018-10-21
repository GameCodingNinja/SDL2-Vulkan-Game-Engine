
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

// Standard lib dependencies
#include <vector>

class iNode
{
public:

    // Constructor
    iNode( int id, int parentId );

    // Destructor
    virtual ~iNode();

    // Get the next node
    iNode * next();

    // Add a node
    virtual bool addNode( iNode * pNode );

    // Find the parent
    // NOTE: This is a recursive function
    iNode * findParent( iNode * pSearchNode );

    // Get the node type
    NDefs::ENodeType getType() const;
    
    // Reset the iterator
    void reset();

protected:

    // Node type
    NDefs::ENodeType m_type = NDefs::ENT_NULL;

    // node vector. Do NOT free. Head node will free
    std::vector<iNode *> m_nodeVec;

    // next node iterator
    std::vector<iNode *>::iterator nodeIter;

    // node id
    int m_id;

    // parent node id
    int m_parentId;
};

#endif
