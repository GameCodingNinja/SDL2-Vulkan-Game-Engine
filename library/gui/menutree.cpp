
/************************************************************************
*    FILE NAME:       menutree.h
*
*    DESCRIPTION:     Class that hold a tree of menus
************************************************************************/

// Physical component dependency
#include <gui/menutree.h>

// Game lib dependencies
#include <utilities/xmlParser.h>
#include <utilities/xmlparsehelper.h>
#include <utilities/exceptionhandling.h>
#include <utilities/genfunc.h>
#include <gui/menu.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CMenuTree::CMenuTree(
    const std::string & name,
    std::map<const std::string, CMenu> & rMenuMap,
    const std::string & rootMenu,
    const std::string & defaultMenu,
    bool interfaceTree ) :
    m_name(name),
    m_rMenuMap( rMenuMap ),
    m_pRootMenu( nullptr),
    m_pDefaultMenu( nullptr ),
    m_interfaceTree( interfaceTree ),
    m_state( EMenuState::IDLE )
{
    auto iter = rMenuMap.find( rootMenu );
    if( iter != rMenuMap.end() )
        m_pRootMenu = &iter->second;

    iter = rMenuMap.find( defaultMenu );
    if( iter != rMenuMap.end() )
        m_pDefaultMenu = &iter->second;
}

/************************************************************************
*    DESC:  destructor
************************************************************************/
CMenuTree::~CMenuTree()
{
}

/************************************************************************
*    DESC:  Init the tree for use
************************************************************************/
void CMenuTree::init()
{
    m_pMenuPathVec.clear();

    if( m_pRootMenu != nullptr )
    {
        // If we have a root menu, add it to the path
        m_pMenuPathVec.push_back( m_pRootMenu );

        m_pRootMenu->initRootMenu();
    }
}

/************************************************************************
*    DESC:  Update the menu tree
************************************************************************/
void CMenuTree::update()
{
    for( auto iter : m_pMenuPathVec )
        iter->update();
}

/************************************************************************
*    DESC:  Transform the menu tree
************************************************************************/
void CMenuTree::transform()
{
    for( auto iter : m_pMenuPathVec )
        iter->transform();
}

void CMenuTree::transform( const CObject & object )
{
    for( auto iter : m_pMenuPathVec )
        iter->transform( object );
}

/***************************************************************************
*    DESC:  Record the command buffer for all the sprite
*           objects that are to be rendered
****************************************************************************/
void CMenuTree::recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuf, const CCamera & camera )
{
    for( auto iter : m_pMenuPathVec )
        iter->recordCommandBuffer( index, cmdBuf, camera );
}

/************************************************************************
*    DESC:  Is a tree active?
************************************************************************/
bool CMenuTree::isActive()
{
    return !m_pMenuPathVec.empty();
}

/************************************************************************
*    DESC:  Does this tee have a root menu
************************************************************************/
bool CMenuTree::hasRootMenu()
{
    return (m_pRootMenu != nullptr);
}

/************************************************************************
*    DESC:  Handle events
************************************************************************/
void CMenuTree::handleEvent( const SDL_Event & rEvent )
{
    // Trap only controller events to check for actions
    if( !m_interfaceTree )
    {
        if( !m_pMenuPathVec.empty() )
            m_pMenuPathVec.back()->handleEvent( rEvent );

        if( m_state == EMenuState::IDLE )
        {
            if( rEvent.type == NMenuEvent::ESCAPE_ACTION )
            {
                onEscape( rEvent );
            }
            else if( rEvent.type == NMenuEvent::TOGGLE_ACTION )
            {
                onToggle( rEvent );
            }
            else if( rEvent.type == NMenuEvent::BACK_ACTION )
            {
                onBack( rEvent );
            }
            else if( rEvent.type == NMenuEvent::TO_TREE )
            {
                onToTree( rEvent );
            }
            else if( rEvent.type == NMenuEvent::TO_MENU )
            {
                onToMenu( rEvent );
            }
        }
        else if( rEvent.type == NMenuEvent::TRANS_IN )
        {
            onTransIn( rEvent );
        }
        else if( rEvent.type == NMenuEvent::TRANS_OUT )
        {
            onTransOut( rEvent );
        }
    }
    // Don't process menu specific messages if this is an interface menu
    else if( (rEvent.type < NMenuEvent::USER_EVENTS) || (rEvent.type > NMenuEvent::GAME_STATE_CHANGE) )
    {
        if( !m_pMenuPathVec.empty() )
            m_pMenuPathVec.back()->handleEvent( rEvent );
    }
}

