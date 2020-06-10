
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
#include <common/objecttransform.h>

// Forward declaration(s)
class CNodeData;

class CObjectNodeMultiLst : public CNodeMultiLst
{
public:
    
    // Constructor
    CObjectNodeMultiLst( const CNodeData & rNodeData );

    // Destructor
    virtual ~CObjectNodeMultiLst();
    
    // Transform the nodes
    void transform() override;
    void transform( const CObjectTransform & object ) override;
    
    // Get the sprite
    CObjectTransform * getObject() override;
    
    // Get the sprite id number
    int getId() const override;
    
private:
    
    // Node data
    CObjectTransform m_object;
};

#endif
