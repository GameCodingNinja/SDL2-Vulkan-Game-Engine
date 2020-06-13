
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
#include <atomic>

// Vulkan lib dependencies
#include <system/vulkan.h>

// Forward declaration(s)
class CSprite;
class CCamera;
class CObjectTransform;
class CUIControl;

typedef std::vector<class iNode *>::iterator nodeVecIter_t;

class iNode
{
public:

    // Constructor
    iNode( int nodeId = defs_DEFAULT_ID, int parentId = defs_DEFAULT_ID ) :
        m_type(NDefs::ENT_NULL),
        m_handle(m_hAtomicIter++),
        m_id(defs_DEFAULT_ID),
        m_nodeId(nodeId),
        m_parentId(parentId)
    {}

    // Destructor
    virtual ~iNode(){}

    // Get the next node
    virtual nodeVecIter_t getNodeIter()
    { return nodeVecIter_t(); }

    // Get the next node
    virtual iNode * next(nodeVecIter_t & rIter)
    { return nullptr; }

    // Add a node
    virtual bool addNode( iNode * pNode, const std::string & nodeName = "" )
    { return false; }
    
    // Push back node into vector
    virtual void pushBackNode( iNode * pNode ){}

    // Find the parent
    // NOTE: This is a recursive function
    virtual iNode * findParent( iNode * pSearchNode )
    { return nullptr; }

    // Get the id number
    handle16_t getHandle() const
    { return m_handle; }

    // Get the id number
    int getId() const
    { return m_id; }

    // Get the node id number
    int getNodeId() const
    { return m_nodeId; }

    // Get the parent id
    int getParentId() const
    { return m_parentId; }

    // Update the nodes
    virtual void update(){}

    // Transform the nodes
    virtual void transform(){}
    // Used to transform object on a sector
    virtual void transform( const CObjectTransform & object ){}

    // Record the command buffer
    virtual void recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer, const CCamera & camera ){}
    
    // Get the node type
    virtual NDefs::ENodeType getType() const
    { return m_type; }
    
    // Get the child node
    virtual iNode * getChildNode( const std::string & nodeName )
    { return nullptr; }
    
    // Get the object
    virtual CObjectTransform * getObject()
    { return nullptr; }
    
    // Get the sprite
    virtual CSprite * getSprite()
    { return nullptr; }

    // Get the control
    virtual CUIControl * getControl()
    { return nullptr; }

protected:

    // Node type
    NDefs::ENodeType m_type;

    // Atomic handle incrementer
    static std::atomic<handle16_t> m_hAtomicIter;

    // node handle
    handle16_t m_handle;

    // id
    int16_t m_id;

    // node id
    int16_t m_nodeId;

    // parent node id
    int16_t m_parentId;
};

#endif
