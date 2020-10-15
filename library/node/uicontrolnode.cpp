
/************************************************************************
*    FILE NAME:       uicontrolnode.cpp
*
*    DESCRIPTION:     UI Control node that allows for children
************************************************************************/

// Physical component dependency
#include <node/uicontrolnode.h>

// Game lib dependencies
#include <gui/uicontrol.h>
#include <node/nodedata.h>
#include <utilities/genfunc.h>

/************************************************************************
*    DESC:  Constructor / Destructor
************************************************************************/
CUIControlNode::CUIControlNode( std::unique_ptr<CUIControl> upControl, const CNodeData & rNodeData ) :
    CRenderNode( rNodeData.getNodeId(), rNodeData.getParentNodeId() )
{
    m_upControl = std::move(upControl);
    m_userId = rNodeData.getUserId();
    m_type = ENodeType::UI_CONTROL;

    // Create a CRC16 of the node name
    if( !rNodeData.getNodeName().empty() )
        m_crcUserId = NGenFunc::CalcCRC16( rNodeData.getNodeName() );

    m_upControl->loadFromNode( rNodeData.getXMLNode() );
    m_upControl->init();
}

CUIControlNode::~CUIControlNode()
{}

/***************************************************************************
*    DESC:  Update the control.
*           NOTE: Only gets called if this is the head node
****************************************************************************/
void CUIControlNode::update()
{
    m_upControl->update();

    // Call inherited for recursion of children
    CRenderNode::update();
}

/***************************************************************************
*    DESC:  Transform the control
*           NOTE: Only gets called if this is the head node
****************************************************************************/
void CUIControlNode::transform()
{
    m_upControl->transform();

    // Call inherited for recursion of children
    CRenderNode::transform();
}

void CUIControlNode::transform( const CObject & object )
{
    m_upControl->transform( object );

    // Call inherited for recursion of children
    CRenderNode::transform();
}

/***************************************************************************
*    DESC:  Record the command buffer vector in the device
*           for all the sprite objects that are to be rendered
****************************************************************************/
void CUIControlNode::recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer, const CCamera & camera )
{
    m_upControl->recordCommandBuffer( index, cmdBuffer, camera );

    // Call inherited for recursion of children
    CRenderNode::recordCommandBuffer( index, cmdBuffer, camera );
}

/************************************************************************
*    DESC:  Get the control
************************************************************************/
CUIControl * CUIControlNode::getControl()
{
    return m_upControl.get();
}

/************************************************************************
*    DESC:  Get the object
************************************************************************/
CObject * CUIControlNode::getObject()
{
    return static_cast<CObject *>(m_upControl.get());
}
