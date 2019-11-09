
/************************************************************************
*    FILE NAME:       menumanager.cpp
*
*    DESCRIPTION:     menu manager class singleton
************************************************************************/

// Physical component dependency
#include <gui/menumanager.h>

// Game lib dependencies
#include <utilities/xmlParser.h>
#include <utilities/exceptionhandling.h>
#include <utilities/genfunc.h>
#include <utilities/settings.h>
#include <gui/menutree.h>
#include <gui/menu.h>
#include <gui/scrollparam.h>
#include <gui/messagecracker.h>
#include <managers/actionmanager.h>
#include <managers/cameramanager.h>
#include <system/device.h>

// SDL lib dependencies
#include <SDL.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CMenuMgr::CMenuMgr() :
    m_active(false),
    m_scrollTimerId(0),
    m_allow(false),
    m_pCamera( &CCameraMgr::Instance().getDefault() )
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CMenuMgr::~CMenuMgr()
{
    SDL_RemoveTimer( m_scrollTimerId );
}


/************************************************************************
 *    DESC:  Load the menu group
 ************************************************************************/
void CMenuMgr::loadGroup( const std::string & group, const bool doInit )
{
    // Check for a hardware extension
    std::string ext;
    if( !m_mobileExt.empty() && CSettings::Instance().isMobileDevice() )
        if( m_listTableMap.find( group + m_mobileExt ) != m_listTableMap.end() )
            ext = m_mobileExt;

    // Make sure the group we are looking has been defined in the list table file
    auto listTableIter = m_listTableMap.find( group + ext );
    if( listTableIter == m_listTableMap.end() )
        throw NExcept::CCriticalException("Menu Load Group Error!",
            boost::str( boost::format("Menu group name can't be found (%s).\n\n%s\nLine: %s")
                % group % __FUNCTION__ % __LINE__ ));

    // Load the group data if it doesn't already exist
    if( m_menuMapMap.find( group ) == m_menuMapMap.end() )
    {
        // Create a new group map inside of our maps
        m_menuMapMap.emplace( group, std::map<const std::string, CMenu>() );
        m_menuTreeMapMap.emplace( group, std::map<const std::string, CMenuTree>() );

        for( auto & iter : listTableIter->second )
            load( group, iter );
    }
    else
    {
        throw NExcept::CCriticalException("Menu Group load Error!",
            boost::str( boost::format("Menu group has alread been loaded (%s).\n\n%s\nLine: %s")
                % group % __FUNCTION__ % __LINE__ ));
    }

    if( doInit )
        initGroup( group );
}


/************************************************************************
 *    DESC:  Free the menu group
 ************************************************************************/
void CMenuMgr::freeGroup( const std::string & group )
{
    // Make sure the group we are looking for exists
    auto listTableIter = m_listTableMap.find( group );
    if( listTableIter == m_listTableMap.end() )
        throw NExcept::CCriticalException("Menu Free Group Error!",
            boost::str( boost::format("Menu group name can't be found (%s).\n\n%s\nLine: %s")
                % group % __FUNCTION__ % __LINE__ ));

    // Free the tree group
    auto treeMapIter = m_menuTreeMapMap.find( group );
    if( treeMapIter != m_menuTreeMapMap.end() )
    {
        // Remove it from the tree vectors if it is there
        for( auto & treeIter : treeMapIter->second )
        {
            if( treeIter.second.isInterfaceTree() )
            {
                auto interIter = std::find( m_pActiveInterTreeVec.begin(), m_pActiveInterTreeVec.end(), &treeIter.second );
                if( interIter != m_pActiveInterTreeVec.end() )
                    m_pActiveInterTreeVec.erase( interIter );
            }
            else
            {
                auto menuIter = std::find( m_pActiveMenuTreeVec.begin(), m_pActiveMenuTreeVec.end(), &treeIter.second );
                if( menuIter != m_pActiveMenuTreeVec.end() )
                    m_pActiveMenuTreeVec.erase( menuIter );
            }
        }

        m_menuTreeMapMap.erase( treeMapIter );
    }

    // Free the menu group
    auto menuMapIter = m_menuMapMap.find( group );
    if( menuMapIter != m_menuMapMap.end() )
        m_menuMapMap.erase( menuMapIter );
}


/************************************************************************
*    DESC:  Clear all memory
************************************************************************/
void CMenuMgr::clear()
{
    m_allow = false;
    
    // Clear the active trees
    clearActiveTrees();
    
    m_menuTreeMapMap.clear();
    m_menuMapMap.clear();
}


