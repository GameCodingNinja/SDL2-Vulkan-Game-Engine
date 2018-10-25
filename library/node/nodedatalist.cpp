
/************************************************************************
*    FILE NAME:       nodedatalist.cpp
*
*    DESCRIPTION:     Node Data List Class
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
CNodeDataList::CNodeDataList( const XMLNode & node )
{
    std::string defGroup, defObjName, defAIName;

    // Check for any defaults
    if( node.isAttributeSet( "defaultGroup" ) )
        defGroup = node.getAttribute( "defaultGroup" );

    if( node.isAttributeSet( "defaultObjectName" ) )
        defObjName = node.getAttribute( "defaultObjectName" );

    if( node.isAttributeSet( "defaultAIName" ) )
        defAIName = node.getAttribute( "defaultAIName" );
    
    int idCounter = 0;

    // Load the node data into the vector
    m_dataVec.emplace_back( node, idCounter++, -1, defGroup, defObjName, defAIName );
    
    // Call the recursive function to load the children
    loadNode( node, m_dataVec.back(), idCounter, defGroup, defObjName, defAIName );
    
    defAIName = "";
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
    int & idCounter,
    const std::string & defGroup,
    const std::string & defObjName,
    const std::string & defAIName )
{
    for( int i = 0; i < xmlNode.nChildNode("node"); ++i )
    {
        const XMLNode & childXMLNode = xmlNode.getChildNode("node", i);

        m_dataVec.emplace_back( childXMLNode, idCounter++, nodeData.getNodeId(), defGroup, defObjName, defAIName );

        loadNode( childXMLNode, m_dataVec.back(), idCounter, defGroup, defObjName, defAIName );
    }
}


/************************************************************************
*    DESC:  Get the node data
************************************************************************/
const std::vector<CNodeData> & CNodeDataList::getData() const
{
    return m_dataVec;
}
