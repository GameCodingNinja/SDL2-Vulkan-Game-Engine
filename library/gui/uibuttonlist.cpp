
/************************************************************************
*    FILE NAME:       uibuttonlist.cpp
*
*    DESCRIPTION:     Class for user interface buttons
************************************************************************/

// Physical component dependency
#include <gui/uibuttonlist.h>

// Game lib dependencies
#include <sprite/sprite.h>
#include <utilities/genfunc.h>
#include <objectdata/objectdata2d.h>
#include <objectdata/objectvisualdata2d.h>
#include <utilities/exceptionhandling.h>
#include <utilities/xmlParser.h>
#include <common/defs.h>
#include <common/ivisualcomponent.h>

// Standard lib dependencies
#include <cstring>

// SDL lib dependencies
#include <SDL3/SDL.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CUIButtonList::CUIButtonList( const std::string & group ) :
    CUISubControl( group ),
    m_activeIndex(0),
    m_imageLstIndex(-1)
{
    m_type = EControlType::BUTTON_LIST;
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
        m_ActionMask.add( static_cast<int>(EAction::UP) );
    }

    if( actionResponse.isAttributeSet( "down" ) &&
        (std::strcmp( actionResponse.getAttribute("down"), "true" ) == 0) )
    {
        m_ActionMask.add( static_cast<int>(EAction::DOWN) );
    }

    if( actionResponse.isAttributeSet( "left" ) &&
        (std::strcmp( actionResponse.getAttribute("left"), "true" ) == 0) )
    {
        m_ActionMask.add( static_cast<int>(EAction::LEFT) );
    }

    if( actionResponse.isAttributeSet( "right" ) &&
        (std::strcmp( actionResponse.getAttribute("right"), "true" ) == 0) )
    {
        m_ActionMask.add( static_cast<int>(EAction::RIGHT) );
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
    for( size_t i = 0; i < m_pSpriteVec.size(); ++i )
    {
	    if( m_pSpriteVec[i]->getObjectData().getVisualData().getFrameCount() > 1 )
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
            NMenuEvent::CONTROL_STATE_CHANGE,
            static_cast<int>(EControlState::SELECT),
            (void *)m_pSubControlVec[static_cast<int>(EAction::INC)] );
}

void CUIButtonList::dec()
{
    NGenFunc::DispatchEvent(
        NMenuEvent::CONTROL_STATE_CHANGE,
        static_cast<int>(EControlState::SELECT),
        (void *)m_pSubControlVec[static_cast<int>(EAction::DEC)] );
}

/************************************************************************
*    DESC:  Handle Onmessage
************************************************************************/
void CUIButtonList::onDownAction( const SDL_Event & rEvent )
{
    if( (rEvent.user.code == static_cast<int>(EActionPress::DOWN)) && m_ActionMask.isSet( static_cast<int>(EAction::DOWN) ) )
        dec();
}

void CUIButtonList::onUpAction( const SDL_Event & rEvent )
{
    if( (rEvent.user.code == static_cast<int>(EActionPress::DOWN)) && m_ActionMask.isSet( static_cast<int>(EAction::UP) ) )
        inc();
}

void CUIButtonList::onLeftAction( const SDL_Event & rEvent )
{
    if( (rEvent.user.code == static_cast<int>(EActionPress::DOWN)) && m_ActionMask.isSet( static_cast<int>(EAction::LEFT) ) )
        dec();
}

void CUIButtonList::onRightAction( const SDL_Event & rEvent )
{
    if( (rEvent.user.code == static_cast<int>(EActionPress::DOWN)) && m_ActionMask.isSet( static_cast<int>(EAction::RIGHT) ) )
        inc();
}

/************************************************************************
*    DESC:  Handle OnLeftScroll message
************************************************************************/
void CUIButtonList::onDownScroll( const SDL_Event & rEvent )
{
    if( m_ActionMask.isSet( static_cast<int>(EAction::DOWN) ) )
        dec();
}

void CUIButtonList::onUpScroll( const SDL_Event & rEvent )
{
    if( m_ActionMask.isSet( static_cast<int>(EAction::UP) ) )
        inc();
}

void CUIButtonList::onLeftScroll( const SDL_Event & rEvent )
{
    if( m_ActionMask.isSet( static_cast<int>(EAction::LEFT) ) )
        dec();
}

void CUIButtonList::onRightScroll( const SDL_Event & rEvent )
{
    if( m_ActionMask.isSet( static_cast<int>(EAction::RIGHT) ) )
        inc();
}

/************************************************************************
*    DESC:  Handle OnStateChange message
************************************************************************/
void CUIButtonList::onStateChange( const SDL_Event & rEvent )
{
    CUISubControl::onStateChange( rEvent );

    EControlState state = static_cast<EControlState>(rEvent.user.code);

    if( state == EControlState::SELECT )
    {
        if( (void *)m_pSubControlVec[static_cast<int>(EAction::DEC)] == rEvent.user.data1 )
        {
            // Dec the list
            decList();

            // Update the display
            updateDisplay();

            // Prepare script function associated with handling this game event
            prepareControlScriptFunction( EControlState::CHANGE );
        }
        else if( (void *)m_pSubControlVec[static_cast<int>(EAction::INC)] == rEvent.user.data1 )
        {
            // Inc the list
            incList();

            // Update the display
            updateDisplay();

            // Prepare script function associated with handling this game event
            prepareControlScriptFunction( EControlState::CHANGE );
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
void CUIButtonList::updateDisplay()
{
    createFontString( m_activeIndex );

    if( m_imageLstIndex > -1 )
        m_pSpriteVec.at(m_imageLstIndex)->getVisualComponent()->setFrame( m_activeIndex );
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
*    DESC:  Get/Set the active index
************************************************************************/
int CUIButtonList::getActiveIndex() const
{
    return m_activeIndex;
}

void CUIButtonList::setActiveIndex( int index )
{
    m_activeIndex = index;
    
    // Update the display
    updateDisplay();
}

/************************************************************************
*    DESC:  Number of items in the list box
************************************************************************/
size_t CUIButtonList::size()
{
    if( m_pSpriteVec.empty() )
        return m_pSpriteVec.size();

    else if( !m_stringVec.empty() )
        return m_stringVec.size();

    return 0;
}
