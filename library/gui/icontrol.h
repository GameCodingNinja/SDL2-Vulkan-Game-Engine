
/************************************************************************
*    FILE NAME:       icontrol.h
*
*    DESCRIPTION:     iControl Interface class
************************************************************************/

#ifndef __i_control_h__
#define __i_control_h__

// Physical component dependency
#include <2d/object2d.h>

// Game lib dependencies
#include <gui/uicontroldefs.h>
#include <common/dynamicoffset.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>
#include <boost/signals2.hpp>

// Standard lib dependencies
#include <string>
#include <vector>

// SDL lib dependencies
#include <SDL.h>

// Vulkan lib dependencies
#include <system/vulkan.h>

// Forward declaration(s)
class CCamera;
class CSmartGuiControl;
class CSelectMsgCracker;
class CScrollParam;
struct XMLNode;

class iControl : public CObject2D, boost::noncopyable
{
public:
    
    typedef boost::signals2::signal<void (iControl *)> ExecutionActionSignal;

    // Constructor
    iControl( const std::string & group );

    // Destructor
    virtual ~iControl();
    
    // Load the initial info from XML node
    virtual void loadFromNode( const XMLNode & node );
    
    // Init control
    virtual void init() = 0;

    // Update the control
    virtual void update() = 0;
    
