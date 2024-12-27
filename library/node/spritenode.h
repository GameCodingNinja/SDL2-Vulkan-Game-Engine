
/************************************************************************
*    FILE NAME:       spritenode.h
*
*    DESCRIPTION:     Sprite node that allows for children
************************************************************************/

#pragma once

// Physical component dependency
#include <node/rendernode.h>
#include <sprite/sprite.h>

// Forward declaration(s)
class iObjectData;
class CNodeData;

// Make use of multiple inheritance so that the sprite can return
// a pointer to the node without having to keep a pointer to it
class CSpriteNode : public CRenderNode, public CSprite
{
public:
    
    // Constructor
    CSpriteNode( const CNodeData & rNodeData );

    // Destructor
    virtual ~CSpriteNode();

    // Only called after node creation with all it's children
    void init();
    
    // Update the nodes
    void update() override;
    
    // Transform the nodes
    void transform() override;
    void transform( const CObject & object ) override;
    
    // Record the command buffer vector in the device
    // for all the sprite objects that are to be rendered
    void recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer, const CCamera & camera ) override;

    // Destroy the physics
    void destroyPhysics() override;
    
    // Get the sprite
    CSprite * getSprite() override;

    // Get the object
    CObject * getObject() override;

    // Get the radius
    float getRadius() override;

    // Get the size
    CSize<float> getSize() override;

private:

    // Calculate the radius
    void calcSize( iNode * pNode, CSize<float> & size );

private:

    // Radius of this object
    float m_radius;

    // Size of this object, meaning the
    // acumulated size of all the children
    CSize<float> m_size;
};
