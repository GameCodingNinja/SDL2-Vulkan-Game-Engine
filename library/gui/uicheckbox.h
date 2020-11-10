
/************************************************************************
*    FILE NAME:       uicheckbox.h
*
*    DESCRIPTION:     Class for user interface check box buttons
************************************************************************/

#pragma once

// Physical component dependency
#include <gui/uicontrol.h>

class CUICheckBox : public CUIControl
{
public:

    // Constructor
    CUICheckBox( const std::string & group );

    // Destructor
    virtual ~CUICheckBox();

    // Set or get the Select state of this control
    void setToggleState( bool value ) override;
    bool getToggleState() override;

    // Record the command buffer for all the sprite objects that are to be rendered
    void recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuf, const CCamera & camera ) override;

protected:

    virtual void onSelectExecute( const SDL_Event & rEvent ) override;
    
private:

    // Select state
    bool m_toggleState;
};
