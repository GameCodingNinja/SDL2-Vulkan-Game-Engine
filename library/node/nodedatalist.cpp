
/************************************************************************
*    FILE NAME:       nodedatalist.cpp
*
*    DESCRIPTION:     Based on how the XML is written, creates a node
*                     list for easy Parent/child node creation
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
*    DESC:  Constructor / Destructor
************************************************************************/
CNodeDataList::CNodeDataList(
    const XMLNode & xmlNode,
    const std::string & _defGroup,
    const std::string & _defObjName,
    int userId ) :
        m_idCounter(defs_DEFAULT_NODE_ID)
{
    std::string defGroup(_defGroup), defObjName(_defObjName), nodeName;

    // Check for any defaults
    if( xmlNode.isAttributeSet( "defaultGroup" ) )
        defGroup = xmlNode.getAttribute( "defaultGroup" );

    if( xmlNode.isAttributeSet( "defaultObjectName" ) )
        defObjName = xmlNode.getAttribute( "defaultObjectName" );
    
    if( xmlNode.isAttributeSet( "defaultId" ) )
        userId = std::atoi(xmlNode.getAttribute( "defaultId" ));
    
    if( xmlNode.isAttributeSet( "name" ) )
        nodeName = xmlNode.getAttribute( "name" );

    // Get the user id number
    if( xmlNode.isAttributeSet( "id" ) )
        userId = std::atoi(xmlNode.getAttribute( "id" ));

    // Make sure user id doesn't exceed the value of a 16 bit integer
    if( userId < INT16_MIN || userId > INT16_MAX )
        throw NExcept::CCriticalException("Node Data List Error!",
                boost::str( boost::format("Node user id exceeds 16 bit range (%s, %d).\n\n%s\nLine: %s")
                    % nodeName % userId % __FUNCTION__ % __LINE__ ));

    // Make sure user id doesn't exceed the value of a byte
    if( m_idCounter > UINT8_MAX )
        throw NExcept::CCriticalException("Node Data List Error!",
                boost::str( boost::format("Node internal id exceeds 8 bit range (%s, %d).\n\n%s\nLine: %s")
                    % nodeName % userId % __FUNCTION__ % __LINE__ ));

    // Load the node data into the vector
    m_dataVec.emplace_back( xmlNode, nodeName, m_idCounter++, defs_DEFAULT_NODE_ID, defGroup, defObjName, userId );
    
    // Call the recursive function to load the children
    loadNode( xmlNode, m_dataVec.back(), defGroup, defObjName, userId );
}

/************************************************************************
*    DESC:  Constructor for dynamic node data generation for sprite creation
************************************************************************/
CNodeDataList::CNodeDataList(
    const std::string & group,
    const std::string & objName )
{
    m_dataVec.emplace_back( group, objName );
}

CNodeDataList::~CNodeDataList()
{}

/************************************************************************
*    DESC:  Load the node data recursively
************************************************************************/
void CNodeDataList::loadNode(
    const XMLNode & xmlNode,
    CNodeData & nodeData,
    const std::string & defGroup,
    const std::string & defObjName,
    int userId )
{
    for( int i = 0; i < xmlNode.nChildNode("node"); ++i )
    {
        const XMLNode & childXMLNode = xmlNode.getChildNode("node", i);
        
        std::string nodeName;
        if( childXMLNode.isAttributeSet( "name" ) )
            nodeName = childXMLNode.getAttribute( "name" );

        m_dataVec.emplace_back( childXMLNode, nodeName, m_idCounter++, nodeData.getNodeId(), defGroup, defObjName, userId );

        // Try to recursively load more children
        loadNode( childXMLNode, m_dataVec.back(), defGroup, defObjName, userId );
    }
}

/************************************************************************
*    DESC:  Get the node data
************************************************************************/
const std::vector<CNodeData> & CNodeDataList::getData() const
{
    return m_dataVec;
}
