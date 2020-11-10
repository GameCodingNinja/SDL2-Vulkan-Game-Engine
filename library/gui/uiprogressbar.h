
/************************************************************************
*    FILE NAME:       uiprogressbar.h
*
*    DESCRIPTION:     Class for user interface progress bar
************************************************************************/

#pragma once

// Physical component dependency
#include <gui/uicontrol.h>

// Game lib dependencies
#include <common/defs.h>
#include <common/size.h>

// Standard lib dependencies
#include <memory>

class CUIProgressBar : public CUIControl
{
public:

    // Constructor
    CUIProgressBar( const std::string & group );

    // Destructor
    virtual ~CUIProgressBar();
    
    // Load the initial info from XML node
    void loadFromNode( const XMLNode & node ) override;
    
    // Load the control specific info from XML node
    void loadControlFromNode( const XMLNode & controlNode ) override;
    
    // Transform the control
    void transform( const CObject & object ) override;
    
    // Record the command buffer for all the sprite objects that are to be rendered
    void recordCommandBuffer( uint32_t index, VkCommandBuffer cmdBuf, const CCamera & camera ) override;
    
    // Set/Get current value
    void setProgressBarValue( float value ) override;
    float getProgressBarValue() override;
    void incProgressBarValue( float value ) override;
    
    // Set/Get min value
    void setProgressBarMinValue( float value ) override;
    float getProgressBarMinValue() override;
    
    // Set/Get max value
    void setProgressBarMaxValue( float value ) override;
    float getProgressBarMaxValue() override;
    
private:
    
    // Calculate the progress bar size and position
    void setSizePos();
    
private:
    
    // stencil mask sprite
    std::unique_ptr<CSprite> m_upStencilMaskSprite;
    
    // current value of progress bar
    float m_curValue;
    
    // Minimum value
    float m_minValue;

    // Max value of progress bar
    float m_maxValue;
    
    // Sprite index to apply stencil mask to
    int m_spriteApplyIndex;
    
    // Orentation
    EOrientation m_orentation;
    
    // progress bar size
    CSize<float> m_size;
    
    // progress bar pos
    CPoint<float> m_pos;
    
    // progress bar scale
    CPoint<float> m_scale;
    
    // alignment
    union UAlignment
    {
        UAlignment() : horz(EHorzAlignment::HORZ_LEFT){}
        EHorzAlignment horz;
        EVertAlignment vert;
    } m_alignment;
};
