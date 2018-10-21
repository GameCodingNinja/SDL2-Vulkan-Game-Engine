
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

    // Load the node data into the vector
    for( int i = 0; i < node.nChildNode(); ++i )
        m_dataVec.emplace_back( node.getChildNode(i), defGroup, defObjName, defAIName );
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CNodeDataList::~CNodeDataList()
{
}


/************************************************************************
*    DESC:  Get the node data
************************************************************************/
const std::vector<CNodeData> & CNodeDataList::getData() const
{
    return m_dataVec;
}
