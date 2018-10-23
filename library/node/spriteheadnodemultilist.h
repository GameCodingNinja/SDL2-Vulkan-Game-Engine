
/************************************************************************
*    FILE NAME:       spriteheadnodemultilist.h
*
*    DESCRIPTION:     Sprite head node multi link list class
************************************************************************/

#ifndef __sprite_head_node_multi_list_h__
#define __sprite_head_node_multi_list_h__

// Physical component dependency
#include <node/spritenodemultilist.h>

// Forward declaration(s)
class CMatrix;

class CSpriteHeadNodeMultiLst : public CSpriteNodeMultiLst
{
public:
    
    // Constructor
    CSpriteHeadNodeMultiLst(
        const CObjectData2D & objectData,
        int spriteId = defs_SPRITE_DEFAULT_ID,
        int nodeId = -1,
        int parentId = -1 );

    CSpriteHeadNodeMultiLst(
        const CObjectData3D & objectData,
        int spriteId = defs_SPRITE_DEFAULT_ID,
        int nodeId = -1,
        int parentId = -1 );

    // Destructor
    virtual ~CSpriteHeadNodeMultiLst();
    
    // Update the nodes
    void update() override;
    
    // Transform the nodes
    void transform() override;
    
    // Record the command buffer vector in the device
    // for all the sprite objects that are to be rendered
    void recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer, const CMatrix & viewProj ) override;
    
    // Add a node
    bool addNode( iNode * pNode ) override;
    
private:
    
    // Reset the iterators
    void resetIterators();
    
    // Update the nodes recursive function
    void transform( iNode * pNode );
    
    // Record command buffer recursive function
    void recordCommandBuffer( iNode * pNode, uint32_t index, VkCommandBuffer cmdBuffer, const CMatrix & viewProj );
    
private:
    
    // List of all nodes.
    std::vector<CSpriteNodeMultiLst *> m_nodeVec;
};

#endif