/************************************************************************
*    DESC:  Activate a menu
************************************************************************/
void CMenuTree::activateMenu( const std::string & menuName )
{
    // Do a sanity check to make sure the menu exists
    auto iter = m_rMenuMap.find(menuName);
    if( iter == m_rMenuMap.end() )
        throw NExcept::CCriticalException("Menu Display Error!",
            boost::str( boost::format("Menu does not exist (%s).\n\n%s\nLine: %s")
                % menuName % __FUNCTION__ % __LINE__ ));

    // Get the name of the menu we are transitioning to
    // This is also used as a flag to indicate moving up the menu tree
    m_toMenu = menuName;

    if( m_pMenuPathVec.empty() )
    {
        // Add the menu to the path
        m_pMenuPathVec.push_back( &iter->second );

        // Set the state as "active" so that input messages are ignored
        m_state = EMenuState::ACTIVE;

        // Start the transition in
        NGenFunc::DispatchEvent( NMenuEvent::TRANS_IN, NTransCode::BEGIN );
    }
    else
    {
        // If this isn't the root menu, start the transition out
        if( m_pMenuPathVec.back() != m_pRootMenu )
        {
            // Set the state as "active" so that input messages are ignored
            m_state = EMenuState::ACTIVE;

            // Start the transition out
            NGenFunc::DispatchEvent( NMenuEvent::TRANS_OUT, NTransCode::BEGIN );
        }
    }
}

/************************************************************************
*    DESC:  Transition the menu
************************************************************************/
void CMenuTree::transitionMenu()
{
    // If the path vector is empty, transition to the default menu
    if( m_pMenuPathVec.empty() )
    {
        // Make sure the menu exists
        if( m_pDefaultMenu == nullptr )
            throw NExcept::CCriticalException("Menu Display Error!",
                boost::str( boost::format("Default menu does not exist.\n\n%s\nLine: %s")
                    % __FUNCTION__ % __LINE__ ));

        // Add the default menu to the path
        m_pMenuPathVec.push_back( m_pDefaultMenu );

        // Get the name of the menu we are transitioning to
        // This is also used as a flag to indicate moving up the menu tree
        m_toMenu = m_pDefaultMenu->getName();

        // Set the state as "active" so that input messages are ignored
        m_state = EMenuState::ACTIVE;

        // Start the transition in
        NGenFunc::DispatchEvent( NMenuEvent::TRANS_IN, NTransCode::BEGIN );
    }
    else
    {
        // If this isn't the root menu, start the transition out
        if( m_pMenuPathVec.back() != m_pRootMenu )
        {
            // Set the state as "active" so that input messages are ignored
            m_state = EMenuState::ACTIVE;

            // Start the transition out
            NGenFunc::DispatchEvent( NMenuEvent::TRANS_OUT, NTransCode::BEGIN );
        }
    }
}

/************************************************************************
*    DESC:  Handle OnEscape message
************************************************************************/
void CMenuTree::onEscape( const SDL_Event & rEvent )
{
    std::string * pNameStr = (std::string *)rEvent.user.data1;
    if( !m_pMenuPathVec.empty() || ((pNameStr != nullptr) && (*pNameStr == m_name)))
    {
        transitionMenu();
    }
}

/************************************************************************
*    DESC:  Handle OnToggle message
************************************************************************/
void CMenuTree::onToggle( const SDL_Event & rEvent )
{
    std::string * pNameStr = (std::string *)rEvent.user.data1;
    if( !m_pMenuPathVec.empty() || ((pNameStr != nullptr) && (*pNameStr == m_name)) )
    {
        // Toggle "on" only works when there is no root menu
        if( m_pRootMenu == nullptr )
        {
            transitionMenu();

            // For toggle, clear out the path vector except for the current menu
            // The current menu will then be used for the transitions out
            if( m_pMenuPathVec.size() > 1 )
            {
                CMenu * pCurMenu = m_pMenuPathVec.back();
                m_pMenuPathVec.clear();
                m_pMenuPathVec.push_back( pCurMenu );
            }
        }
        else
        {
            if( m_pMenuPathVec.size() > 1 )
                transitionMenu();

            // For toggle, clear out the path vector except for the current and root menu
            // The current menu will then be used for the transitions out
            if( m_pMenuPathVec.size() > 2 )
            {
                CMenu * pCurMenu = m_pMenuPathVec.back();
                m_pMenuPathVec.clear();
                m_pMenuPathVec.push_back( m_pRootMenu );
                m_pMenuPathVec.push_back( pCurMenu );
            }
        }
    }
}

/************************************************************************
*    DESC:  Handle OnBack message
************************************************************************/
void CMenuTree::onBack( const SDL_Event & rEvent )
{
    // Going back one require there to be a active menu that is not root
    if( !m_pMenuPathVec.empty() && (m_pMenuPathVec.back() != m_pRootMenu) )
    {
        transitionMenu();
    }

}

