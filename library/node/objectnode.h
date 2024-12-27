
/************************************************************************
*    FILE NAME:       objectnode.h
*
*    DESCRIPTION:     Object node that allows for children
************************************************************************/

#pragma once

// Physical component dependency
#include <node/rendernode.h>
#include <common/object.h>

// Forward declaration(s)
class CNodeData;

// Make use of multiple inheritance so that the object can return
// a pointer to the node without having to keep a pointer to it
class CObjectNode : public CRenderNode, public CObject
{
public:
    
    // Constructor
    CObjectNode( const CNodeData & rNodeData );

    // Destructor
    virtual ~CObjectNode();

    // Only called after node creation with all it's children
    void init();
    
    // Transform the nodes
    void transform() override;
    void transform( const CObject & object ) override;
    
    // Get the sprite
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
