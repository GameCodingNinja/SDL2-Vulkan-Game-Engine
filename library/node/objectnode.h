
/************************************************************************
*    FILE NAME:       objectnode.h
*
*    DESCRIPTION:     Object node that allows for children
************************************************************************/

#ifndef __object_node_h__
#define __object_node_h__

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
    
    // Transform the nodes
    void transform() override;
    void transform( const CObject & object ) override;
    
    // Get the sprite
    CObject * getObject() override;
};

#endif
