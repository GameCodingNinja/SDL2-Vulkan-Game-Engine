
/************************************************************************
*    FILE NAME:       menumanager.h
*
*    DESCRIPTION:     menu manager class singleton
************************************************************************/

#ifndef __menu_manager_h__
#define __menu_manager_h__

// Physical component dependency
#include <managers/managerbase.h>

// Game lib dependencies
#include <utilities/matrix.h>
#include <gui/menudefs.h>
#include <gui/menu.h>
#include <utilities/genfunc.h>

// Standard lib dependencies
#include <string>
#include <vector>
#include <map>

// Forward declaration(s)
class CMenu;
class CMenuTree;
class CUIControl;
union SDL_Event;

class CMenuMgr : public CManagerBase
{
public:
    
    // Flag that indicates the load group doesn't create the VBO, IBO, textures, etc
    static const bool DONT_INIT_GROUP = false;

    // Get the instance of the singleton class
    static CMenuMgr & Instance()
    {
        static CMenuMgr menuMgr;
        return menuMgr;
    }
    
    // Load the menu action list from XML
    void loadMenuAction( const std::string & filePath );
    
    // Load the menu group
    void loadGroup( const std::string & group, const bool doInit = true );
    
    // Free the menu group
    void freeGroup( const std::string & group );
    
    // Init a menu group
    void initGroup( const std::string & group );
    
    // Activate a menu
    void activateMenu( const std::string & group, const std::string & treeStr, const std::string & menuName );
    void activateMenu( const std::string & treeStr, const std::string & menuName );

    // Activate a tree to be used
    void activateTree( const std::string & group, const std::string & treeStr );
    void activateTree( const std::string & treeStr );
    
    // Deactivate a tree that's in use
    void deactivateTree( const std::string & group, const std::string & treeStr );
    void deactivateTree( const std::string & treeStr );

    // Clear the active trees
    void clearActiveTrees();

    // Handle input events and dispatch menu events
    void handleEvent( const SDL_Event & rEvent );

    // Update the menu
    void update();
    
    // Do the transform
    void transform();
    void transform( const CObject2D & object );
    void transformMenu();
    void transformMenu( const CObject2D & object );
    void transformInterface();
    void transformInterface( const CObject2D & object );

    // Render menus
    //void renderMenu( const CMatrix & matrix ); 
    //void renderInterface( const CMatrix & matrix );
    //void render( const CMatrix & matrix );

    // Get reference to the menu in questionn
    CMenu & getMenu( const std::string & nameStr );
    
    // Get reference to the active menu
    CMenu & getActiveMenu();
    
    // Get the pointer to the menu control
    CUIControl * getPtrToMenuControl( const std::string & menuName, const std::string & controlName );

    // Get the reference to the control in question
    template <typename Target>
    Target & getMenuControl( const std::string & menuName, const std::string & controlName );

    // Get the pointer to the active control - can return null
    template <typename Target>
    Target * getPtrToActiveControl( const std::string & menuName );

    // Is this menu system active
    bool isActive();
    
    // Is this standard menu system active?
    bool isMenuActive();

    // Is a menu item active
    bool isMenuItemActive();
    
    // Is a interface item active
    bool isInterfaceItemActive();

    // Reset the transform
    void resetTransform();

    // Reset the dynamic positions of menus
    void resetDynamicOffset();
    
    // Allow message processing
    void allow( bool allow = true );

private:
    
    // Constructor
    CMenuMgr();

    // Destructor
    ~CMenuMgr();
    
    // Load the menu info from file
    void load( const std::string & group, const std::string & filePath );

    // Load the from node
    void loadMenusFromNode( const std::string & group, const XMLNode & node );
    
    // Load the trees from node
    void loadTreesFromNode( const std::string & group, const XMLNode & node );

    // Set the active state
    void setActiveState();

    // Timer call back function
    static Uint32 scrollTimerCallbackFunc( Uint32 interval, void *param );
    
    // Handle input events depending on if this is a menu or interface tree
    void handleEventForTrees( const SDL_Event & rEvent );
    
    // Handle input events depending on if this is a menu or interface tree
    void handleEventForScrolling( const SDL_Event & rEvent );
    
    // Handle input events for menu scrolling
    bool handleMenuScrolling( 
        const SDL_Event & rEvent, const std::vector<CMenuTree *> & activeTreeVec );
    
    // Update the menu
    bool update( const std::vector<CMenuTree *> & activeTreeVec );
    
    // Transform the menu
    void transform( const std::vector<CMenuTree *> & activeTreeVec );
    void transform( const std::vector<CMenuTree *> & activeTreeVec, const CObject2D & object );
    
    // Get a pointer to the active tree
    CMenuTree * getActiveTree();

private:

    // Map map of menu trees
    std::map<const std::string, std::map<const std::string, CMenuTree> > m_menuTreeMapMap;

    // Map map of the menus
    std::map<const std::string, std::map<const std::string, CMenu> > m_menuMapMap;

    // Vector of active menu trees
    std::vector<CMenuTree *> m_pActiveMenuTreeVec;
    
    // Vector of active interface trees
    std::vector<CMenuTree *> m_pActiveInterTreeVec;

    // menu manager state
    bool m_active;

    // Actions
    std::string m_backAction;
    std::string m_toggleAction;
    std::string m_escapeAction;
    std::string m_selectAction;
    std::string m_upAction;
    std::string m_downAction;
    std::string m_leftAction;
    std::string m_rightAction;
    std::string m_tabLeft;
    std::string m_tabRight;
    std::string m_defaultTree;

    // scroll timer Id
    SDL_TimerID m_scrollTimerId;
    
    // Allow message processing
    bool m_allow;
    
};

/************************************************************************
*    desc:  Get the reference to the control in question
************************************************************************/
template <typename Target>
Target & CMenuMgr::getMenuControl( const std::string & menuName, const std::string & controlName )
{
    Target * pControl = NGenFunc::DynCast<Target>(getPtrToMenuControl(menuName, controlName));
    assert( pControl != nullptr );

    return *pControl;
}

/************************************************************************
*    desc:  Get the pointer to the active control - can return null
************************************************************************/
template <typename Target>
Target * CMenuMgr::getPtrToActiveControl( const std::string & menuName )
{
    CMenu & rMenu = getMenu(menuName);
    Target * pControl = NGenFunc::DynCast<Target>(rMenu.getPtrToActiveControl());
    assert( pControl != nullptr );

    return pControl;
}

#endif
