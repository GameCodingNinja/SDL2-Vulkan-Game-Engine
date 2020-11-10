
/************************************************************************
*    FILE NAME:       menu.h
*
*    DESCRIPTION:     Class for user interface menu
************************************************************************/

#pragma once

// Physical component dependency
#include <common/object.h>

// Game lib dependencies
#include <gui/icontrolnavnode.h>
#include <gui/icontrol.h>
#include <gui/scrollparam.h>
#include <common/dynamicoffset.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <string>
#include <vector>
#include <map>
#include <memory>

// Forward declaration(s)
class CSprite;
union SDL_Event;

class CMenu : public CObject, boost::noncopyable
{
public:

    // Define the navigation helper map
    typedef std::map<const std::string, iControlNavNode *> NavHelperMap;

    CMenu( const std::string & name, const std::string & group );
    virtual ~CMenu();

    // Load the menu info from XML file
    void load( const std::string & filePath );
    
    // Init the menu
    void init();

    // Init the root menu
    void initRootMenu();

    // Update the menu
    void update();

    // Transform the menu
    void transform() override;
    void transform( const CObject & object ) override;
    
    // Record the command buffer for all the sprite objects that are to be rendered
    void recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuf, const CCamera & camera );

    // Get the name of the menu
    const std::string & getName() const;

    // Handle events
    void handleEvent( const SDL_Event & rEvent );

    // Reset all controls
    void reset();

    // Get the pointer to the control in question
    iControl * getPtrToControl( const std::string & nameStr );

    // Get the pointer to the active control
    iControl * getPtrToActiveControl();

    // Load the dynamic offset data from node
    void loadDynamicOffsetFromNode( const XMLNode & node );

    // Set the dynamic position
    void resetDynamicPos();

    // Does this menu use dynamic offsets
    bool isDynamicOffset();

    // Get the scroll params
    CScrollParam & getScrollParam( int msg );
    
    // Is the menu idle
    bool isIdle();
    
    // Is the menu active
    bool isActive();
    
    // Set/Get the alpha value of this menu
    void setAlpha( float alpha );
    float getAlpha();

    // Set the first inactive control to be active
    void activateFirstInactiveControl();

    // Deactivate all controls
    void deactivateAllControls();

private:
    
    // Set the dynamic position
    void setDynamicPos();
    
    // Load the script functions from node and add them to the map
    void loadScriptFromNode( const XMLNode & node );

    // Load a static sprite from an XML node
    void loadStaticSpriteFromNode( const XMLNode & node );

    // Load static controls from an XML node
    void loadStaticControlFromNode( XMLNode & node );

    // Load mouse only controls from an XML node
    void loadMouseOnlyControlFromNode( const XMLNode & node );

    // Load a control from an XML node
    void loadControlFromNode( XMLNode & node, NavHelperMap & navNodeMap );

    // Find the reference nodes
    void findNodes(
        const XMLNode & node,
        int nodeIndex,
        NavHelperMap & navNodeMap );

    // Find the reference nodes
    void setNodes(
        const XMLNode & node,
        int nodeIndex,
        std::string attr,
        EAction action,
        NavHelperMap & navNodeMap );

    // Navigate the menu
    void navigateMenu( EAction action );

    // Message handlers
    void onUpAction( const SDL_Event & rEvent );
    void onDownAction( const SDL_Event & rEvent );
    void onLeftAction( const SDL_Event & rEvent );
    void onRightAction( const SDL_Event & rEvent );
    void onSelectAction( const SDL_Event & rEvent );
    void onSetActiveControl( const SDL_Event & rEvent );
    void onReactivate( const SDL_Event & rEvent );
    void onRootTransIn( const SDL_Event & rEvent );
    void onTransIn( const SDL_Event & rEvent );
    void onTransOut( const SDL_Event & rEvent );
    void onMouseMove( const SDL_Event & rEvent );
    
    // Prepare the script function to run
    bool prepare( const std::string & scriptFuncId, uint type = 0, int code = 0 );

private:

    // This menu's name
    std::string m_name;

    // Group name
    std::string m_group;

    // Deque of menu static sprites
    std::vector<CSprite *> m_pSpriteVec;

    // Vector list of static controls
    std::vector<iControl *> m_pStaticControlVec;

    // Vector list of mouse only controls
    std::vector<iControl *> m_pMouseOnlyControlVec;

    // Vector list of controls
    std::vector<iControl *> m_pControlVec;

    // Vector list of navigation nodes
    std::vector<iControlNavNode *> m_pControlNodeVec;

    // Map container of controls for easy name access
    // NOTE: This container does not own it's pointers.
    std::map<const std::string, iControl *> m_pControlMap;

    // Current active node
    iControlNavNode * m_pActiveNode;

    // menu state
    EMenuState m_state;

    // Dynamic offset
    CDynamicOffset m_dynamicOffset;

    // Scrolling parameters
    CScrollParam m_scrollParam;
    
    // menu alpha value
    float m_alpha;
};
