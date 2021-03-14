
/************************************************************************
*    FILE NAME:       node.h
*
*    DESCRIPTION:     Class for creating a general tree.
*                     Each node can have arbitrary number of children.
*                     NOTE: Building the tree involves finding the parent
*                           via it's id. Each node has it's id and it's
*                           parent id
************************************************************************/

#pragma once

// Physical component dependency
#include <node/inode.h>

// Standard lib dependencies
#include <vector>
#include <string>

class CNode : public iNode
{
public:

    // Constructor
    CNode( uint8_t nodeId, uint8_t parentId );

    // Destructor
    virtual ~CNode();

    // Get the starting position of the vector iterator
    nodeVecIter_t getNodeIter() override
    { return m_nodeVec.begin(); }

    // Get the next node
    iNode * next(nodeVecIter_t & rIter) override;

    // Add a node
    bool addNode( iNode * pNode ) override;

    // Find the parent
    // NOTE: This is a recursive function
    iNode * findParent( iNode * pSearchNode ) override;

    // Find the child
    iNode * getChildNode( const std::string & nodeName );

    // Find the child
    // NOTE: This is a recursive function
    iNode * findChild( const uint16_t childId ) override;
    
    // Push back node into vector
    void pushBackNode( iNode * pNode ) override;

protected:

    // Node vector of children
    std::vector<iNode *> m_nodeVec;
};
