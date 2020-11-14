
/************************************************************************
*    FILE NAME:       nodedata.h
*
*    DESCRIPTION:     Node Data Class
************************************************************************/

#pragma once

// Physical component dependency
#include <sprite/spritedata.h>

// Game lib dependencies
#include <gui/uidefs.h>

class CNodeData : public CSpriteData
{
public:

    // Constructor
    CNodeData(
        const XMLNode & node,
        const std::string & nodeName,
        int nodeId = defs_DEFAULT_NODE_ID,
        int parenNodetId = defs_DEFAULT_NODE_ID,
        const std::string & defGroup = std::string(),
        const std::string & defObjName = std::string(),
        int userId = defs_DEFAULT_ID );

    // Constructor for dynamic node data generation for sprite creation
    CNodeData( 
        const std::string & group,
        const std::string & objName );
    
    // Destructor
    virtual ~CNodeData();
    
    // Get the node Name
    const std::string & getNodeName() const;
    
    // Get the node id
    uint8_t getNodeId() const;
    
    // Get the parent node id
    uint8_t getParentNodeId() const;

    // Get the user id
    int getUserId() const;
    
    // Get the node type
    ENodeType getNodeType() const;

    // Get the control type
    EControlType getControlType() const;

    // Does this node have children?
    bool hasChildrenNodes() const;

private:
    
    // node name
    const std::string m_nodeName;

    // Node Id
    const uint8_t m_nodeId;
    
    // Parent Id
    const uint8_t m_parenNodetId;

    // User id
    int16_t m_userId;
    
    // Node type
    ENodeType m_nodeType;

    // Control type of node
    EControlType m_controlType;

    // Does this node have children nodes?
    bool m_hasChildrenNodes;
};
