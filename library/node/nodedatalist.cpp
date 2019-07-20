
/************************************************************************
*    FILE NAME:       nodedatalist.cpp
*
*    DESCRIPTION:     Based on how the XML is written, creates a node
*                     list so that the Parent/child nodes can be created
************************************************************************/

// Physical component dependency
#include <node/nodedatalist.h>

// Game lib dependencies
#include <node/nodedata.h>
#include <utilities/xmlParser.h>
#include <utilities/exceptionhandling.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CNodeDataList::CNodeDataList(
    const XMLNode & node,
    const std::string & _defGroup,
    const std::string & _defObjName,
    const std::string & _defAIName,
    int defId ) :
        m_idCounter(defs_DEFAULT_ID)
{
    std::string defGroup(_defGroup), defObjName(_defObjName), defAIName(_defAIName), nodeName;

    // Check for any defaults
    if( node.isAttributeSet( "defaultGroup" ) )
        defGroup = node.getAttribute( "defaultGroup" );

    if( node.isAttributeSet( "defaultObjectName" ) )
        defObjName = node.getAttribute( "defaultObjectName" );

    if( node.isAttributeSet( "defaultAIName" ) )
        defAIName = node.getAttribute( "defaultAIName" );
    
    if( node.isAttributeSet( "defaultId" ) )
        defId = std::atoi(node.getAttribute( "defaultId" ));
    
    if( node.isAttributeSet( "name" ) )
        nodeName = node.getAttribute( "name" );

    // Load the node data into the vector
    m_dataVec.emplace_back( node, nodeName, m_idCounter++, defs_DEFAULT_ID, defGroup, defObjName, defAIName, defId );
    
    // Call the recursive function to load the children
    loadNode( node, m_dataVec.back(), defGroup, defObjName, defAIName, defId );
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CNodeDataList::~CNodeDataList()
{
}


/************************************************************************
*    DESC:  Load the node data recursively
************************************************************************/
void CNodeDataList::loadNode(
    const XMLNode & xmlNode,
    CNodeData & nodeData,
    const std::string & defGroup,
    const std::string & defObjName,
    const std::string & defAIName,
    int defId )
{
    for( int i = 0; i < xmlNode.nChildNode("node"); ++i )
    {
        const XMLNode & childXMLNode = xmlNode.getChildNode("node", i);
        
        std::string nodeName;
        if( childXMLNode.isAttributeSet( "name" ) )
            nodeName = childXMLNode.getAttribute( "name" );

        m_dataVec.emplace_back( childXMLNode, nodeName, m_idCounter++, nodeData.getNodeId(), defGroup, defObjName, defAIName, defId );

        // Try to recursively load more children
        loadNode( childXMLNode, m_dataVec.back(), defGroup, defObjName, defAIName, defId );
    }
}


/************************************************************************
*    DESC:  Get the node data
************************************************************************/
const std::vector<CNodeData> & CNodeDataList::getData() const
{
    return m_dataVec;
}
