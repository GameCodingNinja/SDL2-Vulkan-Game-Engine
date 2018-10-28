
/************************************************************************
*    FILE NAME:       spriteheadnodemultilist.h
*
*    DESCRIPTION:     Sprite head node multi link list class
************************************************************************/

#ifndef __sprite_node_multi_list_h__
#define __sprite_node_multi_list_h__

// Physical component dependency
#include <node/node.h>

// Game lib dependencies
#include <sprite/sprite.h>

// Forward declaration(s)
class CMatrix;
class CObjectData2D;
class CObjectData3D;

class CSpriteNodeMultiLst : public CNode
{
public:
    
    // Constructor
    CSpriteNodeMultiLst(
        const CObjectData2D & objectData,
        int spriteId = defs_SPRITE_DEFAULT_ID,
        int nodeId = -1,
        int parentId = -1 );

    CSpriteNodeMultiLst(
        const CObjectData3D & objectData,
        int spriteId = defs_SPRITE_DEFAULT_ID,
        int nodeId = -1,
        int parentId = -1 );

    // Destructor
    virtual ~CSpriteNodeMultiLst();
    
    // Do any init
    void init() override;
    
    // Update the nodes
    void update() override;
    
    // Transform the nodes
    void transform() override;
    void transform( const CMatrix & matrix, bool tranformWorldPos );
    
    // Record the command buffer vector in the device
    // for all the sprite objects that are to be rendered
    void recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer, const CMatrix & viewProj ) override;
    void recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer, const CMatrix & rotMatrix, const CMatrix & viewProj ) override;
    
    // Add a node
    bool addNode( iNode * pNode ) override;
    
    // Get the sprite
    CSprite * getSprite() override;
    
    // Get the sprite id number
    int getId() const override;
    
private:
    
    // Reset the iterators
    void resetIterators();
    
    // Update the nodes
    void update( iNode * pNode );
    
    // Update the nodes recursive function
    void transform( iNode * pNode );
    
    // Record command buffer recursive function
    void recordCommandBuffer( iNode * pNode, uint32_t index, VkCommandBuffer cmdBuffer, const CMatrix & viewProj );
    void recordCommandBuffer( iNode * pNode, uint32_t index, VkCommandBuffer cmdBuffer, const CMatrix & rotMatrix, const CMatrix & viewProj );
    
private:
    
    // Node data
    CSprite m_sprite;
    
    // List of all nodes.
    // This is only used by the head node and even though
    // every node will have one of these, it simplifies
    // the code and is minimal overhead
    std::vector<iNode *> m_allNodeVec;
};

#endif