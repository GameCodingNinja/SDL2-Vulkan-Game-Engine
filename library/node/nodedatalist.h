
/************************************************************************
*    FILE NAME:       nodedatalist.h
*
*    DESCRIPTION:     Node Data list Class
************************************************************************/

#ifndef __node_data_list_h__
#define __node_data_list_h__

// Game lib dependencies
#include <common/defs.h>

// Standard lib dependencies
#include <vector>
#include <string>

// Forward declaration(s)
struct XMLNode;
class CNodeData;

class CNodeDataList
{
public:

    // Constructor
    CNodeDataList( const XMLNode & node );
    CNodeDataList(
        const XMLNode & node,
        const std::string & _defGroup,
        const std::string & _defObjName,
        const std::string & _defAIName,
        int defId = defs_SPRITE_DEFAULT_ID );
    
    // Destructor
    virtual ~CNodeDataList();
    
    // Get the node data
    const std::vector<CNodeData> & getData() const;

private:
    
    // Load the node data recursively
    void loadNode(
        const XMLNode & xmlNode,
        CNodeData & nodeData,
        int & idCounter,
        const std::string & defGroup,
        const std::string & defObjName,
        const std::string & defAIName,
        int defId = defs_SPRITE_DEFAULT_ID );
    
private:
    
    // Map of the node data
    std::vector<CNodeData> m_dataVec;
};

#endif

