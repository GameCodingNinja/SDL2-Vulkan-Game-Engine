
/************************************************************************
*    FILE NAME:       uibuttonlist.h
*
*    DESCRIPTION:     Class for user interface buttons
************************************************************************/

#pragma once

// Physical component dependency
#include <gui/uisubcontrol.h>

// Game lib dependencies
#include <utilities/bitmask.h>

class CUIButtonList : public CUISubControl
{
public:

    // Constructor
    CUIButtonList( const std::string & group );

    // Destructor
    virtual ~CUIButtonList();
    
    // Load the initial info from XML node
    void loadFromNode( const XMLNode & node ) override;

    // Set the first inactive control to be active
    bool activateFirstInactiveControl() override;

    // Get the active index
    int getActiveIndex() const override;
    void setActiveIndex( int index ) override;

    // Number of items in the list box
    size_t size() override;

protected:

    // Load the control specific info from XML node
    void loadControlFromNode( const XMLNode & node ) override;

    // Handle message
    void onStateChange( const SDL_Event & rEvent ) override;

private:
    
    // Inc/Dec control
    void inc();
    void dec();

    // Handle Action messages
    virtual void onLeftAction( const SDL_Event & rEvent ) override;
    virtual void onRightAction( const SDL_Event & rEvent ) override;
    virtual void onUpAction( const SDL_Event & rEvent ) override;
    virtual void onDownAction( const SDL_Event & rEvent ) override;

    // Handle Scroll messages
    virtual void onLeftScroll( const SDL_Event & rEvent ) override;
    virtual void onRightScroll( const SDL_Event & rEvent ) override;
    virtual void onUpScroll( const SDL_Event & rEvent ) override;
    virtual void onDownScroll( const SDL_Event & rEvent ) override;

    // Inc/Dec the list
    void incList();
    void decList();
    
    // Update the display
    void updateDisplay();

private:

    // Active index into the list
    int m_activeIndex;

    // Index of the image list
    int m_imageLstIndex;
    
    // Indicates if the control responds to up, down, left or right
    CBitmask<uint> m_ActionMask;
};
