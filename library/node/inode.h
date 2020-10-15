
/************************************************************************
*    FILE NAME:       inode.h
*
*    DESCRIPTION:     Node interface class. Also used as a leaf node
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
class CObject;
class CUIControl;

typedef std::vector<class iNode *>::iterator nodeVecIter_t;

class iNode
{
public:

    // Constructor
    iNode( uint8_t nodeId = defs_DEFAULT_NODE_ID, uint8_t parentId = defs_DEFAULT_NODE_ID );

    // Destructor
    virtual ~iNode();

    // Get the next node
    virtual nodeVecIter_t getNodeIter()
    { return nodeVecIter_t(); }

    // Get the next node
    virtual iNode * next(nodeVecIter_t & rIter)
    { return nullptr; }

    // Add a node
    virtual bool addNode( iNode * pNode )
    { return false; }
    
    // Push back node into vector
    virtual void pushBackNode( iNode * pNode ){}

    // Find the parent
    // NOTE: This is a recursive function
    virtual iNode * findParent( iNode * pSearchNode )
    { return nullptr; }

    // Get the child node
    virtual iNode * findChild( const uint16_t crcValue );

    // Get the child node
    virtual iNode * getChildNode( const std::string & nodeName )
    { return nullptr; }

    // Get the id number
    handle16_t getHandle() const
    { return m_handle; }

    // Get the user id number
    int getId() const
    { return m_userId; }

    // Get the node id number
    uint8_t getNodeId() const
    { return m_nodeId; }

    // Get the parent id
    uint8_t getParentId() const
    { return m_parentId; }

    // Update the nodes
    virtual void update(){}

    // Transform the nodes
    virtual void transform(){}
    // Used to transform object on a sector
    virtual void transform( const CObject & object ){}

    // Record the command buffer
    virtual void recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer, const CCamera & camera ){}
    
    // Get the node type
    virtual ENodeType getType() const
    { return m_type; }
    
    // Get the object
    virtual CObject * getObject()
    { return nullptr; }
    
    // Get the sprite
    virtual CSprite * getSprite()
    { return nullptr; }

    // Get the control
    virtual CUIControl * getControl()
    { return nullptr; }

protected:

    // Node type
    ENodeType m_type;

    // Atomic handle incrementer
    static std::atomic<handle16_t> m_hAtomicIter;

    // unique node handle
    handle16_t m_handle;

    // user id
    int16_t m_userId;

    // node id
    uint8_t m_nodeId;

    // parent node id
    uint8_t m_parentId;

    // CRC user id. CRC value of string name
    // So that the string doesn't have to be stored
    uint16_t m_crcUserId;
};

#endif
