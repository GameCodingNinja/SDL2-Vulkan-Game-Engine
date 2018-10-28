
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
    const std::string & nodeName,
    int nodeId,
    int parenNodetId,
    const std::string & defGroup,
    const std::string & defObjName,
    const std::string & defAIName,
    int defId ) :
        CSpriteData( node.getChildNode(), defGroup, defObjName, defAIName, defId ),
        m_nodeName(nodeName),
        m_nodeId(nodeId),
        m_parenNodetId(parenNodetId),
        m_nodeType(NDefs::ENT_NULL)
{
    // Get the node type
    const std::string nodeType( node.getAttribute( "type" ) );
    if( nodeType == "spriteNode" )
        m_nodeType = NDefs::ENT_SPRITE;
    
    else if( nodeType == "spriteNodeMultiList" )
        m_nodeType = NDefs::ENT_SPRITE_MULTI_LIST;
}

CNodeData::CNodeData( const CNodeData & data ) :
    CSpriteData( data ),
    m_nodeName(data.m_nodeName),
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
*    DESC:  Get the node Name
************************************************************************/
const std::string & CNodeData::getNodeName() const
{
    return m_nodeName;
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
