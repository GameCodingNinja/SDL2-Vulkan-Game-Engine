/************************************************************************
*    FILE NAME:       uimeter.cpp
*
*    DESCRIPTION:     Class for user interface meters
************************************************************************/

// Physical component dependency
#include <gui/uimeter.h>

// Game lib dependencies
#include <utilities/xmlParser.h>
#include <utilities/xmlparsehelper.h>
#include <utilities/highresolutiontimer.h>
#include <utilities/exceptionhandling.h>
#include <common/ivisualcomponent.h>
#include <sprite/sprite.h>

// Boost lib dependencies
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

// Standard lib dependencies
#include <map>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CUIMeter::CUIMeter( const std::string & group ) :
    CUIControl( group ),
    m_currentValue(0.0),
    m_targetValue(0),
    m_velocity(0.0),
    m_acceleration(0.0),
    m_impulse(0.0),
    m_lastValue(0),
    m_fastBangTime(0.0),
    m_bangUp(false),
    m_pSprite(nullptr),
    m_scaleType(AXIS)
{
    m_type = EControlType::METER;
}

/************************************************************************
*    DESC:  Destructor
************************************************************************/
CUIMeter::~CUIMeter()
{
}

/************************************************************************
*    DESC:  Load the control info from XML node
*
*    param: node - XML node
************************************************************************/
void CUIMeter::loadFromNode( const XMLNode & node )
{
    CUIControl::loadFromNode( node );

    // Get the bang range info
    const XMLNode bangRangeNode = node.getChildNode( "bangRange" );
    if( !bangRangeNode.isEmpty() )
    {
        std::map<std::string, EBangType> bangTypeMap
            {{"rampUp", RAMP_UP}, {"linear", LINEAR}, {"hybrid", HYBRID}};

        m_bangRangeVec.reserve( bangRangeNode.nChildNode() );

        // Get the fast bang time
        m_fastBangTime = std::atoi(bangRangeNode.getAttribute( "fastBangTime" ));

        // Set the scale type - How the font is scaled to fit within the meter
        if( std::strcmp( bangRangeNode.getAttribute("scaleType"), "accurate" ) == 0 )
            m_scaleType = ACCURATE;

        for( int i = 0; i < bangRangeNode.nChildNode(); ++i )
        {
            const XMLNode rangeNode = bangRangeNode.getChildNode(i);

            m_bangRangeVec.emplace_back(
                std::atoi(rangeNode.getAttribute( "target" )),
                bangTypeMap[ rangeNode.getAttribute( "bangUpType" ) ],
                std::atof(rangeNode.getAttribute( "velocity" )),
                std::atof(rangeNode.getAttribute( "estimatedTime" )),
                std::atof(rangeNode.getAttribute( "slowStartTime" )) );
        }
    }

    // Get the max size of the font string to fit within this meter.
    // As the string get's bigger, it will be scaled to fit.
    m_maxFontStrSize = NParseHelper::LoadSize( node );
}

/************************************************************************
*    DESC:  Load the control specific info from XML node
************************************************************************/
void CUIMeter::loadControlFromNode( const XMLNode & controlNode )
{
    // Call the parent
    CUIControl::loadControlFromNode( controlNode );

    // Find the sprite that renders the font
    // NOTE: Should always be the last font sprite in the sprite list
    for( auto iter : m_pSpriteVec )
        if( iter->getVisualComponent()->isFontSprite() )
            m_pSprite = iter;

    if( m_pSprite == nullptr )
        throw NExcept::CCriticalException("UI Meter Init Error!",
            boost::str( boost::format("UI Meter doesn't have a sprite for rendering a font string (%s).\n\n%s\nLine: %s")
                % m_name % __FUNCTION__ % __LINE__ ));
}

/************************************************************************
*    DESC:  Init the control
************************************************************************/
void CUIMeter::init()
{
    CUIControl::init();

    // If the font that displays the meter value has no scripts, display the current value
    if( !m_pSprite->hasScriptFunctions() )
        displayValue();
}

/************************************************************************
*    DESC:  Set the amount to the meter without banging up
************************************************************************/
void CUIMeter::setMeterValue( const double amount )
{
    if( ((int64_t)amount != (int64_t)m_currentValue) )
    {
        m_lastValue = m_currentValue;
        m_currentValue = m_targetValue = amount;

        // Display the value in the meter
        displayValue();
    }
}

/************************************************************************
*    DESC:  Start the bang range
************************************************************************/
void CUIMeter::startBangUp( const double amount )
{
    if( (int64_t)amount != (int64_t)m_currentValue )
    {
        m_targetValue = amount;
        m_bangUp = true;

        setBangRange();
    }
}

/************************************************************************
*    DESC:  Increment the bang up
************************************************************************/
void CUIMeter::incBangUp( const double amount )
{
    m_targetValue += amount;
    m_bangUp = true;

    setBangRange();
}

/************************************************************************
*    DESC:  Set the bang range
************************************************************************/
void CUIMeter::setBangRange()
{
    bool found(false);

    for( auto & iter : m_bangRangeVec )
    {
        if( (int64_t)(m_targetValue - m_currentValue) <= iter.m_target )
        {
            found = true;
            initBangRange( iter );
            break;
        }
    }

    if( !found )
        initBangRange( m_bangRangeVec.back() );
}

