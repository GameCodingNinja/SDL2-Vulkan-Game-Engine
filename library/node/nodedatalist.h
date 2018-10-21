
/************************************************************************
*    FILE NAME:       nodedatalist.h
*
*    DESCRIPTION:     Node Data list Class
************************************************************************/

#ifndef __node_data_list_h__
#define __node_data_list_h__

// Standard lib dependencies
#include <vector>

// Forward declaration(s)
struct XMLNode;
class CNodeData;

class CNodeDataList
{
public:

    // Constructor
    CNodeDataList( const XMLNode & node );
    
    // Destructor
    virtual ~CNodeDataList();
    
    // Get the node data
    const std::vector<CNodeData> & getData() const;

private:
    
    // Map of the node data
    std::vector<CNodeData> m_dataVec;
};

#endif


