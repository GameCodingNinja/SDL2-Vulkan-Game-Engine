
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
#include <string>

// Vulkan lib dependencies
#include <system/vulkan.h>

// Forward declaration(s)
class CMatrix;
class CSprite;
class CCamera;
class CObject2D;
class CUIControl;

class iNode
{
public:

    // Constructor
    iNode( int id = defs_DEFAULT_ID, int parentId = defs_DEFAULT_ID ) :
        m_type(NDefs::ENT_NULL), m_id(id), m_parentId(parentId)
    {}

    // Destructor
    virtual ~iNode(){}

    // Get the next node
    virtual iNode * next()
    { return nullptr; }

    // Add a node
    virtual bool addNode( iNode * pNode, const std::string & nodeName = "" )
    { return false; }
    
    // Reset the iterator
    virtual void reset(){}
    
    // Push back node into vector
    virtual void pushBackNode( iNode * pNode ){}

    // Find the parent
    // NOTE: This is a recursive function
    virtual iNode * findParent( iNode * pSearchNode )
    { return nullptr; }

    // Get the unique node id number
    virtual int getId() const
    { return m_id; }

    // Get the parent id
    virtual int getParentId() const
    { return m_parentId; }

    // Update the nodes
    virtual void update(){}

    // Transform the nodes
    virtual void transform(){}
    // Used to transform object on a sector
    virtual void transform( const CObject2D & object ){}

    // Record the command buffer
    virtual void recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer, const CCamera & camera ){}
    
    // Get the node type
    virtual NDefs::ENodeType getType() const
    { return m_type; }
    
    // Get the child node
    virtual iNode * getChildNode( const std::string & nodeName )
    { return nullptr; }
    
    // Get the object
    virtual CObject2D * getObject()
    { return nullptr; }
    
    // Get the sprite
    virtual CSprite * getSprite()
    { return nullptr; }

    // Get the control
    virtual CUIControl * getControl()
    { return nullptr; }
    
    // Reset the iterators
    virtual void resetIterators(){}

protected:

    // Node type
    NDefs::ENodeType m_type;

    // node id
    int m_id;

    // parent node id
    int m_parentId;
};

#endif