/************************************************************************
 *    DESC:  Init a menu group
 *           This allows certain actions to be done after the group load
 ************************************************************************/
void CMenuMgr::initGroup( const std::string & group )
{
    auto menuMapIter = m_menuMapMap.find( group );
    if( menuMapIter != m_menuMapMap.end() )
    {
        for( auto & iter : menuMapIter->second )
            iter.second.init();
    }
    else
    {
        throw NExcept::CCriticalException("Menu Init Group Error!",
            boost::str( boost::format("Menu group name can't be found to clean up (%s).\n\n%s\nLine: %s")
                % group % __FUNCTION__ % __LINE__ ));
    }
}


/************************************************************************
*    DESC:  Load the menu/tree info from file
************************************************************************/
void CMenuMgr::load( const std::string & group, const std::string & filePath )
{
    // open and parse the XML file:
    const XMLNode node = XMLNode::openFileHelper( filePath.c_str(), "menuTreeList" );

    // Load the menus from node
    loadMenusFromNode( group, node );

    // Load the trees from node
    loadTreesFromNode( group, node );
}


/************************************************************************
*    DESC:  Load the menus from node
************************************************************************/
void CMenuMgr::loadMenusFromNode( const std::string & group, const XMLNode & node )
{
    // Get an iterator to the menu group
    auto menuMapIter = m_menuMapMap.find( group );

    // Get the node to the default object data
    const XMLNode menuLstNode = node.getChildNode( "menuList" );

    for( int i = 0; i < menuLstNode.nChildNode(); ++i )
    {
        // Menu name and file path
        const XMLNode menuNode = menuLstNode.getChildNode(i);

        // Get the name of the menu
        const std::string name = menuNode.getAttribute( "name" );

        // Insert the menu into the menu map
        auto iter = menuMapIter->second.emplace( std::piecewise_construct, std::forward_as_tuple(name), std::forward_as_tuple(name, group) );

        // Check for duplicate names
        if( !iter.second )
        {
            throw NExcept::CCriticalException("Menu Load Error!",
                boost::str( boost::format("Duplicate menu name (%s).\n\n%s\nLine: %s")
                    % name % __FUNCTION__ % __LINE__ ));
        }

        // Load the transform from node
        iter.first->second.loadTransFromNode( menuNode );

        // Load the dynamic offset from node
        iter.first->second.loadDynamicOffsetFromNode( menuNode );

        // Have the menu load it's share
        iter.first->second.load( menuNode.getAttribute( "file" ) );
    }
}


/************************************************************************
*    DESC:  Load the trees from node
************************************************************************/
void CMenuMgr::loadTreesFromNode( const std::string & group, const XMLNode & node )
{
    // Get an iterator to the menu group
    auto menuMapIter = m_menuMapMap.find( group );

    // Get an iterator to the tree group
    auto treeMapIter = m_menuTreeMapMap.find( group );

    // Get the node to the default object data
    const XMLNode treeLstNode = node.getChildNode( "treeList" );

    for( int i = 0; i < treeLstNode.nChildNode(); ++i )
    {
        // Get the tree node
        const XMLNode treeNode = treeLstNode.getChildNode("tree", i);

        // Get the name
        const std::string name = treeNode.getAttribute( "name" );

        // Get the root menu
        std::string rootMenu;
        if( treeNode.isAttributeSet("root") )
            rootMenu = treeNode.getAttribute( "root" );

        // Get the default menu
        std::string defaultMenu;
        if( treeNode.isAttributeSet("default") )
            defaultMenu = treeNode.getAttribute( "default" );

        // Is this tree an interface tree?
        bool interfaceTree(false);
        if( treeNode.isAttributeSet("interfaceTree") )
            interfaceTree = ( std::strcmp( treeNode.getAttribute("interfaceTree"), "true" ) == 0 );

        // Add the tree data to the map
        auto iter = treeMapIter->second.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(name),
            std::forward_as_tuple(name, menuMapIter->second, rootMenu, defaultMenu, interfaceTree) );

        // Check for duplicate names
        if( !iter.second )
        {
            throw NExcept::CCriticalException("Menu Tree Load Error!",
                boost::str( boost::format("Duplicate tree name (%s).\n\n%s\nLine: %s")
                    % name % __FUNCTION__ % __LINE__ ));
        }

        // Check that the root menu exists
        if( !rootMenu.empty() )
        {
            if( menuMapIter->second.find(rootMenu) == menuMapIter->second.end() )
            {
                throw NExcept::CCriticalException("Menu Load Error!",
                    boost::str( boost::format("Root menu doesn't exist (%s).\n\n%s\nLine: %s")
                        % rootMenu % __FUNCTION__ % __LINE__ ));
            }
        }
        else if( !defaultMenu.empty() )
        {
            if( menuMapIter->second.find(defaultMenu) == menuMapIter->second.end() )
            {
                throw NExcept::CCriticalException("Menu Load Error!",
                    boost::str( boost::format("Default menu doesn't exist (%s).\n\n%s\nLine: %s")
                        % defaultMenu % __FUNCTION__ % __LINE__ ));
            }
        }
    }
}


