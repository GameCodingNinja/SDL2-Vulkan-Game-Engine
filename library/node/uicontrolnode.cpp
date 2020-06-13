
/************************************************************************
*    FILE NAME:       uicontrolnode.cpp
*
*    DESCRIPTION:     UI Control node class for rendering a ui control
************************************************************************/

// Physical component dependency
#include <node/uicontrolnode.h>

// Game lib dependencies
#include <gui/uicontrol.h>
#include <node/nodedata.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CUIControlNode::CUIControlNode( std::unique_ptr<CUIControl> upControl, const CNodeData & rNodeData ) :
    iNode( rNodeData.getNodeId(), rNodeData.getParentNodeId() )
{
    m_upControl = std::move(upControl);
    m_id = rNodeData.getId();
    m_type = NDefs::ENT_UI_CONTROL;

    m_upControl->loadFromNode( rNodeData.getXMLNode() );
    m_upControl->init();
}

/***************************************************************************
*    DESC:  Update the sprite.
****************************************************************************/
void CUIControlNode::update()
{
    m_upControl->update();
}

/***************************************************************************
*    DESC:  Transform the sprite
****************************************************************************/
void CUIControlNode::transform()
{
    m_upControl->transform();
}

void CUIControlNode::transform( const CObjectTransform & object )
{
    m_upControl->transform( object );
}

/***************************************************************************
*    DESC:  Record the command buffer vector in the device
*           for all the sprite objects that are to be rendered
****************************************************************************/
void CUIControlNode::recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer, const CCamera & camera )
{
    m_upControl->recordCommandBuffer( index, cmdBuffer, camera );
}

/************************************************************************
*    DESC:  Get the sprite
************************************************************************/
CUIControl * CUIControlNode::getControl()
{
    return m_upControl.get();
}

/************************************************************************
*    DESC:  Get the object
************************************************************************/
CObjectTransform * CUIControlNode::getObject()
{
    return static_cast<CObjectTransform *>(m_upControl.get());
}
