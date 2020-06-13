
/************************************************************************
*    FILE NAME:       nodedata.h
*
*    DESCRIPTION:     Node Data Class
************************************************************************/

#ifndef __node_data_h__
#define __node_data_h__

// Physical component dependency
#include <sprite/spritedata.h>

// Game lib dependencies
#include <gui/uicontroldefs.h>

class CNodeData : public CSpriteData
{
public:

    // Constructor
    CNodeData(
        const XMLNode & node,
        const std::string & nodeName,
        int nodeId = defs_DEFAULT_ID,
        int parenNodetId = defs_DEFAULT_ID,
        const std::string & defGroup = std::string(),
        const std::string & defObjName = std::string(),
        int defId = defs_DEFAULT_ID );

    // Constructor for dynamic node data generation for sprite creation
    CNodeData( 
        const std::string & group,
        const std::string & objName );
    
    // Destructor
    virtual ~CNodeData(){};
    
    // Get the node Name
    const std::string & getNodeName() const;
    
    // Get the node id
    int getNodeId() const;
    
    // Get the parent node id
    int getParentNodeId() const;
    
    // Get the node type
    NDefs::ENodeType getNodeType() const;

    // Get the control type
    NUIControlDefs::EControlType getControlType() const;

    // Does this node have children?
    bool hasChildrenNodes() const;

private:
    
    // node name
    const std::string m_nodeName;

    // Node Id
    const int m_nodeId;
    
    // Parent Id
    const int m_parenNodetId;
    
    // Node type
    NDefs::ENodeType m_nodeType;

    // Control type of node
    NUIControlDefs::EControlType m_controlType;

    // Does this node have children nodes?
    bool m_hasChildrenNodes;
};

#endif