/************************************************************************
*    DESC:  Handle OnToTree message
************************************************************************/
void CMenuTree::onToTree( const SDL_Event & rEvent )
{
    std::string * pNameStr = (std::string *)rEvent.user.data1;
    if( (pNameStr != nullptr) && (*pNameStr == m_name) )
    {
        // Only works when there is no root menu
        if( m_pRootMenu == nullptr )
            transitionMenu();
    }
}

/************************************************************************
*    DESC:  Handle OnToMenu message
************************************************************************/
void CMenuTree::onToMenu( const SDL_Event & rEvent )
{
    // Going to a menu require there to be a active menu
    // and the calling control is on a menu on this tree
    if( !m_pMenuPathVec.empty() &&
        ((void *)m_pMenuPathVec.back()->getPtrToActiveControl() == rEvent.user.data2) )
    {
        // Set the state as "active" so that input messages are ignored
        m_state = EMenuState::ACTIVE;

        // Get the name of the menu we are transitioning to
        // This is also used as a flag to indicate moving deaper into the menu tree
        m_toMenu = *(std::string *)rEvent.user.data1;

        // Do a sanity check to make sure the menu exists
        if( m_rMenuMap.find(m_toMenu) == m_rMenuMap.end() )
            throw NExcept::CCriticalException("Menu Display Error!",
                boost::str( boost::format("Menu does not exist (%s).\n\n%s\nLine: %s")
                    % m_toMenu % __FUNCTION__ % __LINE__ ));

        // Start the transition out
        NGenFunc::DispatchEvent( NMenuEvent::TRANS_OUT, NTransCode::BEGIN );
    }
}

/************************************************************************
*    DESC:  Handle OnTransOut message
************************************************************************/
void CMenuTree::onTransOut( const SDL_Event & rEvent )
{
    if( rEvent.user.code == NTransCode::END )
    {
        if( !m_toMenu.empty() )
        {
            m_pMenuPathVec.push_back( &m_rMenuMap.find(m_toMenu)->second );
            NGenFunc::DispatchEvent( NMenuEvent::TRANS_IN, NTransCode::BEGIN );
        }
        else if( !m_pMenuPathVec.empty() && (m_pMenuPathVec.back() != m_pRootMenu) )
        {
            // Do a full reset on all the controls
            m_pMenuPathVec.back()->reset();

            // Pop it off the vector because this menu is done
            m_pMenuPathVec.pop_back();

            if( !m_pMenuPathVec.empty() )
                NGenFunc::DispatchEvent( NMenuEvent::TRANS_IN, NTransCode::BEGIN );
        }

        // Normally, after one menu transitions out, the next menu transitions in
        // Only set the idle state if this transition out is final
        if( m_pMenuPathVec.empty() )
            m_state = EMenuState::IDLE;
    }
}

/************************************************************************
*    DESC:  Handle OnTransIn message
************************************************************************/
void CMenuTree::onTransIn( const SDL_Event & rEvent )
{
    if( rEvent.user.code == NTransCode::END )
    {
        // m_toMenu is also used as a flag to indicate moving up the menu tree
        // When moving up the menu tree, activate the first control on the menu
        // When backing out of the menu tree, activate the last control used previously
        NGenFunc::DispatchEvent( NMenuEvent::SET_ACTIVE_CONTROL,
            (m_toMenu.empty()) ? NActiveControl::LAST : NActiveControl::FIRST );

        // Set to idle to allow for input messages to come through
        m_state = EMenuState::IDLE;

        // Clear in the event we start backing out of the menu tree
        m_toMenu.clear();
    }
}

/************************************************************************
*    DESC:  Get the active menu
************************************************************************/
CMenu & CMenuTree::getActiveMenu()
{
    if( m_pMenuPathVec.empty() )
        throw NExcept::CCriticalException("Menu Error!",
            boost::str( boost::format("There is no active menu.\n\n%s\nLine: %s")
                % __FUNCTION__ % __LINE__ ));

    return *m_pMenuPathVec.back();
}

/************************************************************************
*    DESC:  Get the scroll param data
************************************************************************/
CScrollParam & CMenuTree::getScrollParam( int msg )
{
    if( m_pMenuPathVec.empty() )
        throw NExcept::CCriticalException("Menu Error!",
            boost::str( boost::format("There is no active menu.\n\n%s\nLine: %s")
                % __FUNCTION__ % __LINE__ ));

    return m_pMenuPathVec.back()->getScrollParam( msg );
}

/************************************************************************
*    DESC:  Is a menu item active
************************************************************************/
bool CMenuTree::isMenuItemActive()
{
    if( isActive() )
    {
        if( getActiveMenu().getPtrToActiveControl() != nullptr )
            return true;
    }

    return false;
}

/************************************************************************
*    DESC:  Is this an interface tree
************************************************************************/
bool CMenuTree::isInterfaceTree() const
{
    return m_interfaceTree;
}

/************************************************************************
*    DESC:  Get the name of the tree
************************************************************************/
std::string & CMenuTree::getName()
{
    return m_name;
}
