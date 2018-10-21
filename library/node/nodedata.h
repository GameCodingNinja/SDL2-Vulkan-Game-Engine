
/************************************************************************
*    FILE NAME:       nodedata.h
*
*    DESCRIPTION:     Node Data Class
************************************************************************/

#ifndef __node_data_h__
#define __node_data_h__

// Physical component dependency
#include <sprite/spritedata.h>

class CNodeData : public CSpriteData
{
public:

    // Constructor
    CNodeData(
        const XMLNode & node,
        const std::string & defGroup = std::string(),
        const std::string & defObjName = std::string(),
        const std::string & defAIName = std::string(),
        int defId = defs_SPRITE_DEFAULT_ID );
    
    CNodeData( const CNodeData & data );
    
    // Destructor
    virtual ~CNodeData();
    
    // Get the node id
    int getNodeId() const;
    
    // Get the parent node id
    int getParentNodeId() const;
    
    // Get the node type
    NDefs::ENodeType getNodeType() const;

private:

    // Node Id
    int m_nodeId;
    
    // Parent Id
    int m_parenNodetId;
    
    // Node type
    NDefs::ENodeType m_nodeType;
};

#endif


