
/************************************************************************
*    FILE NAME:       nodemultilist.h
*
*    DESCRIPTION:     Node multi link list class
************************************************************************/

#ifndef __node_multi_list_h__
#define __node_multi_list_h__

// Physical component dependency
#include <node/node.h>

// Standard lib dependencies
#include <map>
#include <string>

class CNodeMultiLst : public CNode
{
public:
    
    // Constructor
    CNodeMultiLst( int nodeId = defs_NODE_DEFAULT_ID, int parentId = defs_PARENT_NODE_DEFAULT_ID );

    // Destructor
    virtual ~CNodeMultiLst();
    
    // Update the nodes
    virtual void update() override;
    
    // Transform the nodes
    virtual void transform() override;
    
    // Record the command buffer vector in the device
    // for all the sprite objects that are to be rendered
    virtual void recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer, const CCamera & camera ) override;
    
    // Add a node
    bool addNode( iNode * pNode, const std::string & nodeName = "" ) override;
    
    // Get the child node
    iNode * getChildNode( const std::string & nodeName ) override;
    
private:
    
    // Reset the iterators
    void resetIterators() override;
    
    // Update the nodes
    void update( iNode * pNode );
    
    // Update the nodes recursive function
    void transform( iNode * pNode );
    
    // Record command buffer recursive function
    void recordCommandBuffer( iNode * pNode, uint32_t index, VkCommandBuffer cmdBuffer, const CCamera & camera );
    
private:
    
    // List of all nodes.
    // This is only used by the head node and even though
    // every node will have one of these, it simplifies
    // the code and is minimal overhead
    // This class OWNES these pointers
    std::map<const std::string, iNode *> m_allNodeMap;
};

#endif
