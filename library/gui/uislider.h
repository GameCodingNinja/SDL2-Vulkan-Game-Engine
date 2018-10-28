
/************************************************************************
*    FILE NAME:       uislider.h
*
*    DESCRIPTION:     Class for user interface slider
************************************************************************/

#ifndef __ui_slider_h__
#define __ui_slider_h__

// Physical component dependency
#include <gui/uisubcontrol.h>

class CUISlider : public CUISubControl
{
public:

    enum EOrientation
    {
        EO_HORIZONTAL,
        EO_VERTICAL
    };
    
    // Constructor
    CUISlider( const std::string & group );

    // Destructor
    virtual ~CUISlider();

    // Load the initial info from XML node
    virtual void loadFromNode( const XMLNode & node ) override;
    
    // Init control
    virtual void init() override;

    // Set the slider min value
    void setMinValue( float value );

    // Set the slider max value
    void setMaxValue( float value );

    // Set the slider position
    void setSlider( float value = 0 );

    // Set the slider inc value
    void incSlider( float value = 0 );

    // Get the current value
    float getValue();
    
    // Is the mouse down
    bool isMouseDown();
    
    // Deactivate the control
    virtual void deactivateControl() override;
    
    // Handle the mouse move
    virtual bool onMouseMove( const SDL_Event & rEvent ) override;

protected:

    // Load the control info from XML node
    virtual void loadControlFromNode( const XMLNode & node ) override;

    // Handle the select action
    bool handleSelectAction( const CSelectMsgCracker & msgCracker ) override;

private:

    // Update the slider
    void updateSlider();

    // Cap the slider value
    void capSliderValue();

    // Set the position of the slider
    void setSliderPos();

    // Inc/Set the slider based on mouse movement
    void incSliderMovePos( float value );

    // Handle Action messages
    virtual void onLeftAction( const SDL_Event & rEvent ) override;
    virtual void onRightAction( const SDL_Event & rEvent ) override;

    // Handle Scroll messages
    virtual void onLeftScroll( const SDL_Event & rEvent ) override;
    virtual void onRightScroll( const SDL_Event & rEvent ) override;

    // Handle the slider change
    void handleSliderChange( float value, bool prepareOnSelect = false );

private:

    // Slider travel distance in pixels
    float m_travelDistPixels;

    // Slider Orientation
    EOrientation m_orientation;

    // Min value
    float m_minValue;

    // Max value
    float m_maxValue;

    // Current value
    float m_curValue;

    // inc value
    float m_incValue;

    // Flag to indicate to display the value as an int
    bool m_displayValueAsInt;

    // Default position of the slider button
    CPoint<float> m_defaultPos;

    // slider button hold flag
    bool m_sliderBtnHold;

    // The current press type
    NDefs::EActionPress m_pressType;
};

#endif