/************************************************************************
*    DESC:  Load the menu action list
************************************************************************/
void CMenuMgr::loadMenuAction( const std::string & filePath )
{
    // open and parse the XML file:
    const XMLNode node = XMLNode::openFileHelper( filePath.c_str(), "menuActionList" );

    m_backAction = node.getChildNode( "backAction" ).getText();
    m_toggleAction = node.getChildNode( "toggleAction" ).getText();
    m_escapeAction = node.getChildNode( "escapeAction" ).getText();
    m_selectAction = node.getChildNode( "selectAction" ).getText();
    m_upAction = node.getChildNode( "upAction" ).getText();
    m_downAction = node.getChildNode( "downAction" ).getText();
    m_leftAction = node.getChildNode( "leftAction" ).getText();
    m_rightAction = node.getChildNode( "rightAction" ).getText();
    m_tabLeft = node.getChildNode( "tabLeft" ).getText();
    m_tabRight = node.getChildNode( "tabRight" ).getText();
    m_defaultTree = node.getChildNode( "defaultTree" ).getText();
}


/************************************************************************
*    DESC:  Activate a tree to be used by tree name only
*           NOTE: Assumes unique tree names
************************************************************************/
void CMenuMgr::activateMenu( const std::string & treeStr, const std::string & menuName )
{
    for( auto & groupIter : m_menuTreeMapMap )
    {
        for( auto & treeIter : groupIter.second )
        {
            if( treeIter.first == treeStr )
            {
                activateMenu( groupIter.first, treeIter.first, menuName );
                return;
            }
        }
    }

    // If you got this far, it's a problem
    throw NExcept::CCriticalException("Menu Activate Error!",
        boost::str( boost::format("Menu tree doesn't exist (%s).\n\n%s\nLine: %s")
            % treeStr % __FUNCTION__ % __LINE__ ));
} 

/************************************************************************
*    DESC:  Activate a menu to be used
************************************************************************/
void CMenuMgr::activateMenu( const std::string & group, const std::string & treeStr, const std::string & menuName )
{
    auto groupIter = m_menuTreeMapMap.find( group );
    if( groupIter != m_menuTreeMapMap.end() )
    {
        // Find the tree in the map
        auto treeIter = groupIter->second.find( treeStr );
        if( treeIter != groupIter->second.end() )
        {
            // This doesn't make sense for interface trees
            if( treeIter->second.isInterfaceTree() )
                throw NExcept::CCriticalException("Menu Activate Error!",
                    boost::str( boost::format("Interface menus can't be activated (%s - %s - %s).\n\n%s\nLine: %s")
                        % group % treeStr % menuName % __FUNCTION__ % __LINE__ ));

            // Init the menu for use
            treeIter->second.activateMenu( menuName );
        }
        else
        {
            throw NExcept::CCriticalException("Menu Activate Error!",
                boost::str( boost::format("Menu tree doesn't exist (%s - %s).\n\n%s\nLine: %s")
                    % group % treeStr % __FUNCTION__ % __LINE__ ));
        }
    }
    else
    {
        throw NExcept::CCriticalException("Menu Activate Error!",
            boost::str( boost::format("Menu tree group doesn't exist (%s - %s).\n\n%s\nLine: %s")
                % group % treeStr % __FUNCTION__ % __LINE__ ));
    }
}


/************************************************************************
*    DESC:  Activate a tree to be used by tree name only
*           NOTE: Assumes unique tree names
************************************************************************/
void CMenuMgr::activateTreeLst( const std::vector<std::string> & treeStrVec )
{
    for( auto & iter : treeStrVec )
        activateTree( iter );
}

void CMenuMgr::activateTree( const std::string & treeStr )
{
    for( auto & groupIter : m_menuTreeMapMap )
    {
        for( auto & treeIter : groupIter.second )
        {
            if( treeIter.first == treeStr )
            {
                activateTree( groupIter.first, treeIter.first );
                return;
            }
        }
    }

    // If you got this far, it's a problem
    throw NExcept::CCriticalException("Menu Tree Activate Error!",
        boost::str( boost::format("Menu tree doesn't exist (%s).\n\n%s\nLine: %s")
            % treeStr % __FUNCTION__ % __LINE__ ));
}

