
/************************************************************************
*    FILE NAME:       spritenode.h
*
*    DESCRIPTION:     Sprite node class
************************************************************************/

#ifndef __sprite_node_h__
#define __sprite_node_h__

// Physical component dependency
#include <common/inode.h>

// Game lib dependencies
#include <sprite/sprite.h>

// Forward declaration(s)
class CObjectData2D;
class CObjectData3D;

class CSpriteNode : public iNode<CSprite>
{
public:

    // Constructor
    CSpriteNode(
        int nodeId,
        int parentId,
        const CObjectData2D & objectData,
        int spriteId = defs_SPRITE_DEFAULT_ID ) :
            iNode<CSprite>(nodeId, parentId),
            m_sprite(objectData, spriteId)
    {
        m_type = NDefs::ENT_SPRITE;
    }

    CSpriteNode(
        int nodeId,
        int parentId,
        const CObjectData3D & objectData,
        int spriteId = defs_SPRITE_DEFAULT_ID ) :
            iNode<CSprite>(nodeId, parentId),
            m_sprite(objectData, spriteId)
    {
        m_type = NDefs::ENT_SPRITE;
    }

    virtual ~CSpriteNode(){}

    // Get the data
    CSprite & getData() override
    {
        return m_sprite;
    }

private:

    // Node data
    CSprite m_sprite;
};

#endif
