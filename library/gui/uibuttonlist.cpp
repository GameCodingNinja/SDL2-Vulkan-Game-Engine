
/************************************************************************
*    FILE NAME:       uibuttonlist.cpp
*
*    DESCRIPTION:     Class for user interface buttons
************************************************************************/

// Physical component dependency
#include <gui/uibuttonlist.h>

// Game lib dependencies
#include <gui/menudefs.h>
#include <gui/uicontroldefs.h>
#include <2d/sprite2d.h>
#include <utilities/genfunc.h>
#include <objectdata/objectdata2d.h>
#include <objectdata/objectvisualdata2d.h>
#include <utilities/exceptionhandling.h>
#include <common/defs.h>

// Standard lib dependencies
#include <cstring>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CUIButtonList::CUIButtonList( const std::string & group ) :
    CUISubControl( group ),
    m_activeIndex(0),
    m_imageLstIndex(-1)
{
    m_type = NUIControl::ECT_BUTTON_LIST;

}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CUIButtonList::~CUIButtonList()
{
}


/************************************************************************
*    DESC:  Load the control info from XML node
************************************************************************/
void CUIButtonList::loadFromNode( const XMLNode & node )
{
    // Call the parent
    CUISubControl::loadFromNode( node );

    // See what the control will respond to
    const XMLNode actionResponse = node.getChildNode( "actionResponse" );
    if( actionResponse.isAttributeSet( "up" ) &&
        (std::strcmp( actionResponse.getAttribute("up"), "true" ) == 0) )
    {
        m_ActionMask.add( EAR_UP );
    }

    if( actionResponse.isAttributeSet( "down" ) &&
        (std::strcmp( actionResponse.getAttribute("down"), "true" ) == 0) )
    {
        m_ActionMask.add( EAR_DOWN );
    }

    if( actionResponse.isAttributeSet( "left" ) &&
        (std::strcmp( actionResponse.getAttribute("left"), "true" ) == 0) )
    {
        m_ActionMask.add( EAR_LEFT );
    }

    if( actionResponse.isAttributeSet( "right" ) &&
        (std::strcmp( actionResponse.getAttribute("right"), "true" ) == 0) )
    {
        m_ActionMask.add( EAR_RIGHT );
    }
}


/************************************************************************
*    DESC:  Load the control specific info from XML node
************************************************************************/
void CUIButtonList::loadControlFromNode( const XMLNode & node )
{
    // Call the parent
    CUISubControl::loadControlFromNode( node );

    // See if there is an image list
    for( size_t i = 0; i < m_spriteDeq.size(); ++i )
    {
	if( m_spriteDeq[i].getObjectData().getVisualData().getFrameCount() > 1 )
        {
            m_imageLstIndex = i;
            break;
        }
    }
}


/************************************************************************
*    DESC:  Inc/Dec control
************************************************************************/
void CUIButtonList::inc()
{
    NGenFunc::DispatchEvent(
            NMenu::EGE_MENU_CONTROL_STATE_CHANGE,
            NUIControl::ECS_SELECTED,
            (void *)m_pSubControlVec[NUIControl::BTN_INC] );
}

void CUIButtonList::dec()
{
    NGenFunc::DispatchEvent(
        NMenu::EGE_MENU_CONTROL_STATE_CHANGE,
        NUIControl::ECS_SELECTED,
        (void *)m_pSubControlVec[NUIControl::BTN_DEC] );
}


/************************************************************************
*    DESC:  Handle Onmessage
************************************************************************/
void CUIButtonList::onDownAction( const SDL_Event & rEvent )
{
    if( (rEvent.user.code == NDefs::EAP_DOWN) && m_ActionMask.isSet( EAR_DOWN ) )
        dec();
}

void CUIButtonList::onUpAction( const SDL_Event & rEvent )
{
    if( (rEvent.user.code == NDefs::EAP_DOWN) && m_ActionMask.isSet( EAR_UP ) )
        inc();
}

void CUIButtonList::onLeftAction( const SDL_Event & rEvent )
{
    if( (rEvent.user.code == NDefs::EAP_DOWN) && m_ActionMask.isSet( EAR_LEFT ) )
        dec();
}

void CUIButtonList::onRightAction( const SDL_Event & rEvent )
{
    if( (rEvent.user.code == NDefs::EAP_DOWN) && m_ActionMask.isSet( EAR_RIGHT ) )
        inc();
}


/************************************************************************
*    DESC:  Handle OnLeftScroll message
************************************************************************/
void CUIButtonList::onDownScroll( const SDL_Event & rEvent )
{
    if( m_ActionMask.isSet( EAR_DOWN ) )
        dec();
}

void CUIButtonList::onUpScroll( const SDL_Event & rEvent )
{
    if( m_ActionMask.isSet( EAR_UP ) )
        inc();
}

void CUIButtonList::onLeftScroll( const SDL_Event & rEvent )
{
    if( m_ActionMask.isSet( EAR_LEFT ) )
        dec();
}

void CUIButtonList::onRightScroll( const SDL_Event & rEvent )
{
    if( m_ActionMask.isSet( EAR_RIGHT ) )
        inc();
}


/************************************************************************
*    DESC:  Handle OnStateChange message
************************************************************************/
void CUIButtonList::onStateChange( const SDL_Event & rEvent )
{
    CUISubControl::onStateChange( rEvent );

    NUIControl::EControlState state = NUIControl::EControlState(rEvent.user.code);

    if( state == NUIControl::ECS_SELECTED )
    {
        if( (void *)m_pSubControlVec[NUIControl::BTN_DEC] == rEvent.user.data1 )
        {
            // Dec the list
            decList();

            // Update the display
            updateDisplay( m_activeIndex );

            // Execute smart gui
            smartExecuteAction();
        }
        else if( (void *)m_pSubControlVec[NUIControl::BTN_INC] == rEvent.user.data1 )
        {
            // Inc the list
            incList();

            // Update the display
            updateDisplay( m_activeIndex );

            // Execute smart gui
            smartExecuteAction();
        }
    }
}


/************************************************************************
*    DESC:  Inc the list
************************************************************************/
void CUIButtonList::incList()
{
    if( !m_stringVec.empty() )
        m_activeIndex = (m_activeIndex + 1) % m_stringVec.size();
}

void CUIButtonList::decList()
{
    if( !m_stringVec.empty() )
    {
        if( m_activeIndex > 0 )
            m_activeIndex = (m_activeIndex - 1) % m_stringVec.size();
        else
            m_activeIndex = static_cast<int>(m_stringVec.size()-1);
    }
}


/************************************************************************
*    DESC:  Update the display
************************************************************************/
void CUIButtonList::updateDisplay( int index )
{
    m_activeIndex = index;

    createFontString( m_activeIndex );

    if( m_imageLstIndex > -1 )
        m_spriteDeq.at(m_imageLstIndex).getVisualComponent().setFrame( m_activeIndex );
}


/************************************************************************
*    DESC: Set the first inactive control to be active
*    NOTE: Don't want this functuality for the button list
************************************************************************/
bool CUIButtonList::activateFirstInactiveControl()
{
    return CUIControl::activateFirstInactiveControl();
} 


/************************************************************************
*    DESC:  Get the active index
************************************************************************/
int CUIButtonList::getActiveIndex() const
{
    return m_activeIndex;
}
