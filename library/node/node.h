
/************************************************************************
*    FILE NAME:       node.h
*
*    DESCRIPTION:     Node class
*                     NOTE: Building the tree involves finding the parent
*                           via it's id. Each node has it's id and it's
*                           parent id
************************************************************************/

#ifndef __node_h__
#define __node_h__

// Physical component dependency
#include <node/inode.h>

// Game lib dependencies
#include <common/defs.h>

// Standard lib dependencies
#include <vector>

class CNode : public iNode
{
public:

    // Constructor
    CNode( int id, int parentId );

    // Destructor
    virtual ~CNode();

    // Get the next node
    iNode * next() override;

    // Add a node
    virtual bool addNode( iNode * pNode ) override;

    // Find the parent
    // NOTE: This is a recursive function
    iNode * findParent( iNode * pSearchNode ) override;
    
    // Reset the iterator
    void reset();
    
    // Get the parent id
    int getParentId() const override;

protected:

    // node vector. Do NOT free. Head node will free
    std::vector<iNode *> m_nodeVec;

    // next node iterator
    std::vector<iNode *>::iterator m_Iter;

    // node id
    int m_id;

    // parent node id
    int m_parentId;
};

#endif