/************************************************************************
*    DESC:  Init the bang range
************************************************************************/
void CUIMeter::initBangRange( const CBangRange & bangRange )
{
    m_bangRange = bangRange;
    m_terminalVelocity = 0.0;
    m_acceleration = 0.0;
    m_impulse = 0.0;
    m_bangScaleAdjustment.set(1,1);

    m_pSprite->setScale( CPoint<float>(1,1,1) );

    m_velocity = bangRange.m_velocity / 1000.0;

    double range( m_targetValue - m_currentValue );

    // Ramp up from start to finish
    if( bangRange.m_bangType == RAMP_UP )
    {
        m_impulse = range / (bangRange.m_estimatedTime * bangRange.m_estimatedTime * 1000.0);
        m_acceleration = m_impulse;
    }
    // Linear bang up from the start
    else if( bangRange.m_bangType == LINEAR )
    {
        m_acceleration = range / (bangRange.m_estimatedTime * 1000.0);
    }
    // combination of ramp up and linear
    else if( bangRange.m_bangType == HYBRID )
    {
        m_terminalVelocity = range / (bangRange.m_estimatedTime * 1000.0);
        m_impulse = range / (bangRange.m_estimatedTime * bangRange.m_estimatedTime * 500.0);
        m_acceleration = m_impulse;
    }

    // Set the timer to allow the bang-up to start off slowly
    m_startUpTimer.set( bangRange.m_slowStartTime );

    // Prepare the start script function if one exists
    m_pSprite->prepare( "start" );
}

/************************************************************************
*    DESC:  Do a fast bang
************************************************************************/
void CUIMeter::fastBang()
{
    if( m_bangUp )
    {
        double acceleration = (m_targetValue - m_currentValue) / m_fastBangTime;

        // use the fast bang acceleration if the current one is less
        if( m_acceleration < acceleration )
            m_acceleration = acceleration;
    }
}

/************************************************************************
*    DESC:  Update the control
************************************************************************/
void CUIMeter::update()
{
    CUIControl::update();

    if( m_bangUp )
    {
        const double elapsedTime = CHighResTimer::Instance().getElapsedTime();

        // Ramp up from start to finish
        if( m_bangRange.m_bangType == RAMP_UP )
        {
            m_currentValue += m_velocity * elapsedTime;

            if( m_startUpTimer.expired() )
            {
                m_velocity += m_acceleration * elapsedTime;
                m_acceleration += m_impulse * elapsedTime;
            }
            else
            {
                m_velocity += m_acceleration * elapsedTime;
            }
        }
        // Linear bang up from the start
        else if( m_bangRange.m_bangType == LINEAR )
        {
            m_currentValue += m_velocity;

            if( m_startUpTimer.expired() )
                m_velocity += m_acceleration * elapsedTime;
        }
        // combination of ramp up and linear
        else if( m_bangRange.m_bangType == HYBRID )
        {
            m_currentValue += m_velocity;

            if( m_startUpTimer.expired() )
            {
                if( m_terminalVelocity > m_acceleration )
                {
                    m_velocity += m_acceleration * elapsedTime;
                    m_acceleration += m_impulse * elapsedTime;
                }
                else
                {
                    m_velocity += m_acceleration * elapsedTime;
                }
            }
            else
            {
                m_velocity += m_acceleration * elapsedTime;
            }
        }

        // Only update the meter if the value is different
        if( (int64_t)m_lastValue != (int64_t)m_currentValue )
        {
            m_lastValue = m_currentValue;

            // check if the bang up has finished
            if( m_currentValue > m_targetValue )
            {
                m_currentValue = m_targetValue;
                m_bangUp = false;

                // Prepare the stop script function if one exists
                m_pSprite->prepare( "stop" );
            }

            // Display the value in the meter
            displayValue();
        }
    }
}

/************************************************************************
*    DESC:  Display the value in the meter
************************************************************************/
void CUIMeter::displayValue()
{
    // Display the new value
    m_pSprite->getVisualComponent()->createFontString( boost::lexical_cast<std::string>((int64_t)m_currentValue ) );

    // Get the font size
    const CSize<float> & size = m_pSprite->getVisualComponent()->getSize();

    // Check if the font string size is greater then what is allowed
    if( size > m_maxFontStrSize )
    {
        const CSize<float> dif = m_maxFontStrSize / size;

        // Is the difference less then the last size change
        if( dif < m_bangScaleAdjustment )
        {
            m_bangScaleAdjustment = dif;

            CPoint<float> scale( m_pSprite->getScale() );
            if( dif.w < dif.h )
                scale.set(dif.w, (m_scaleType == AXIS ? 1.f : dif.w), 1.f);
            else
                scale.set((m_scaleType == AXIS ? 1.f : dif.h), dif.h, 1.f);

            m_pSprite->setScale( scale );
        }
    }
}

/************************************************************************
*    DESC:  Is the meter banging
************************************************************************/
bool CUIMeter::isBanging()
{
    return m_bangUp;
}

/************************************************************************
*    DESC:  Clear the meter
************************************************************************/
void CUIMeter::clearMeter()
{
    m_lastValue = m_currentValue = m_targetValue = 0;
    m_bangUp = false;

    if( !m_pSprite->prepare( "clear" ) )
        m_pSprite->getVisualComponent()->createFontString( boost::lexical_cast<std::string>((int64_t)m_currentValue ) );
}
