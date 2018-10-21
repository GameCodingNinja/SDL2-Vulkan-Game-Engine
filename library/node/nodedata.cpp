
/************************************************************************
*    FILE NAME:       nodedata.cpp
*
*    DESCRIPTION:     Node Data Class
************************************************************************/

// Physical component dependency
#include <node/nodedata.h>

// Game lib dependencies
#include <utilities/xmlParser.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CNodeData::CNodeData( 
    const XMLNode & node,
    const std::string & defGroup,
    const std::string & defObjName,
    const std::string & defAIName,
    int defId ) :
        CSpriteData( node.getChildNode(), defGroup, defObjName, defAIName, defId ),
        m_nodeId(-1),
        m_parenNodetId(-1),
        m_nodeType(NDefs::ENT_NULL)
{
    // Get the node Id
    if( node.isAttributeSet( "nodeId" ) )
        m_nodeId = std::atoi(node.getAttribute( "nodeId" ));
    
    // Get the parent Id
    if( node.isAttributeSet( "parentId" ) )
        m_parenNodetId = std::atoi(node.getAttribute( "parentId" ));
    
    // Get the node type
    const std::string nodeName( node.getName() );
    if( nodeName == "spriteNode" )
        m_nodeType = NDefs::ENT_SPRITE;
}

CNodeData::CNodeData( const CNodeData & data ) :
    CSpriteData( data ),
    m_nodeId( data.m_nodeId ),
    m_parenNodetId( data.m_parenNodetId ),
    m_nodeType( data.m_nodeType )
{
}


/************************************************************************
*    DESC:  destructor                                                             
************************************************************************/
CNodeData::~CNodeData()
{
}


/************************************************************************
*    DESC:  Get the node id
************************************************************************/
int CNodeData::getNodeId() const
{
    return m_nodeId;
}


/************************************************************************
*    DESC:  Get the parent node id
************************************************************************/
int CNodeData::getParentNodeId() const
{
    return m_parenNodetId;
}


/************************************************************************
*    DESC:  Get the node type
************************************************************************/
NDefs::ENodeType CNodeData::getNodeType() const
{
    return m_nodeType;
}
