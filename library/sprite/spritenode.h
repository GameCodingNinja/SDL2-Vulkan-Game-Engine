
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

class CSpriteNode : public iNode
{
public:

    // Constructor
    CSpriteNode(
        int nodeId,
        int parentId,
        const CObjectData2D & objectData,
        int spriteId = defs_SPRITE_DEFAULT_ID ) :
            iNode(nodeId, parentId),
            m_sprite(objectData, spriteId)
    {
        m_type = NDefs::ENT_SPRITE;
    }

    CSpriteNode(
        int nodeId,
        int parentId,
        const CObjectData3D & objectData,
        int spriteId = defs_SPRITE_DEFAULT_ID ) :
            iNode(nodeId, parentId),
            m_sprite(objectData, spriteId)
    {
        m_type = NDefs::ENT_SPRITE;
    }

    // Destructor
    virtual ~CSpriteNode(){}

    // Node data
    CSprite m_sprite;
};

class CSpriteHeadNode : public CSpriteNode
{
public:
    
    // Constructor
    CSpriteHeadNode(
        int nodeId,
        int parentId,
        const CObjectData2D & objectData,
        int spriteId = defs_SPRITE_DEFAULT_ID );

    CSpriteHeadNode(
        int nodeId,
        int parentId,
        const CObjectData3D & objectData,
        int spriteId = defs_SPRITE_DEFAULT_ID );

    // Destructor
    virtual ~CSpriteHeadNode();
    
    // Add a node
    bool addNode( iNode * pNode ) override;
    
private:
    
    // Reset the iterators
    void resetIterators();
    
private:
    
    // List of all nodes.
    std::vector<iNode *> m_allNodesVec;
};

#endif