void CMenuMgr::activateTree( const std::string & group, const std::string & treeStr )
{
    auto groupIter = m_menuTreeMapMap.find( group );
    if( groupIter != m_menuTreeMapMap.end() )
    {
        // Find the tree in the map
        auto treeIter = groupIter->second.find( treeStr );
        if( treeIter != groupIter->second.end() )
        {
            if( treeIter->second.isInterfaceTree() )
            {
                if( std::find( m_pActiveInterTreeVec.begin(), m_pActiveInterTreeVec.end(), &treeIter->second ) != m_pActiveInterTreeVec.end() )
                    throw NExcept::CCriticalException("Menu Tree Activate Error!",
                        boost::str( boost::format("Menu tree already active (%s - %s).\n\n%s\nLine: %s")
                            % group % treeStr % __FUNCTION__ % __LINE__ ));

                m_pActiveInterTreeVec.push_back( &treeIter->second );
            }
            else
            {
                if( std::find( m_pActiveMenuTreeVec.begin(), m_pActiveMenuTreeVec.end(), &treeIter->second ) != m_pActiveMenuTreeVec.end() )
                    throw NExcept::CCriticalException("Menu Tree Activate Error!",
                        boost::str( boost::format("Menu tree already active (%s - %s).\n\n%s\nLine: %s")
                            % group % treeStr % __FUNCTION__ % __LINE__ ));

                m_pActiveMenuTreeVec.push_back( &treeIter->second );
            }

            // Init the tree for use
            treeIter->second.init();
        }
        else
        {
            throw NExcept::CCriticalException("Menu Tree Activate Error!",
                boost::str( boost::format("Menu tree doesn't exist (%s - %s).\n\n%s\nLine: %s")
                    % group % treeStr % __FUNCTION__ % __LINE__ ));
        }
    }
    else
    {
        throw NExcept::CCriticalException("Menu Tree Activate Error!",
            boost::str( boost::format("Menu tree group doesn't exist (%s - %s).\n\n%s\nLine: %s")
                % group % treeStr % __FUNCTION__ % __LINE__ ));
    }
}


/************************************************************************
*    DESC:  Deactivate a tree to be used
*           NOTE: Assumes unique tree names
************************************************************************/
void CMenuMgr::deactivateTreeLst( const std::vector<std::string> & treeStrVec )
{
    for( auto & iter : treeStrVec )
        deactivateTree( iter );
}

void CMenuMgr::deactivateTree( const std::string & treeStr )
{
    for( auto & groupIter : m_menuTreeMapMap )
    {
        for( auto & treeIter : groupIter.second )
        {
            if( treeIter.first == treeStr )
            {
                deactivateTree( groupIter.first, treeIter.first );
                return;
            }
        }
    }

    // If you got this far, it's a problem
    throw NExcept::CCriticalException("Menu Tree Deactivate Error!",
        boost::str( boost::format("Menu tree doesn't exist (%s - %s).\n\n%s\nLine: %s")
            % treeStr % __FUNCTION__ % __LINE__ ));
}

void CMenuMgr::deactivateTree( const std::string & group, const std::string & treeStr )
{
    auto groupIter = m_menuTreeMapMap.find( group );
    if( groupIter != m_menuTreeMapMap.end() )
    {
        // Find the tree in the map
        auto treeIter = groupIter->second.find( treeStr );
        if( treeIter != groupIter->second.end() )
        {
            // Remove the tree from the vector
            if( treeIter->second.isInterfaceTree() )
            {
                auto iter = std::find( m_pActiveInterTreeVec.begin(), m_pActiveInterTreeVec.end(), &treeIter->second );
                if( iter != m_pActiveInterTreeVec.end() )
                    m_pActiveInterTreeVec.erase( iter );
            }
            else
            {
                auto iter = std::find( m_pActiveMenuTreeVec.begin(), m_pActiveMenuTreeVec.end(), &treeIter->second );
                if( iter != m_pActiveMenuTreeVec.end() )
                    m_pActiveMenuTreeVec.erase( iter );
            }
        }
        else
        {
            throw NExcept::CCriticalException("Menu Tree Deactivate Error!",
                boost::str( boost::format("Menu tree doesn't exist (%s - %s).\n\n%s\nLine: %s")
                    % group % treeStr % __FUNCTION__ % __LINE__ ));
        }
    }
    else
    {
        throw NExcept::CCriticalException("Menu Tree Deacticate Error!",
            boost::str( boost::format("Menu tree group doesn't exist (%s - %s).\n\n%s\nLine: %s")
                % group % treeStr % __FUNCTION__ % __LINE__ ));
    }
}


