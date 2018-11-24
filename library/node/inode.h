
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

// Vulkan lib dependencies
#include <system/vulkan.h>

// Forward declaration(s)
class CMatrix;
class CSprite;
class CCamera;

class iNode
{
public:

    // Constructor
    iNode() : m_type(NDefs::ENT_NULL)
    {}

    // Destructor
    virtual ~iNode(){}

    // Get the next node
    virtual iNode * next()
    { return nullptr; }

    // Add a node
    virtual bool addNode( iNode * pNode )
    { return false; }
    
    // Reset the iterator
    virtual void reset(){}
    
    // Push back node into vector
    virtual void pushBackNode( iNode * pNode ){}

    // Find the parent
    // NOTE: This is a recursive function
    virtual iNode * findParent( iNode * pSearchNode )
    { return nullptr; }

    // Get the unique head node id number
    virtual int getId() const
    { return 0; }

    // Get the parent id
    virtual int getParentId() const
    { return 0; }

    // Update the nodes
    virtual void update(){}

    // Transform the nodes
    virtual void transform(){}
    virtual void transform( const CMatrix & matrix, bool tranformWorldPos = false ){}

    // Record the command buffer
    virtual void recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer, const CCamera & camera ){}
    
    // Get the node type
    virtual NDefs::ENodeType getType() const
    { return m_type; }
    
    // Get the sprite
    virtual CSprite * getSprite()
    { return nullptr; }
    
    // Reset the iterators
    virtual void resetIterators(){}

protected:

    // Node type
    NDefs::ENodeType m_type;
};

#endif
