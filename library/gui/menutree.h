
/************************************************************************
*    FILE NAME:       menutree.cpp
*
*    DESCRIPTION:     Class that hold a tree of menus
************************************************************************/

#pragma once

// Game lib dependencies
#include <gui/uidefs.h>
#include <gui/scrollparam.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <string>
#include <vector>
#include <map>

// Vulkan lib dependencies
#include <system/vulkan.h>

// SDL lib dependencies
#include <SDL3/SDL.h>

// Forward declaration(s)
class CObject;
class CMenu;
class CCamera;

class CMenuTree : public boost::noncopyable
{
public:

    // Constructor
    CMenuTree(
        const std::string & name,
        std::map<const std::string, CMenu> & rMenuMap, 
        const std::string & rootMenu,
        const std::string & defaultMenu,
        bool interfaceTree = false );

    // Destructor
    virtual ~CMenuTree();

    // Handle events
    void handleEvent( const SDL_Event & rEvent );
    
    // Activate a menu
    void activateMenu( const std::string & menuName );

    // Update the menu tree
    void update();

    // Transform the menu tree
    void transform();
    void transform( const CObject & object );
    
    // Record the command buffer for all the sprite objects that are to be rendered
    void recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuf, const CCamera & camera );

    // Is a tree active
    bool isActive();

    // Does this tee have a root menu
    bool hasRootMenu();

    // Init the tree for use
    void init();

    // Get the active menu
    CMenu & getActiveMenu();

    // Get the scroll param data
    CScrollParam & getScrollParam( int msg );

    // Is a menu item active
    bool isMenuItemActive();
    
    // Is this an interface tree
    bool isInterfaceTree() const;
    
    // Get the name of the tree
    std::string & getName();

    // Transition the menu
    void transitionMenu();

private:

    // Handle message
    void onEscape( const SDL_Event & rEvent );
    void onToggle( const SDL_Event & rEvent );
    void onBack( const SDL_Event & rEvent );
    void onToTree( const SDL_Event & rEvent );
    void onToMenu( const SDL_Event & rEvent );
    void onTransOut( const SDL_Event & rEvent );
    void onTransIn( const SDL_Event & rEvent );

private:
    
    // Name of the tree
    std::string m_name;

    // Map of the menus
    std::map<const std::string, CMenu> & m_rMenuMap;
    
    // Pointer to the root menu
    // Do not free. We DON'T own the pointer
    CMenu * m_pRootMenu;

    // Pointer to the default menu
    // Do not free. We DON'T own the pointer
    CMenu * m_pDefaultMenu;
    
    // Is interface tree?
    const bool m_interfaceTree;

    // Name of menu we are transitioning to
    std::string m_toMenu;

    // Vector of the path taken through the menu
    // Do not free. We DON'T own the pointers
    std::vector<CMenu *> m_pMenuPathVec;

    // menu state
    EMenuState m_state;
};
