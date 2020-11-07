
/************************************************************************
*    FILE NAME:       icontrol.h
*
*    DESCRIPTION:     iControl Interface class
************************************************************************/

#pragma once

// Physical component dependency
#include <common/object.h>

// Game lib dependencies
#include <gui/uidefs.h>
#include <common/dynamicoffset.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <string>
#include <vector>

// Vulkan lib dependencies
#include <system/vulkan.h>

// Forward declaration(s)
union SDL_Event;
class CCamera;
class CSelectMsgCracker;
class CScrollParam;
struct XMLNode;

class iControl : public CObject, boost::noncopyable
{
public:

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
    virtual EControlState getState() const = 0;
    virtual void setState( EControlState state, bool setLastState = false ) = 0;

    // Get the type of action
    virtual EControlActionType getActionType() = 0;

    // Get the execution action
    virtual const std::string & getExecutionAction() = 0;
    
    // Reset the control to inactive if its not disabled
    virtual void reset( bool complete = false ) = 0;

    // Set the type of action
    virtual void setActionType( EControlActionType value ) = 0;
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
    virtual void changeState( EControlState state ) = 0;

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
    
    // Get the pointer to the active control
    virtual iControl * getPtrToActiveControl() = 0;
    
    // Get the mouse select type
    virtual EActionPress getMouseSelectType() const = 0;
    
    // Get the size of this control
    virtual const CSize<float> & getSize() const = 0;
    
    // an this control scroll?
    virtual bool canScroll( int msg ) = 0;

    // Get the object group name
    const std::string & getGroup() const;
    
    // Get/Set the control name
    const std::string & getName() const;
    
    // Get the control type
    EControlType getType() const;
    
    // Load the control specific info from XML node
    virtual void loadControlFromNode( const XMLNode & node ) = 0;
    
    // Set the dynamic position
    void setDynamicPos();
    
    // Get the faction name
    const std::string & getFaction() const;
    
    // Set the alpha value of this menu
    virtual void setAlpha( float alpha ) = 0;
    float getAlpha() const;

    // Prepare the script Id function to run
    virtual bool prepare( const std::string & scriptFuncId ) = 0;

    // Interface sub controls

    // Number of sub controls
    virtual size_t size() { return 0; }
    
    // Interface to button list
    
    // Get the active index
    virtual int getActiveIndex() const { return 0; }
    virtual void setActiveIndex( int index ){}
    
    // Interface to check box
    
    // Set or get the Select state of this control
    virtual bool getToggleState(){ return false; }
    virtual void setToggleState( bool value ){}
    
    // Interface to meter
    
    // Set the amount to the meter without banging up
    virtual void setMeterValue( const double amount ){}
    
    // Start the bang range
    virtual void startBangUp( const double amount ){}

    // Increment the bang up
    virtual void incBangUp( const double amount ){};
    
    // Do a fast bang
    virtual void fastBang(){}
    
    // Is the meter banging
    virtual bool isBanging(){ return false; }
    
    // Clear the meter
    virtual void clearMeter(){}
    
    // Interface to progress bar
    
    // Set/Get current value
    virtual void setProgressBarValue( float value ){}
    virtual float getProgressBarValue(){ return 0.f; }
    virtual void incProgressBarValue( float value ){}
    
    // Set/Get min value
    virtual void setProgressBarMinValue( float value ){}
    virtual float getProgressBarMinValue(){ return 0.f; }
    
    // Set/Get max value
    virtual void setProgressBarMaxValue( float value ){}
    virtual float getProgressBarMaxValue(){ return 0.f; }
    
    // Interface to slider

    // Set/Get the slider position
    virtual void setSliderPosValue( float value = 0 ){}
    virtual float getSliderPosValue(){ return 0.f; }

    // Inc/Set the slider based on mouse movement
    virtual float incSliderMovePos( float value ){ return 0.f; };
    
    // Set/Get the slider min value
    virtual void setSliderMinValue( float value ){}
    virtual void setSliderMaxValue( float value ){}
    
    // Is the mouse down
    virtual bool isSliderMouseDown(){ return false; }

    // Interface to scroll box

    // Get the pointer to the scroll box control
    virtual iControl * getPtrToScrollBoxControl( uint index ){ return nullptr; }

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
    EControlType m_type;
    
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
