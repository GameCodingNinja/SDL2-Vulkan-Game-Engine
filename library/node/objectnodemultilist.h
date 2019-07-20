
/************************************************************************
*    FILE NAME:       objectnodemultilist.h
*
*    DESCRIPTION:     Object node multi link list class
************************************************************************/

#ifndef __object_node_multi_list_h__
#define __object_node_multi_list_h__

// Physical component dependency
#include <node/nodemultilist.h>

// Game lib dependencies
#include <2d/object2d.h>

class CObjectNodeMultiLst : public CNodeMultiLst
{
public:
    
    // Constructor
    CObjectNodeMultiLst(
        int objectId = defs_DEFAULT_ID,
        int nodeId = defs_DEFAULT_ID,
        int parentId = defs_DEFAULT_ID );

    // Destructor
    virtual ~CObjectNodeMultiLst();
    
    // Transform the nodes
    void transform() override;
    void transform( const CObject2D & object ) override;
    
    // Get the sprite
    CObject2D * getObject() override;
    
    // Get the sprite id number
    int getId() const override;
    
private:
    
    // Node data
    CObject2D m_object;
    
    // Object Id of this node
    int m_objectId;
};

#endif