/************************************************************************
*    DESC:  Clear the active trees
************************************************************************/
void CMenuMgr::clearActiveTrees()
{
    m_active = false;

    SDL_RemoveTimer( m_scrollTimerId );

    m_pActiveMenuTreeVec.clear();
    m_pActiveInterTreeVec.clear();
}


/************************************************************************
*    DESC:  Handle input events and dispatch menu events
************************************************************************/
void CMenuMgr::handleEvent( const SDL_Event & rEvent )
{
    if( m_allow )
    {
        // Convert keyboard, mouse and controller messages in action type messages
        if( (rEvent.type > SDL_SYSWMEVENT) && (rEvent.type < SDL_CLIPBOARDUPDATE) )
        {
            // Message TESTING code
            /*if( rEvent.type == SDL_CONTROLLERBUTTONDOWN )
                NGenFunc::PostDebugMsg( boost::str( boost::format("Controller Button: %d") % ((int)rEvent.cbutton.button) ) );
            else if( rEvent.type == SDL_JOYBUTTONDOWN )
                NGenFunc::PostDebugMsg( boost::str( boost::format("Joystick Button: %d") % ((int)rEvent.cbutton.button) ) );
            else if( rEvent.type == SDL_MOUSEMOTION )
                NGenFunc::PostDebugMsg( boost::str( boost::format("Mouse Motion: x = %d, y = %d") % ((int)rEvent.motion.x) % ((int)rEvent.motion.y) ) );
            else if( rEvent.type == SDL_MOUSEBUTTONDOWN )
                NGenFunc::PostDebugMsg( boost::str( boost::format("Mouse Button Down: %d") % ((int)rEvent.button.button) ) );
            else if( rEvent.type == SDL_MOUSEBUTTONUP )
                NGenFunc::PostDebugMsg( boost::str( boost::format("Mouse Button Up: %d") % ((int)rEvent.button.button) ) );
            else if( rEvent.type == SDL_KEYDOWN )
                NGenFunc::PostDebugMsg( boost::str( boost::format("Key Button: %d") % ((int)rEvent.key.keysym.sym) ) );

            else if( rEvent.type == SDL_CONTROLLERAXISMOTION )
            {
                if( rEvent.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX )
                    NGenFunc::PostDebugMsg( boost::str( boost::format("CAxis Left: x = %d") % ((int)rEvent.caxis.value) ) );
                else if( rEvent.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY )
                    NGenFunc::PostDebugMsg( boost::str( boost::format("CAxis Left: y = %d") % ((int)rEvent.caxis.value) ) );
                if( rEvent.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTX )
                    NGenFunc::PostDebugMsg( boost::str( boost::format("CAxis Right: x = %d") % ((int)rEvent.caxis.value) ) );
                else if( rEvent.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTY )
                    NGenFunc::PostDebugMsg( boost::str( boost::format("CAxis Right: y = %d") % ((int)rEvent.caxis.value) ) );
                else if( rEvent.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT )
                    NGenFunc::PostDebugMsg( boost::str( boost::format("CTrigger Left: %d") % ((int)rEvent.caxis.value) ) );
                else if( rEvent.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT )
                    NGenFunc::PostDebugMsg( boost::str( boost::format("CTrigger Right: %d") % ((int)rEvent.caxis.value) ) );
            }
            else if( rEvent.type == SDL_JOYAXISMOTION )
            {
                if( rEvent.jaxis.axis == SDL_CONTROLLER_AXIS_LEFTX )
                    NGenFunc::PostDebugMsg( boost::str( boost::format("Joystick Left: x = %d") % ((int)rEvent.jaxis.value) ) );
                else if( rEvent.jaxis.axis == SDL_CONTROLLER_AXIS_LEFTY )
                    NGenFunc::PostDebugMsg( boost::str( boost::format("Joystick Left: y = %d") % ((int)rEvent.jaxis.value) ) );
            }*/

            // Only the default tree can execute an escape or toggle when none are active.
            if( CActionMgr::Instance().wasAction( rEvent, m_escapeAction, NDefs::EAP_DOWN ) )
            {
                dispatchEscapeAction();
            }
            else if( CActionMgr::Instance().wasAction( rEvent, m_toggleAction, NDefs::EAP_DOWN ) )
            {
                dispatchToggleAction();
            }
            else if( m_active )
            {
                NDefs::EActionPress pressType;

                // common and can result in many messages which is why it's specifically defined here
                if( rEvent.type == SDL_MOUSEMOTION )
                {
                    // Allow the mouse move message to get eaten when action handling is disabled.
                    handleEventForTrees( rEvent );
                }
                // Need to pack multiple data items into one 32-bit int for this message
                else if( (pressType = CActionMgr::Instance().wasAction( rEvent, m_selectAction )) > NDefs::EAP_IDLE )
                {
                    // Need to pack a lot of information into one 32 bit int
                    CSelectMsgCracker msgCracker;
                    msgCracker.setPressType( pressType );
                    msgCracker.setDeviceId( CActionMgr::Instance().getLastDeviceUsed() );

                    if( msgCracker.getDeviceId() == NDefs::MOUSE )
                    {
                        msgCracker.setX( rEvent.button.x );
                        msgCracker.setY( rEvent.button.y );
                    }

                    NGenFunc::DispatchEvent( NMenuDefs::EME_MENU_SELECT_ACTION, msgCracker.getPackedUnit() );
                }
                else if( CActionMgr::Instance().wasAction( rEvent, m_backAction, NDefs::EAP_DOWN ) )
                    NGenFunc::DispatchEvent( NMenuDefs::EME_MENU_BACK_ACTION );

                else if( (pressType = CActionMgr::Instance().wasAction( rEvent, m_upAction )) > NDefs::EAP_IDLE )
                    NGenFunc::DispatchEvent( NMenuDefs::EME_MENU_UP_ACTION, pressType );

                else if( (pressType = CActionMgr::Instance().wasAction( rEvent, m_downAction )) > NDefs::EAP_IDLE )
                    NGenFunc::DispatchEvent( NMenuDefs::EME_MENU_DOWN_ACTION, pressType );

                else if( (pressType = CActionMgr::Instance().wasAction( rEvent, m_leftAction )) > NDefs::EAP_IDLE )
                    NGenFunc::DispatchEvent( NMenuDefs::EME_MENU_LEFT_ACTION, pressType );

                else if( (pressType = CActionMgr::Instance().wasAction( rEvent, m_rightAction )) > NDefs::EAP_IDLE )
                    NGenFunc::DispatchEvent( NMenuDefs::EME_MENU_RIGHT_ACTION, pressType );

                else if( (pressType = CActionMgr::Instance().wasAction( rEvent, m_tabLeft )) > NDefs::EAP_IDLE )
                    NGenFunc::DispatchEvent( NMenuDefs::EME_MENU_TAB_LEFT, pressType );

                else if( (pressType = CActionMgr::Instance().wasAction( rEvent, m_tabRight )) > NDefs::EAP_IDLE )
                    NGenFunc::DispatchEvent( NMenuDefs::EME_MENU_TAB_RIGHT, pressType );

                // If none of the predefined actions have been hit, just send the message for processing
                else
                {
                    handleEventForTrees( rEvent );
                }
            }
        }
        else if( rEvent.type > SDL_USEREVENT )
        {
            // Are we doing menu actions? May need to do some scrolling
            if( (rEvent.type >= NMenuDefs::EME_MENU_UP_ACTION) && (rEvent.type <= NMenuDefs::EME_MENU_RIGHT_ACTION) )
            {
                // Free a timer if one happens to be running
                SDL_RemoveTimer(m_scrollTimerId);
                m_scrollTimerId = 0;

                if( rEvent.user.code == NDefs::EAP_DOWN )
                    handleEventForScrolling( rEvent );
            }

            handleEventForTrees( rEvent );
        }
    }
}


