
/************************************************************************
*    FILE NAME:       spriteheadnodemultilist.h
*
*    DESCRIPTION:     Sprite head node multi link list class
************************************************************************/

#ifndef __sprite_node_h__
#define __sprite_node_h__

// Physical component dependency
#include <node/inode.h>

// Game lib dependencies
#include <sprite/sprite.h>

// Forward declaration(s)
class CMatrix;
class CObjectData2D;
class CObjectData3D;

class CSpriteNode : public iNode
{
public:

    // Constructor
    CSpriteNode(
        const CObjectData2D & objectData,
        int spriteId = defs_SPRITE_DEFAULT_ID );

    CSpriteNode(
        const CObjectData3D & objectData,
        int spriteId = defs_SPRITE_DEFAULT_ID );

    // Destructor
    virtual ~CSpriteNode();
    
    // Do any init
    void init() override;

    // Update the nodes
    void update() override;

    // Transform the nodes
    void transform() override;
    void transform( const CMatrix & matrix, bool tranformWorldPos = false ) override;

    // Record the command buffer vector in the device
    // for all the sprite objects that are to be rendered
    void recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer, const CMatrix & viewProj ) override;
    void recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer, const CMatrix & rotMatrix, const CMatrix & viewProj ) override;

    // Get the unique head node id number
    int getId() const override;

    // Get the sprite
    CSprite * getSprite() override;

private:

    // Update the nodes recursive function
    void transform( iNode * pNode );

    // Record command buffer recursive function
    void recordCommandBuffer( iNode * pNode, uint32_t index, VkCommandBuffer cmdBuffer, const CMatrix & viewProj );
    void recordCommandBuffer( iNode * pNode, uint32_t index, VkCommandBuffer cmdBuffer, const CMatrix & rotMatrix, const CMatrix & viewProj );

protected:

    // Node data
    CSprite m_sprite;
};

#endif