    // Record the command buffer for all the sprite objects that are to be rendered
    virtual void recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuf, const CCamera & camera ) = 0;
    
    // Handle events
    virtual void handleEvent( const SDL_Event & rEvent ) = 0;

    // Set the control to their default behavior
    virtual void revertToDefaultState() = 0;
    
    // Get/Set the state of this control
    virtual NUIControl::EControlState getState() const = 0;
    virtual void setState( NUIControl::EControlState state, bool setLastState = false ) = 0;

    // Get the type of action
    virtual NUIControl::EControlActionType getActionType() = 0;

    // Get the execution action
    virtual const std::string & getExecutionAction() = 0;
    
    // Reset the control to inactive if its not disabled
    virtual void reset( bool complete = false ) = 0;

    // Set/Get the Smart GUI pointer
    virtual void setSmartGui( CSmartGuiControl * pSmartGuiControl ) = 0;
    virtual CSmartGuiControl * getSmartGuiPtr() = 0;

    // Do any smart create
    virtual void smartCreate() = 0;

    // Set the type of action
    virtual void setActionType( NUIControl::EControlActionType value ) = 0;
    virtual void setActionType( const std::string & value ) = 0;

    // Set the execution action
    virtual void setExecutionAction( const std::string & action ) = 0;

    // Create the font string
    virtual void createFontString( const std::string & fontString, int spriteIndex = 0 ) = 0;
    virtual void createFontString( int stringIndex = 0, int spriteIndex = 0 ) = 0;
    
    // Set the font string
    virtual void setFontString( const std::string & fontString, int spriteIndex = 0 ) = 0;
    
    // Handle the mouse move
    virtual bool onMouseMove( const SDL_Event & rEvent ) = 0;

    // Handle the select action
    virtual bool handleSelectAction( const CSelectMsgCracker & msgCracker ) = 0;

    // Set the first inactive control to be active
    virtual bool activateFirstInactiveControl() = 0;

    // Get the pointer to the control if found
    virtual iControl * findControl( const std::string & name ) = 0;
    virtual iControl * findControl( void * pVoid ) = 0;

    // Set the string to vector
    virtual void setStringToList( const std::string & str ) = 0;

    // Change the control state
    virtual void changeState( NUIControl::EControlState state ) = 0;

    // Is this control active/selected
    virtual bool isDisabled() = 0;
    virtual bool isInactive() = 0;
    virtual bool isActive() = 0;
    virtual bool isSelected() = 0;
    virtual bool isSelectable() = 0;

    // Get the collision position
    virtual const CPoint<float> & getCollisionPos() const = 0;

    // Activate the control
    virtual bool activateControl() = 0;

    // Deactivate the control
    virtual void deactivateControl() = 0;
    
    // Disable the control
    virtual void disableControl() = 0;
    
    // Enable the control to the inactive state
    virtual void enableControl() = 0;

    // Get the scroll params
    virtual CScrollParam & getScrollParam() = 0;
    
    // Get the number of strings
    virtual const std::vector<std::string> & getStringVec() const = 0;
    
    // Check if control is a sub control
    virtual bool isSubControl() const = 0;
    
    // Connect to the execution action signal
    virtual void connect_executionAction( const ExecutionActionSignal::slot_type & slot ) = 0;
    
    // Call a script function map key for sprite
    virtual void callSpriteScriptFuncKey( const std::string & scriptFuncMapKey, bool forceUpdate = false ) = 0;
    
    // Get the pointer to the active control
    virtual iControl * getPtrToActiveControl() = 0;
    
    // Get the mouse select type
    virtual NDefs::EActionPress getMouseSelectType() const = 0;
    
    // Get the size of this control
    virtual const CSize<float> & getSize() const = 0;
    
    // an this control scroll?
    virtual bool canScroll( int msg ) = 0;
    
    // Set the script state function
    virtual void setScriptStateFunc( const std::string & scriptStateStr, const std::string & scriptFuncStr ) = 0;

    // Get the object group name
    const std::string & getGroup() const;
    
    // Get/Set the control name
    const std::string & getName() const;
    
    // Get the control type
    NUIControl::EControlType getType() const;
    
    // Load the control specific info from XML node
    virtual void loadControlFromNode( const XMLNode & node ) = 0;
    
    // Set the dynamic position
    void setDynamicPos();
    
    // Get the faction name
    const std::string & getFaction() const;
    
    // Set the alpha value of this menu
    virtual void setAlpha( float alpha ) = 0;
    float getAlpha() const;
    
    // Interface to button list
    
    // Update the display
    virtual void updateDisplay( int index ){}
    
    // Get the active index
    virtual int getActiveIndex() const { return 0; }
    
    // Interface to check box
    
    // Set or get the Select state of this control
    virtual void setToggleState( bool value ){}
    virtual bool getToggleState(){ return false; }
    
    // Interface to meter
    
    // Set the amount to the meter without banging up
    virtual void set( const double amount ){}
    
    // Start the bang range
    virtual void startBangUp( const double amount ){}
    
    // Do a fast bang
    virtual void fastBang(){}
    
    // Is the meter banging
    virtual bool isBanging(){ return false; }
    
    // Clear the meter
    virtual void clear(){}
    
    // Interface to progress bar
    
    // Set/Get current value
    virtual void setCurValue( float value ){}
    virtual float getCurValue(){ return 0.f; }
    virtual void incCurValue( float value ){}
    
    // Set/Get min value
    virtual void setMinValue( float value ){}
    virtual float getMinValue(){ return 0.f; }
    
    // Set/Get max value
    virtual void setMaxValue( float value ){}
    virtual float getMaxValue(){ return 0.f; }
    
    // Interface to slider

    // Set the slider position
    virtual void setSlider( float value = 0 ){}

    // Get the current value
    virtual float getValue(){ return 0.f; }
    
private:
    
    // Load the dynamic offset data from node
    void loadDynamicOffsetFromNode( const XMLNode & node );
    
    // Load a sprite from an XML node
    virtual void loadSpriteFromNode( const XMLNode & node, size_t & fontSpriteCount ) = 0;
    
protected:
        
    // Object data group name
    std::string m_group;
    
    // Unique string id
    std::string m_name;
    
    // The type of control
    NUIControl::EControlType m_type;
    
    // Control string list
    std::vector<std::string> m_stringVec;
    
    // A name that is applied to similar controls.
    // Provides a way to check for many controls without having to use unique names
    std::string m_faction;
    
    // Dynamic offset
    CDynamicOffset m_dynamicOffset;
    
    // menu alpha value
    float m_alpha;
};

#endif
