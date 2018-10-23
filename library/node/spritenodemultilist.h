
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

class CSpriteNodeMultiLst : public CNode
{
public:

    // Constructor
    CSpriteNodeMultiLst(
        const CObjectData2D & objectData,
        int spriteId = defs_SPRITE_DEFAULT_ID,
        int nodeId = -1,
        int parentId = -1 ) :
            CNode(nodeId, parentId),
            m_sprite(objectData, spriteId)
    {
        m_type = NDefs::ENT_SPRITE;
    }

    CSpriteNodeMultiLst(
        const CObjectData3D & objectData,
        int spriteId = defs_SPRITE_DEFAULT_ID,
        int nodeId = -1,
        int parentId = -1 ) :
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
    
    // Get the unique head node id number
    int getId() const override
    { return m_sprite.getId(); }

protected:
    
    // Node data
    CSprite m_sprite;
};

#endif
