
/************************************************************************
*    FILE NAME:       spritenodemultilist.h
*
*    DESCRIPTION:     Sprite node multi link list class
************************************************************************/

#ifndef __sprite_node_multi_list_h__
#define __sprite_node_multi_list_h__

// Physical component dependency
#include <node/nodemultilist.h>

// Game lib dependencies
#include <sprite/sprite.h>

// Forward declaration(s)
class CObjectData2D;
class CObjectData3D;

class CSpriteNodeMultiLst : public CNodeMultiLst
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
    
    // Update the nodes
    void update() override;
    
    // Transform the nodes
    void transform() override;
    void transform( const CMatrix & matrix, bool tranformWorldPos ) override;
    
    // Record the command buffer vector in the device
    // for all the sprite objects that are to be rendered
    void recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer, const CCamera & camera ) override;
    
    // Get the sprite
    CSprite * getSprite() override;
    
    // Get the sprite id number
    int getId() const override;
    
private:
    
    // Node data
    CSprite m_sprite;
};

#endif
