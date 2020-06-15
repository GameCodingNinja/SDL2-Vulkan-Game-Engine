
/************************************************************************
*    FILE NAME:       spriteleafnode.h
*
*    DESCRIPTION:     Sprite node class for handling a sprite with
*                     no children to keep the overhead low
************************************************************************/

#ifndef __sprite_leaf_node_h__
#define __sprite_leaf_node_h__

// Physical component dependency
#include <node/inode.h>
#include <sprite/sprite.h>

// Forward declaration(s)
class CMatrix;
class iObjectData;
class CNodeData;

// Make use of multiple inheritance so that the sprite can return
// a pointer to the node without having to keep a pointer to it
class CSpriteLeafNode : public iNode, public CSprite
{
public:

    // Constructor
    CSpriteLeafNode( const CNodeData & rNodeData );

    // Destructor
    virtual ~CSpriteLeafNode(){};

    // Update the nodes
    void update() override;

    // Transform the nodes
    void transform() override;
    // Used to transform object on a sector
    void transform( const CObjectTransform & object ) override;

    // Record the command buffer vector in the device
    // for all the sprite objects that are to be rendered
    void recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer, const CCamera & camera ) override;

    // Get the sprite
    CSprite * getSprite() override;

    // Get the object
    CObjectTransform * getObject() override;

private:

    // Update the nodes recursive function
    void transform( iNode * pNode );

    // Record command buffer recursive function
    void recordCommandBuffer( iNode * pNode, uint32_t index, VkCommandBuffer cmdBuffer, const CMatrix & viewProj );
    void recordCommandBuffer( iNode * pNode, uint32_t index, VkCommandBuffer cmdBuffer, const CMatrix & rotMatrix, const CMatrix & viewProj );
};

#endif
