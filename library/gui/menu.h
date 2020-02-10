
/************************************************************************
*    FILE NAME:       menu.h
*
*    DESCRIPTION:     Class for user interface menu
************************************************************************/

#ifndef __menu_h__
#define __menu_h__

// Physical component dependency
#include <common/objecttransform.h>

// Game lib dependencies
#include <gui/menudefs.h>
#include <gui/icontrolnavnode.h>
#include <gui/icontrol.h>
#include <gui/scrollparam.h>
#include <script/scriptcomponent.h>
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

class CMenu : public CObjectTransform, boost::noncopyable
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

    // Activate this menu because it's probably a root menu
    void activateMenu();

    // Update the menu
    void update();

    // Transform the menu
    void transform() override;
    void transform( const CObjectTransform & object ) override;
    
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

private:
    
    // Set the dynamic position
    void setDynamicPos();
    
    // Init the script functions
    void initScriptFunctions( const XMLNode & node );

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
        iControlNavNode::ENavNode navNode,
        NavHelperMap & navNodeMap );

    // Navigate the menu
    void navigateMenu( iControlNavNode::ENavNode navNodeAction );

    // Message handlers
    void onUpAction( const SDL_Event & rEvent );
    void onDownAction( const SDL_Event & rEvent );
    void onLeftAction( const SDL_Event & rEvent );
    void onRightAction( const SDL_Event & rEvent );
    void onSelectAction( const SDL_Event & rEvent );
    void onSetActiveControl( const SDL_Event & rEvent );
    void onReactivate( const SDL_Event & rEvent );
    void onTransIn( const SDL_Event & rEvent );
    void onTransOut( const SDL_Event & rEvent );
    void onMouseMove( const SDL_Event & rEvent );
    
    // Prepare the script function to run
    void prepare( const std::string & scriptFuncId, uint type = 0, int code = 0 );

    // Set the first inactive control to be active
    void activateFirstInactiveControl();

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
    NMenuDefs::EMenuState m_state;

    // Dynamic offset
    CDynamicOffset m_dynamicOffset;

    // Scrolling parameters
    CScrollParam m_scrollParam;
    
    // The script conponent
    CScriptComponent m_scriptComponent;
    
    // Script function map. Tie events to script functions
    std::map<const std::string, std::tuple<std::string, std::string, bool>> m_scriptFunctionMap;
    
    // menu alpha value
    float m_alpha;
};

#endif
