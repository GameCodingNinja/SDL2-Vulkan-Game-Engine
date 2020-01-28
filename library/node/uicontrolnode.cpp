
/************************************************************************
*    FILE NAME:       uicontrolnode.cpp
*
*    DESCRIPTION:     UI Control node class for rendering a ui control
************************************************************************/

// Physical component dependency
#include <node/uicontrolnode.h>

// Game lib dependencies
#include <gui/uicontrol.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CUIControlNode::CUIControlNode(
    std::unique_ptr<CUIControl> upControl,
    int nodeId,
    int parentId ) :
        iNode( nodeId, parentId )
{
    m_upControl = std::move(upControl);
    m_type = NDefs::ENT_UI_CONTROL;
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CUIControlNode::~CUIControlNode()
{
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

// Used to transform object on a sector
void CUIControlNode::transform( const CObject2D & object )
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
