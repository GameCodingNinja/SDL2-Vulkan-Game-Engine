
/************************************************************************
*    FILE NAME:       uicheckbox.cpp
*
*    DESCRIPTION:     Class for user interface check box buttons
************************************************************************/

// Physical component dependency
#include <gui/uicheckbox.h>

// Game lib dependencies
#include <sprite/sprite.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CUICheckBox::CUICheckBox( const std::string & group ) :
    CUIControl( group ),
    m_toggleState(false)
{
    m_type = NUIControl::ECT_CHECK_BOX;
}


/************************************************************************
*    DESC:  destructor                                                             
************************************************************************/
CUICheckBox::~CUICheckBox()
{
}


/************************************************************************
*    DESC:  Set the Select state of this control
*  
*    param: EControlState value - control state
************************************************************************/
void CUICheckBox::setToggleState( bool value )
{
    m_toggleState = value;
}


/************************************************************************
*    DESC:  Get the Select state of this control
*  
*    ret: EControlState - control state
************************************************************************/
bool CUICheckBox::getToggleState()
{
    return m_toggleState;
}

/************************************************************************
*    DESC:  Handle OnSelectExecute message
************************************************************************/
void CUICheckBox::onSelectExecute( const SDL_Event & rEvent )
{
    if( getState() == NUIControl::ECS_SELECTED )
        m_toggleState = !m_toggleState;

    CUIControl::onSelectExecute( rEvent );
}


/***************************************************************************
*    DESC:  Record the command buffer for all the sprite
*           objects that are to be rendered
****************************************************************************/
void CUICheckBox::recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuf, const CCamera & camera )
{
    for( size_t i = 0; i < m_spriteDeq.size()-1; ++i )
        m_spriteDeq[i].recordCommandBuffer( index, cmdBuf, camera );

    if( m_toggleState == NUIControl::TOGGLE_STATE_ON )
        m_spriteDeq.back().recordCommandBuffer( index, cmdBuf, camera );
}
