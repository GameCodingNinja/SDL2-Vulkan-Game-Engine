
/************************************************************************
*    FILE NAME:       spritenode.h
*
*    DESCRIPTION:     Sprite node class for rendering just one sprite
************************************************************************/

#ifndef __sprite_node_h__
#define __sprite_node_h__

// Physical component dependency
#include <node/inode.h>

// Game lib dependencies
#include <sprite/sprite.h>

// Forward declaration(s)
class CMatrix;
class iObjectData;

class CSpriteNode : public iNode
{
public:

    // Constructor
    CSpriteNode(
        const iObjectData & objectData,
        int spriteId = defs_DEFAULT_ID,
        int nodeId = defs_DEFAULT_ID,
        int parentId = defs_DEFAULT_ID );

    // Destructor
    virtual ~CSpriteNode();

    // Update the nodes
    void update() override;

    // Transform the nodes
    void transform() override;
    // Used to transform object on a sector
    void transform( const CObject2D & object ) override;

    // Record the command buffer vector in the device
    // for all the sprite objects that are to be rendered
    void recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer, const CCamera & camera ) override;

    // Get the unique head node id number
    int getId() const override;

    // Get the sprite
    CSprite * getSprite() override;

    // Get the object
    CObject2D * getObject() override;

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
