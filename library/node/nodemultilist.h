
/************************************************************************
*    FILE NAME:       nodemultilist.h
*
*    DESCRIPTION:     Node multi link list class
************************************************************************/

#ifndef __node_multi_list_h__
#define __node_multi_list_h__

// Physical component dependency
#include <node/node.h>

class CNodeMultiLst : public CNode
{
public:
    
    // Constructor
    CNodeMultiLst( int nodeId = -1, int parentId = -1 );

    // Destructor
    virtual ~CNodeMultiLst();
    
    // Update the nodes
    virtual void update() override;
    
    // Transform the nodes
    virtual void transform() override;
    virtual void transform( const CMatrix & matrix, bool tranformWorldPos ) override;
    
    // Record the command buffer vector in the device
    // for all the sprite objects that are to be rendered
    virtual void recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer, const CCamera & camera ) override;
    
    // Add a node
    bool addNode( iNode * pNode ) override;
    
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
    std::vector<iNode *> m_allNodeVec;
};

#endif
