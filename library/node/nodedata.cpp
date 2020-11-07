
/************************************************************************
*    FILE NAME:       nodedata.cpp
*
*    DESCRIPTION:     Node Data Class
************************************************************************/

// Physical component dependency
#include <node/nodedata.h>

// Game lib dependencies
#include <utilities/xmlParser.h>
#include <utilities/exceptionhandling.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Standard lib dependencies
#include <cstring>

/************************************************************************
*    DESC:  Constructor / Destructor
************************************************************************/
CNodeData::CNodeData( 
    const XMLNode & node,
    const std::string & nodeName,
    int nodeId,
    int parenNodetId,
    const std::string & defGroup,
    const std::string & defObjName,
    int userId ) :
        CSpriteData( node.getChildNode(), nodeName, defGroup, defObjName ),
        m_nodeName(nodeName),
        m_nodeId(nodeId),
        m_parenNodetId(parenNodetId),
        m_userId(userId),
        m_nodeType(ENodeType::_NULL_),
        m_controlType(EControlType::_NULL),
        m_hasChildrenNodes(false)
{
    // Does this node have children nodes?
    if( node.nChildNode("node") > 0 )
        m_hasChildrenNodes = true;

    // Get the node type
    for( int i = 0; i < node.nChildNode(); ++i )
    {
        const XMLNode childNode = node.getChildNode( i );

        if( std::strcmp( childNode.getName(), "object" ) == 0 )
        {
            m_nodeType = ENodeType::OBJECT;
            break;
        }
        else if( std::strcmp( childNode.getName(), "sprite" ) == 0 )
        {
            m_nodeType = ENodeType::SPRITE;
            break;
        }
        else if( std::strcmp( childNode.getName(), "uiMeter" ) == 0 )
        {
            m_nodeType = ENodeType::UI_CONTROL;
            m_controlType = EControlType::METER;
            break;
        }
        else if( std::strcmp( childNode.getName(), "uiProgressBar" ) == 0 )
        {
            m_nodeType = ENodeType::UI_CONTROL;
            m_controlType = EControlType::PROGRESS_BAR;
            break;
        }
    }

    if( m_nodeType == ENodeType::_NULL_ )
        throw NExcept::CCriticalException("Node Load Error!",
                boost::str( boost::format("Node type not defined (%s).\n\n%s\nLine: %s")
                    % nodeName % __FUNCTION__ % __LINE__ ));
}

/************************************************************************
*    DESC:  Constructor for dynamic node data generation for sprite creation
************************************************************************/
CNodeData::CNodeData( 
    const std::string & group,
    const std::string & objName ) :
        CSpriteData( group, objName ),
        m_nodeName(objName),
        m_nodeId(defs_DEFAULT_NODE_ID),
        m_parenNodetId(defs_DEFAULT_NODE_ID),
        m_userId(defs_DEFAULT_ID),
        m_nodeType(ENodeType::SPRITE),
        m_controlType(EControlType::_NULL),
        m_hasChildrenNodes(false)
{
}

CNodeData::~CNodeData()
{}

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
uint8_t CNodeData::getNodeId() const
{
    return m_nodeId;
}

/************************************************************************
*    DESC:  Get the parent node id
************************************************************************/
uint8_t CNodeData::getParentNodeId() const
{
    return m_parenNodetId;
}

/************************************************************************
*    DESC:  Get the user id
************************************************************************/
int CNodeData::getUserId() const
{
    return m_userId;
}

/************************************************************************
*    DESC:  Get the node type
************************************************************************/
ENodeType CNodeData::getNodeType() const
{
    return m_nodeType;
}

/************************************************************************
*    DESC:  Get the control type
************************************************************************/
EControlType CNodeData::getControlType() const
{
    return m_controlType;
}

/************************************************************************
*    DESC:  Does this node have children?
************************************************************************/
bool CNodeData::hasChildrenNodes() const
{
    return m_hasChildrenNodes;
}