/************************************************************************
*    DESC:  Dispatch the escape action
************************************************************************/
void CMenuMgr::dispatchEscapeAction()
{
    CMenuTree * pTree = getActiveTree();

    if( pTree == nullptr )
        NGenFunc::DispatchEvent( NMenuDefs::EME_MENU_ESCAPE_ACTION, 0, &m_defaultTree );
    else
        NGenFunc::DispatchEvent( NMenuDefs::EME_MENU_ESCAPE_ACTION, 0, &pTree->getName() );
}


/************************************************************************
*    DESC:  Dispatch the toggle action
************************************************************************/
void CMenuMgr::dispatchToggleAction()
{
    CMenuTree * pTree = getActiveTree();

    if( pTree == nullptr )
        NGenFunc::DispatchEvent( NMenuDefs::EME_MENU_TOGGLE_ACTION, 0, &m_defaultTree );
    else
        NGenFunc::DispatchEvent( NMenuDefs::EME_MENU_TOGGLE_ACTION, 0, &pTree->getName() );
}


/************************************************************************
*    DESC:  Handle input events depending on if this is a menu or interface tree
************************************************************************/
void CMenuMgr::handleEventForTrees( const SDL_Event & rEvent )
{
    bool menuActive(false);

    for( auto iter : m_pActiveMenuTreeVec )
    {
        // See if there's an active tree
        menuActive |= iter->isActive();

        // Even if a menu tree is not active, it needs to receive events to become active
        iter->handleEvent( rEvent );
    }

    // Only allow event handling for interface menus when regular menus are not active
    if( !menuActive )
    {
        for( auto iter : m_pActiveInterTreeVec )
        {
            if( iter->isActive() )
                iter->handleEvent( rEvent );
        }
    }
}


