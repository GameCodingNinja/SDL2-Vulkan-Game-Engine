
/************************************************************************
*    FILE NAME:       uicontrol.h
*
*    DESCRIPTION:     class for user interface controls
************************************************************************/

#ifndef __ui_control_h__
#define __ui_control_h__

// Physical component dependency
#include <gui/icontrol.h>

// Game lib dependencies
#include <gui/menudefs.h>
#include <gui/scrollparam.h>
#include <gui/messagecracker.h>
#include <common/size.h>
#include <common/quad.h>
#include <common/rect.h>
#include <script/scriptcomponent.h>

// Standard lib dependencies
#include <memory>
#include <map>
#include <vector>

// Forward declaration(s)
class CSmartGuiControl;
class CObject2D;
class CSprite;

class CUIControl : public iControl
{
public:

    // Constructor
    CUIControl( const std::string & group );

    // Destructor
    virtual ~CUIControl();

    // Load the initial info from XML node
    virtual void loadFromNode( const XMLNode & node ) override;
    
    // Init control
    virtual void init() override;

    // Update the control
    virtual void update() override;

    // Transform the control
    virtual void transform( const CObject2D & object ) override;
    
    // Record the command buffer for all the sprite objects that are to be rendered
    virtual void recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuf, const CCamera & camera ) override;

    // Handle events
    virtual void handleEvent( const SDL_Event & rEvent ) override;

    // Set the control to their default behavior
    void revertToDefaultState() override;

    // Get/Set the state of this control
    NUIControl::EControlState getState() const override;
    void setState( NUIControl::EControlState state, bool setLastState = false ) override;

    // Get the type of action
    NUIControl::EControlActionType getActionType() override;

    // Get the execution action
    const std::string & getExecutionAction() override;

    // Reset the control to inactive if its not disabled
    virtual void reset( bool complete = false ) override;

    // Set/Get the Smart GUI pointer
    void setSmartGui( CSmartGuiControl * pSmartGuiControl ) override;
    CSmartGuiControl * getSmartGuiPtr() override;

    // Do any smart create
    void smartCreate() override;

    // Set the type of action
    void setActionType( NUIControl::EControlActionType value ) override;
    void setActionType( const std::string & value ) override;

    // Set the execution action
    void setExecutionAction( const std::string & action ) override;

    // Create the font string
    void createFontString( const std::string & fontString, int spriteIndex = 0 ) override;
    void createFontString( int stringIndex = 0, int spriteIndex = 0 ) override;
    
    // Set the font string
    void setFontString( const std::string & fontString, int spriteIndex = 0 ) override;

    // Handle the mouse move
    virtual bool onMouseMove( const SDL_Event & rEvent ) override;

    // Handle the select action
    virtual bool handleSelectAction( const CSelectMsgCracker & msgCracker ) override;

    // Set the first inactive control to be active
    virtual bool activateFirstInactiveControl() override;

    // Get the pointer to the control if found
    virtual iControl * findControl( const std::string & name ) override;
    virtual iControl * findControl( void * pVoid ) override;

    // Set the string to vector
    void setStringToList( const std::string & str ) override;

    // Change the control state
    void changeState( NUIControl::EControlState state ) override;

    // Is this control active/selected
    bool isDisabled() override;
    bool isInactive() override;
    bool isActive() override;
    bool isSelected() override;
    bool isSelectable() override;

    // Get the collision position
    const CPoint<float> & getCollisionPos() const override;

    // Activate the control
    bool activateControl() override;

    // Deactivate the control
    virtual void deactivateControl() override;
    
    // Disable the control
    virtual void disableControl() override;
    
    // Enable the control to the inactive state
    virtual void enableControl() override;

    // Get the scroll params
    CScrollParam & getScrollParam() override;
    
    // Get the number of strings
    const std::vector<std::string> & getStringVec() const override;
    
    // Check if control is a sub control
    virtual bool isSubControl() const override;
    
    // Connect to the execution action signal
    void connect_executionAction( const ExecutionActionSignal::slot_type & slot ) override;
    
    // Call a script function map key for sprite
    void callSpriteScriptFuncKey( const std::string & scriptFuncMapKey, bool forceUpdate = false ) override;
    
    // Set the alpha value of this menu
    virtual void setAlpha( float alpha ) override;
    
    // Get the pointer to the active control
    virtual iControl * getPtrToActiveControl() override;
    
    // Get the mouse select type
    NDefs::EActionPress getMouseSelectType() const override;
    
    // Get the size of this control
    const CSize<float> & getSize() const override;
    
    // an this control scroll?
    bool canScroll( int msg ) override;
    
    // Set the script state function
    void setScriptStateFunc( const std::string & scriptStateStr, const std::string & scriptFuncStr ) override;

protected:

    // Set the sprite's display based on it's current state
    void setDisplayState();

    // Prepare the sprite script function
    void prepareSpriteScriptFunction( NUIControl::EControlState controlState );

    // Prepare the control script function to run
    void prepareControlScriptFunction( NUIControl::EControlState controlState );

    // Recycle the contexts
    void recycleContext();

    // Smart execute the action
    void smartExecuteAction();
    
    // Do any smart event handling
    void smartHandleEvent( const SDL_Event & rEvent );

    // Handle message
    virtual void onStateChange( const SDL_Event & rEvent );
    virtual void onSelectExecute( const SDL_Event & rEvent );
    void onSetActiveControl( const SDL_Event & rEvent );
    void onReactivate( const SDL_Event & rEvent );
    void onTransIn( const SDL_Event & rEvent );
    void onTransOut( const SDL_Event & rEvent );
    
    // Load the control specific info from XML node
    virtual void loadControlFromNode( const XMLNode & node ) override;
    
    // Is the point in the control
    bool isPointInControl( int x, int y );
    bool isPointInControl( const CPoint<float> & pos );
    
    // Get the script state
    NUIControl::EControlState getScriptState( const std::string & scriptStateStr );

private:

    // Load a sprite from an XML node
    virtual void loadSpriteFromNode( const XMLNode & node, size_t & fontSpriteCount ) override;

    // Set the default state of this control
    void setDefaultState( NUIControl::EControlState value );
    void setDefaultState( const std::string & value );
    
    // Transform the collision
    void transformCollision();
    
protected:
    
    // sprite deque
    std::vector<CSprite *> m_pSpriteVec;

private:

    // Script component object
    CScriptComponent m_scriptComponent;

    // control's default state
    NUIControl::EControlState m_defaultState;

    // control's current state
    NUIControl::EControlState m_state;
    NUIControl::EControlState m_lastState;

    // Name of the action to perform under the correct circumstances
    std::string m_executionAction;

    // How the control should respond when selected
    NUIControl::EControlActionType m_actionType;

    // This control's size
    CSize<float> m_size;

    // This is the size modifier
    // when calculating the collision rect
    CRect<float> m_sizeModifier;

    // Collision rect
    CQuad m_collisionQuad;

    // Collision center
    CPoint<float> m_collisionCenter;

    // Base smart Gui control scoped pointer
    std::unique_ptr<CSmartGuiControl> m_upSmartGui;

    // Mouse selection type
    NDefs::EActionPress m_mouseSelectType;

    // On state script function names
    std::map< NUIControl::EControlState, std::string > m_scriptFunction;

    // Scrolling parameters
    CScrollParam m_scrollParam;
    
    // Boost signals
    ExecutionActionSignal m_executionActionSignal;
};

#endif
