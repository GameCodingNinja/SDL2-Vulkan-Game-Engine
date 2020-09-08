
/************************************************************************
*    FILE NAME:       rendernode.h
*
*    DESCRIPTION:     Class specifically handles resursion of
*                     game loop render functions
************************************************************************/

#ifndef __render_node_h__
#define __render_node_h__

// Physical component dependency
#include <node/node.h>

// Standard lib dependencies
#include <map>
#include <string>

class CRenderNode : public CNode
{
public:
    
    // Constructor
    CRenderNode( uint8_t nodeId = defs_DEFAULT_NODE_ID, uint8_t parentId = defs_DEFAULT_NODE_ID );

    // Destructor
    virtual ~CRenderNode();
    
    // Update the nodes
    virtual void update() override;
    
    // Transform the nodes
    virtual void transform() override;
    
    // Record the command buffer vector in the device
    // for all the sprite objects that are to be rendered
    virtual void recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer, const CCamera & camera ) override;
    
private:
    
    // Update the nodes
    void update( iNode * pNode );
    
    // Update the nodes recursive function
    void transform( iNode * pNode );
    
    // Record command buffer recursive function
    void recordCommandBuffer( iNode * pNode, uint32_t index, VkCommandBuffer cmdBuffer, const CCamera & camera );
};

#endif