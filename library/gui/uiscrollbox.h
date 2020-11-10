
/************************************************************************
*    FILE NAME:       uiscrollbox.h
*
*    DESCRIPTION:     Class for user interface scroll boxes
************************************************************************/

#pragma once

// Physical component dependency
#include <gui/uisubcontrol.h>

// Game lib dependencies
#include <common/point.h>
#include <common/defs.h>
#include <utilities/bitmask.h>

// Standard lib dependencies
#include <vector>
#include <memory>

// Forward declaration(s)
class CSprite;

class CUIScrollBox : public CUISubControl
{
public:

    // Constructor
    CUIScrollBox( const std::string & group );

    // Destructor
    virtual ~CUIScrollBox();
    
    // Load the initial info from XML node
    void loadFromNode( const XMLNode & node ) override;
    
    // Init control
    void init() override;

    // Update the control
    void update() override;
    
    // Handle events
    void handleEvent( const SDL_Event & rEvent ) override;

    // Transform the control
    void transform( const CObject & object ) override;
    
    // Record the command buffer for all the sprite objects that are to be rendered
    void recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuf, const CCamera & camera ) override;

    // Set the first inactive control to be active
    bool activateFirstInactiveControl() override;
    
    // Handle the mouse move
    bool onMouseMove( const SDL_Event & rEvent ) override;
    
    // Only deactivate sub controls
    void deactivateControl() override;

    // Get the scroll control vector
    const std::vector<iControl *> & getScrollCtrlVec();
    
    // Set the alpha value of this menu
    void setAlpha( float alpha ) override;
    
    // Get the pointer to the active control
    iControl * getPtrToActiveControl() override;

    // Number of controls in the list box
    size_t size() override;

    // Get the pointer to the scroll box control
    iControl * getPtrToScrollBoxControl( uint index ) override;

protected:

    // Load the control specific info from XML node
    virtual void loadControlFromNode( const XMLNode & node ) override;

    // Add the scroll control from node
    iControl * addScrollControlFromNode( const XMLNode & node );
    
    // Handle Action messages
    void onUpAction( const SDL_Event & rEvent ) override;
    void onDownAction( const SDL_Event & rEvent ) override;
    
    // Handle Scroll messages
    void onUpScroll( const SDL_Event & rEvent ) override;
    void onDownScroll( const SDL_Event & rEvent ) override;
    
    // Handle Tab messages
    void onTabLeft( const SDL_Event & rEvent ) override;
    void onTabRight( const SDL_Event & rEvent ) override;
    
    // Handle the select action
    bool handleSelectAction( const CSelectMsgCracker & msgCracker ) override;
    
    // Handle the keyboard/Gamepad scrolling
    void handleKeyboardGamepadScroll( int scrollVector );
    
    // Handle the page scrolling
    void handlePageScroll( int scrollVector );
    
    // Handle the sub control mouse move
    bool onSubControlMouseMove( const SDL_Event & rEvent ) override;
    
    // Get the pointer to the subcontrol if found
    iControl * findSubControl( const std::string & name ) override;
    
    // Find the sub control via is pointer
    iControl * findSubControl( void * pVoid ) override;
    
    // Deactivate the sub control
    void deactivateSubControl() override;
    
    // Set the start and end positions
    void setStartEndPos();
    
    // Reposition the scroll controls
    void repositionScrollControls();

private:
    
    // Handle the time based Scrolling of the contents of the scroll box
    void handleScrollUpdate();
    
    // Align the scroll box to it's proper stopping point
    void alignScrollPostion();
    
    // Select the next control
    CBitmask<uint> selectNextControl( int scrollVector );
    
    // Select the paged control
    void selectPagedControl( int scrollVector );
    
    // Set the active control to the viewable area
    bool setActiveCtrlToViewableArea( int scrollVector );
    
    // Scroll to the next control in the viewable area
    CBitmask<uint> scrollToTheNextCtrlInViewableArea( int scrollVector );
    
    // Is the scroll index in view
    bool inView( int scrollIndex, int scrollVector );
    
    // Init the variables that scroll the contents of the scroll box
    void initScrolling( int scrollVector, float distance, bool endScrollSelection = true, bool paging = false );
    
    // Get the fractional amount the controls are off within the scroll box
    float getControlAlignment();
    
    // See if we can activate this scroll control
    bool activateScrollCtrl( int scrollControlIndex );
    
    // Do we need to select and reposition the control
    bool selectAndRepositionCtrl( int scrollVector );
    
private:
    
    const uint IN_VIEWABLE_AREA;
    const uint NEW_ACTIVE_CTRL;

    // Vector list of controls in scroll box
    std::vector<iControl *> m_pScrollControlVec;

    // Initial scroll box control offset
    CPoint<float> m_initialOffset;

    // Height to cull
    float m_cullHeight;

    // height of control
    uint m_controlHeight;
    
    // Scroll move counter
    float m_scrollCurPos;

    // Number of controls visible in scroll box
    int m_visibleCount;
    
    // Visible start pos
    int m_visStartPos;
    int m_visEndPos;

    // Max scroll amount
    float m_maxMoveAmount;

    // stencil mask sprite
    std::unique_ptr<CSprite> m_upStencilMaskSprite;
    
    // Active scroll control index in this control
    int m_activeScrollCtrl;
    
    // index of first control in scroll box
    int m_firstScrollCtrlIndex;
    
    // Default offsets
    std::vector<float> m_defaultOffsetVec;
    
    // speed members
    float m_scrollSpeed;
    float m_pageSpeed;
    
    // Scroll vector to indicate the control is scrolling
    int m_scrollVector;
    
    // Flag to indicate the control is paging
    bool m_paging;
    
    // Scroll counter
    float m_scrollCounter;

    // Scroll distance
    float m_scrollDistance;
    
    // Flag to indicate scrolling needs to stop
    // but allows the scrolling to finish
    bool m_endScroll;
    
    // Flag to indicate that the scroll message has been sent
    bool m_scrollMsg;
    
    // Flag to allow for end scroll selection
    bool m_endScrollSelection;
};
