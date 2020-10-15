
/************************************************************************
*    FILE NAME:       uicontrolleafnode.cpp
*
*    DESCRIPTION:     UI Control node class for rendering a UIControl
*                     Node class for handling a UIControl with no children
*                     to keep the overhead low
************************************************************************/

// Physical component dependency
#include <node/uicontrolleafnode.h>

// Game lib dependencies
#include <gui/uicontrol.h>
#include <node/nodedata.h>
#include <utilities/genfunc.h>

/************************************************************************
*    DESC:  Constructor / Destructor
************************************************************************/
CUIControlLeafNode::CUIControlLeafNode( std::unique_ptr<CUIControl> upControl, const CNodeData & rNodeData ) :
    iNode( rNodeData.getNodeId(), rNodeData.getParentNodeId() )
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

CUIControlLeafNode::~CUIControlLeafNode()
{}

/***************************************************************************
*    DESC:  Update the sprite.
****************************************************************************/
void CUIControlLeafNode::update()
{
    m_upControl->update();
}

/***************************************************************************
*    DESC:  Transform the sprite
****************************************************************************/
void CUIControlLeafNode::transform()
{
    m_upControl->transform();
}

void CUIControlLeafNode::transform( const CObject & object )
{
    m_upControl->transform( object );
}

/***************************************************************************
*    DESC:  Record the command buffer vector in the device
*           for all the sprite objects that are to be rendered
****************************************************************************/
void CUIControlLeafNode::recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer, const CCamera & camera )
{
    m_upControl->recordCommandBuffer( index, cmdBuffer, camera );
}

/************************************************************************
*    DESC:  Get the sprite
************************************************************************/
CUIControl * CUIControlLeafNode::getControl()
{
    return m_upControl.get();
}

/************************************************************************
*    DESC:  Get the object
************************************************************************/
CObject * CUIControlLeafNode::getObject()
{
    return static_cast<CObject *>(m_upControl.get());
}
