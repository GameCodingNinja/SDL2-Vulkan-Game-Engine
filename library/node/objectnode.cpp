
/************************************************************************
*    FILE NAME:       objectnode.cpp
*
*    DESCRIPTION:     Object node that allows for children
************************************************************************/

// Physical component dependency
#include <node/objectnode.h>

// Game lib dependencies
#include <node/nodedata.h>
#include <utilities/genfunc.h>

/************************************************************************
*    DESC:  Constructor / Destructor
************************************************************************/
CObjectNode::CObjectNode( const CNodeData & rNodeData ) :
    CRenderNode( rNodeData.getNodeId(), rNodeData.getParentNodeId() )
{
    m_userId = rNodeData.getUserId();
    m_type = ENodeType::OBJECT;

    // Create a CRC16 of the node name
    if( !rNodeData.getNodeName().empty() )
        m_crcUserId = NGenFunc::CalcCRC16( rNodeData.getNodeName() );

    // Create a CRC16 of the node name
    if( !rNodeData.getNodeName().empty() )
        m_crcUserId = NGenFunc::CalcCRC16( rNodeData.getNodeName() );

    // Load the transforms from XML node
    CObjectTransform::loadTransFromNode( rNodeData.getXMLNode() );
}

CObjectNode::~CObjectNode()
{}

/***************************************************************************
*    DESC:  Transform the nodes
*           NOTE: Only gets called if this is the head node
****************************************************************************/
void CObjectNode::transform()
{
    CObjectTransform::transform();

    // Call inherited for recursion of children
    CRenderNode::transform();
}

void CObjectNode::transform( const CObjectTransform & object )
{
    CObjectTransform::transform( object );

    // Call inherited for recursion of children
    CRenderNode::transform();
}

/************************************************************************
*    DESC:  Get the object
************************************************************************/
CObjectTransform * CObjectNode::getObject()
{
    return static_cast<CObjectTransform *>(this);
}