/************************************************************************
*    DESC:  Handle input events depending on if this is a menu or interface tree
************************************************************************/
void CMenuMgr::handleEventForScrolling( const SDL_Event & rEvent )
{
    if( m_active )
    {
        if( !handleMenuScrolling( rEvent, m_pActiveMenuTreeVec ) )
        {
            // Only allow event handling for interface menus when regular menus are not active
            handleMenuScrolling( rEvent, m_pActiveInterTreeVec );
        }
    }
}


/************************************************************************
*    DESC:  Handle input events for menu scrolling
************************************************************************/
bool CMenuMgr::handleMenuScrolling(
    const SDL_Event & rEvent, const std::vector<CMenuTree *> & activeTreeVec )
{
    bool menuActive(false);

    for( auto iter : activeTreeVec )
    {
        // See if there's an active menu
        if( iter->isActive() )
        {
            menuActive = true;

            CScrollParam & rScrollParam = iter->getScrollParam( rEvent.type );

            // If scrolling is allowed, start the timer
            if( rScrollParam.canScroll( rEvent.type ) )
            {
                m_scrollTimerId = SDL_AddTimer(rScrollParam.getStartDelay(), scrollTimerCallbackFunc, &rScrollParam);
                break;
            }
        }
    }

    return menuActive;
}


/************************************************************************
*    DESC:  Update the menu
************************************************************************/
void CMenuMgr::update()
{
    m_active = false;
    
    if( !update( m_pActiveMenuTreeVec ) )
    {
        // Only allow Updating for interface menus when regular menus are not active
        // Don't want interface menus animating when the menu is displayed
        update( m_pActiveInterTreeVec );
    }
}

bool CMenuMgr::update( const std::vector<CMenuTree *> & activeTreeVec )
{
    bool menuActive(false);
    
    for( auto iter : activeTreeVec )
    {
        // See if there's an active menu
        if( iter->isActive() )
        {
            m_active = menuActive = true;
            iter->update();
        }
    }
    
    return menuActive;
}


/************************************************************************
*    DESC:  Transform the menu
************************************************************************/
void CMenuMgr::transformInterface()
{
    if( m_active )
        transform( m_pActiveInterTreeVec );
}

void CMenuMgr::transformMenu()
{
    if( m_active )
        transform( m_pActiveMenuTreeVec );
}

void CMenuMgr::transform()
{
    if( m_active )
    {
        transform( m_pActiveInterTreeVec );
        transform( m_pActiveMenuTreeVec );
    }
}

void CMenuMgr::transform( const std::vector<CMenuTree *> & activeTreeVec )
{
    for( auto iter : activeTreeVec )
    {
        // See if there's an active menu
        if( iter->isActive() )
            iter->transform();
    }
}


/***************************************************************************
*    DESC:  Record the command buffer for all the sprite
*           objects that are to be rendered
****************************************************************************/
void CMenuMgr::recordCommandBuffer( uint32_t index )
{
    if( m_active && !m_commandBufVec.empty() )
    {
        auto cmdBuf( m_commandBufVec[index] );

        CDevice::Instance().beginCommandBuffer( index, cmdBuf );

        const CCamera & rCamera = *m_pCamera;
    
        for( auto iter : m_pActiveInterTreeVec )
            if( iter->isActive() )
                iter->recordCommandBuffer( index, cmdBuf, rCamera );
        
        for( auto iter : m_pActiveMenuTreeVec )
            if( iter->isActive() )
                iter->recordCommandBuffer( index, cmdBuf, rCamera );
        
        CDevice::Instance().endCommandBuffer( cmdBuf );
    }
}


/************************************************************************
*    DESC:  Is this menu system active?
************************************************************************/
bool CMenuMgr::isActive()
{
    return m_active;
}


