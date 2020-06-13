
/************************************************************************
*    FILE NAME:       uicontrolnodemultilist.cpp
*
*    DESCRIPTION:     UI Control node class for rendering a ui control
************************************************************************/

// Physical component dependency
#include <node/uicontrolnodemultilist.h>

// Game lib dependencies
#include <gui/uicontrol.h>
#include <node/nodedata.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CUIControlNodeMultiLst::CUIControlNodeMultiLst( std::unique_ptr<CUIControl> upControl, const CNodeData & rNodeData ) :
    CNodeMultiLst( rNodeData.getNodeId(), rNodeData.getParentNodeId() )
{
    m_upControl = std::move(upControl);
    m_id = rNodeData.getId();
    m_type = NDefs::ENT_UI_CONTROL;

    m_upControl->loadFromNode( rNodeData.getXMLNode() );
    m_upControl->init();
}

/***************************************************************************
*    DESC:  Update the control.
*           NOTE: Only gets called if this is the head node
****************************************************************************/
void CUIControlNodeMultiLst::update()
{
    m_upControl->update();

    // Call inherited but it has to be last
    CNodeMultiLst::update();
}

/***************************************************************************
*    DESC:  Transform the control
*           NOTE: Only gets called if this is the head node
****************************************************************************/
void CUIControlNodeMultiLst::transform()
{
    m_upControl->transform();

    // Call inherited but it has to be last
    CNodeMultiLst::transform();
}

void CUIControlNodeMultiLst::transform( const CObjectTransform & object )
{
    m_upControl->transform( object );

    // Call inherited but it has to be last
    CNodeMultiLst::transform();
}

/***************************************************************************
*    DESC:  Record the command buffer vector in the device
*           for all the sprite objects that are to be rendered
****************************************************************************/
void CUIControlNodeMultiLst::recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuffer, const CCamera & camera )
{
    m_upControl->recordCommandBuffer( index, cmdBuffer, camera );

    // Call inherited but it has to be last
    CNodeMultiLst::recordCommandBuffer( index, cmdBuffer, camera );
}

/************************************************************************
*    DESC:  Get the control
************************************************************************/
CUIControl * CUIControlNodeMultiLst::getControl()
{
    return m_upControl.get();
}

/************************************************************************
*    DESC:  Get the object
************************************************************************/
CObjectTransform * CUIControlNodeMultiLst::getObject()
{
    return static_cast<CObjectTransform *>(m_upControl.get());
}
