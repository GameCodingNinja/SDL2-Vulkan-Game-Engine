
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
class iObjectData;

class CSpriteNodeMultiLst : public CNodeMultiLst
{
public:
    
    // Constructor
    CSpriteNodeMultiLst(
        const iObjectData & objectData,
        int spriteId = defs_DEFAULT_ID,
        int nodeId = defs_DEFAULT_ID,
        int parentId = defs_DEFAULT_ID );

    // Destructor
    virtual ~CSpriteNodeMultiLst();
    
    // Update the nodes
    void update() override;
    
    // Transform the nodes
    void transform() override;
    void transform( const CObject2D & object ) override;
    
    // Record the command buffer vector in the device
    // for all the sprite objects that are to be rendered
    void recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer, const CCamera & camera ) override;

    // Get the sprite id number
    int getId() const override;
    
    // Get the sprite
    CSprite * getSprite() override;

    // Get the object
    CObject2D * getObject() override;
    
private:
    
    // Node data
    CSprite m_sprite;
};

#endif