/************************************************************************
*    DESC:  Is this standard menu system active?
************************************************************************/
bool CMenuMgr::isMenuActive()
{
    if( m_active )
        for( auto iter : m_pActiveMenuTreeVec )
            if( iter->isActive() )
                return true;

    return false;
}


/************************************************************************
*    DESC:  Is a menu item active
************************************************************************/
bool CMenuMgr::isMenuItemActive()
{
    bool result(false);

    if( m_active )
    {
        for( auto iter : m_pActiveMenuTreeVec )
        {
            if( iter->isActive() )
            {
                result = iter->isMenuItemActive();

                break;
            }
        }
    }

    return result;
}


/************************************************************************
*    DESC:  Is a interface item active
************************************************************************/
bool CMenuMgr::isInterfaceItemActive()
{
    bool result(false);

    if( m_active )
    {
        for( auto iter : m_pActiveInterTreeVec )
        {
            if( iter->isActive() )
            {
                result = iter->isMenuItemActive();

                break;
            }
        }
    }

    return result;
}


/************************************************************************
*    DESC:  Get reference to the menu in question
************************************************************************/
CMenu & CMenuMgr::getMenu( const std::string & nameStr )
{
    bool found(false);
    std::map<const std::string, CMenu>::iterator menuIter;

    for( auto & iter : m_menuMapMap )
    {
        menuIter = iter.second.find( nameStr );
        if( menuIter != iter.second.end() )
        {
            found = true;
            break;
        }
    }

    if( !found )
    {
        throw NExcept::CCriticalException("UIMenu Error!",
            boost::str( boost::format("Menu being asked for is missing (%s).\n\n%s\nLine: %s")
                % nameStr.c_str() % __FUNCTION__ % __LINE__ ));
    }

    return menuIter->second;
}


/************************************************************************
*    DESC:  Get reference to the first active menu.
*           NOTE: Only call this function if you are certain it will not fail
************************************************************************/
CMenu & CMenuMgr::getActiveMenu()
{
    CMenu * pMenu(nullptr);

    for( auto iter : m_pActiveMenuTreeVec )
    {
        if( iter->isActive() )
        {
            pMenu = &iter->getActiveMenu();
            break;
        }
    }

    if( pMenu == nullptr )
        throw NExcept::CCriticalException("Menu Error!",
            boost::str( boost::format("There is no active menu.\n\n%s\nLine: %s")
                % __FUNCTION__ % __LINE__ ));

    return *pMenu;
}


/************************************************************************
*    DESC:  Get a pointer to the active tree
************************************************************************/
CMenuTree * CMenuMgr::getActiveTree()
{
    CMenuTree * pTree(nullptr);

    for( auto iter : m_pActiveMenuTreeVec )
    {
        if( iter->isActive() )
        {
            pTree = iter;
            break;
        }
    }

    return pTree;
}


/************************************************************************
*    DESC:  Get the name of the default tree
************************************************************************/
const std::string & CMenuMgr::getDefaultTreeName()
{
    return m_defaultTree;
}


/************************************************************************
*    DESC:  Reset the transform
************************************************************************/
void CMenuMgr::resetTransform()
{
    for( auto & grpIter : m_menuMapMap )
        for( auto & iter : grpIter.second )
            iter.second.forceTransform();
}


/************************************************************************
*    DESC:  Reset the dynamic positions of menus
************************************************************************/
void CMenuMgr::resetDynamicOffset()
{
    for( auto & grpIter : m_menuMapMap )
        for( auto & iter : grpIter.second )
            iter.second.resetDynamicPos();
}


/************************************************************************
*    DESC:  Timer call back function for
************************************************************************/
Uint32 CMenuMgr::scrollTimerCallbackFunc( Uint32 interval, void *param )
{
    CScrollParam * pScrollParam = static_cast<CScrollParam *>(param);

    NGenFunc::DispatchEvent( pScrollParam->getMsg() );

    return pScrollParam->getScrollDelay();
}


/************************************************************************
*    DESC:  Allow message processing
************************************************************************/
void CMenuMgr::allow( bool allow )
{
    m_allow = allow;
}


/************************************************************************
*    DESC:  Set the command buffer vec
************************************************************************/
void CMenuMgr::setCommandBuffers( const std::string & cmdBufPool )
{
    m_commandBufVec = CDevice::Instance().createSecondaryCommandBuffers( cmdBufPool );
}

void CMenuMgr::setCommandBuffers( std::vector<VkCommandBuffer> & commandBufVec )
{
    m_commandBufVec = commandBufVec;
}
