
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
    std::string defGroup, defObjName, defAIName, nodeName;

    // Check for any defaults
    if( node.isAttributeSet( "defaultGroup" ) )
        defGroup = node.getAttribute( "defaultGroup" );

    if( node.isAttributeSet( "defaultObjectName" ) )
        defObjName = node.getAttribute( "defaultObjectName" );

    if( node.isAttributeSet( "defaultAIName" ) )
        defAIName = node.getAttribute( "defaultAIName" );
    
    if( node.isAttributeSet( "name" ) )
        nodeName = node.getAttribute( "name" );
    
    int idCounter = 0;

    // Load the node data into the vector
    m_dataVec.emplace_back( node, nodeName, idCounter++, -1, defGroup, defObjName, defAIName );
    
    // Call the recursive function to load the children
    loadNode( node, m_dataVec.back(), idCounter, defGroup, defObjName, defAIName );
}

CNodeDataList::CNodeDataList(
    const XMLNode & node,
    const std::string & _defGroup,
    const std::string & _defObjName,
    const std::string & _defAIName,
    int defId )
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
    
    int idCounter = 0;

    // Load the node data into the vector
    m_dataVec.emplace_back( node, nodeName, idCounter++, -1, defGroup, defObjName, defAIName, defId );
    
    // Call the recursive function to load the children
    loadNode( node, m_dataVec.back(), idCounter, defGroup, defObjName, defAIName, defId );
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
    const std::string & defAIName,
    int defId )
{
    for( int i = 0; i < xmlNode.nChildNode("node"); ++i )
    {
        const XMLNode & childXMLNode = xmlNode.getChildNode("node", i);

        m_dataVec.emplace_back( childXMLNode, "", idCounter++, nodeData.getNodeId(), defGroup, defObjName, defAIName, defId );

        loadNode( childXMLNode, m_dataVec.back(), idCounter, defGroup, defObjName, defAIName, defId );
    }
}


/************************************************************************
*    DESC:  Get the node data
************************************************************************/
const std::vector<CNodeData> & CNodeDataList::getData() const
{
    return m_dataVec;
}
