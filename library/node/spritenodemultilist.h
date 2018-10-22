
/************************************************************************
*    FILE NAME:       spritenodemultilist.h
*
*    DESCRIPTION:     Sprite node multi link list class
************************************************************************/

#ifndef __sprite_node_multi_list_h__
#define __sprite_node_multi_list_h__

// Physical component dependency
#include <node/node.h>

// Game lib dependencies
#include <sprite/sprite.h>

// Forward declaration(s)
class CObjectData2D;
class CObjectData3D;
class CMatrix;

class CSpriteNodeMultiLst : public CNode
{
public:

    // Constructor
    CSpriteNodeMultiLst(
        const CObjectData2D & objectData,
        int nodeId = -1,
        int parentId = -1,
        int spriteId = defs_SPRITE_DEFAULT_ID ) :
            CNode(nodeId, parentId),
            m_sprite(objectData, spriteId)
    {
        m_type = NDefs::ENT_SPRITE;
    }

    CSpriteNodeMultiLst(
        const CObjectData3D & objectData,
        int nodeId = -1,
        int parentId = -1,
        int spriteId = defs_SPRITE_DEFAULT_ID ) :
            CNode(nodeId, parentId),
            m_sprite(objectData, spriteId)
    {
        m_type = NDefs::ENT_SPRITE;
    }

    // Destructor
    virtual ~CSpriteNodeMultiLst(){}
    
    // Get the sprite
    CSprite & getSprite()
    { return m_sprite; }

protected:
    
    // Node data
    CSprite m_sprite;
};

class CSpriteHeadNodeMultiLst : public CSpriteNodeMultiLst
{
public:
    
    // Constructor
    CSpriteHeadNodeMultiLst(
        int id,
        const CObjectData2D & objectData,
        int nodeId = -1,
        int parentId = -1,
        int spriteId = defs_SPRITE_DEFAULT_ID );

    CSpriteHeadNodeMultiLst(
        int id,
        const CObjectData3D & objectData,
        int nodeId = -1,
        int parentId = -1,
        int spriteId = defs_SPRITE_DEFAULT_ID );

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
    
    // Reset the iterators
    void resetIterators() override;
    
    // Get the unique head node id number
    int getId() const override;
    
private:
    
    // Update the nodes recursive function
    void transform( iNode * pNode );
    
    // Record command buffer recursive function
    void recordCommandBuffer( iNode * pNode, uint32_t index, VkCommandBuffer cmdBuffer, const CMatrix & viewProj );
    
private:
    
    // List of all nodes.
    std::vector<CSpriteNodeMultiLst *> m_nodeVec;
    
    // head node id
    int m_id;
};

#endif
