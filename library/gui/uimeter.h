/************************************************************************
*    FILE NAME:       uimeter.h
*
*    DESCRIPTION:     Class for user interface meters
************************************************************************/

#pragma once

// Physical component dependency
#include <gui/uicontrol.h>

// Game lib dependencies
#include <common/defs.h>
#include <common/size.h>
#include <common/point.h>
#include <utilities/timer.h>

// Standard lib dependencies
#include <string>
#include <map>

// Forward declaration(s)
class CSprite;

class CUIMeter : public CUIControl
{
public:
    
    // Constructor
    CUIMeter( const std::string & group );

    // Destructor
    virtual ~CUIMeter();
    
    // Load the initial info from XML node
    void loadFromNode( const XMLNode & node ) override;

    // Init control
    void init() override;
    
    // Update the control
    void update() override;
    
    // Set the amount to the meter without banging up
    void setMeterValue( const double amount ) override;
    
    // Start the bang range
    void startBangUp( const double amount ) override;

    // Increment the bang up
    void incBangUp( const double amount ) override;
    
    // Do a fast bang
    void fastBang() override;
    
    // Is the meter banging
    bool isBanging() override;
    
    // Clear the meter
    void clearMeter() override;
    
protected:

    // Load the control specific info from XML node
    virtual void loadControlFromNode( const XMLNode & node ) override;
    
    // Display the value in the meter
    void displayValue();
    
private:
    
    enum EBangType
    {
        RAMP_UP,
        LINEAR,
        HYBRID
    };
    
    enum EScaleType
    {
        AXIS,
        ACCURATE
    };
    
    class CBangRange
    {
    public:
        CBangRange( int target, EBangType bangType, double velocity, double estimatedTime, double slowStartTime ) :
            m_target(target),
            m_bangType(bangType),
            m_velocity(velocity),
            m_estimatedTime(estimatedTime),
            m_slowStartTime(slowStartTime)
        {}
            
        CBangRange() {}
            
    public:
        
        int m_target;
        EBangType m_bangType;
        double m_velocity;
        double m_estimatedTime;
        double m_slowStartTime;
    };
    
    // Set the bang range
    void setBangRange();
    
    // Init the bang range
    void initBangRange( const CBangRange & bangRange );
    
private:
        
    // Current value
    double m_currentValue;
    
    // Target value
    double m_targetValue;
    
    // Velocity value
    double m_velocity;
    
    // Terminal velocity value
    double m_terminalVelocity;
    
    // Acceleration value
    double m_acceleration;
    
    // Impulse value
    double m_impulse;
    
    // last value
    double m_lastValue;
    
    // fast bang time amount
    double m_fastBangTime;
    
    // bang up flag
    bool m_bangUp;
    
    // spin timer
    CTimer m_startUpTimer;
    
    // Current bang range
    CBangRange m_bangRange;
    
    // Sprite pointer
    // We DO NOT own this pointer. Don't free it!
    CSprite * m_pSprite;
    
    // Bang range value
    std::vector<CBangRange> m_bangRangeVec;
    
    // Max Font string size for this meter
    CSize<float> m_maxFontStrSize;
    
    // The amount to scale the font by to fit within the meter
    CSize<float> m_bangScaleAdjustment;
    
    // Scale on axis or accurate
    EScaleType m_scaleType;
};
