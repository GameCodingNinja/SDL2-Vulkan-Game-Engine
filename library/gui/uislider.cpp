
/************************************************************************
*    FILE NAME:       uislider.cpp
*
*    DESCRIPTION:     Class for user interface slider
************************************************************************/

// Physical component dependency
#include <gui/uislider.h>

// Game lib dependencies
#include <utilities/genfunc.h>
#include <utilities/xmlParser.h>
#include <utilities/settings.h>
#include <gui/menumanager.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Standard lib dependencies
#include <cstring>

// SDL lib dependencies
#include <SDL3/SDL.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CUISlider::CUISlider( const std::string & group ) :
    CUISubControl( group ),
    m_travelDistPixels(0),
    m_orientation(EOrientation::HORZ),
    m_minValue(0),
    m_maxValue(0),
    m_curValue(0),
    m_incValue(0),
    m_displayValueAsInt(false),
    m_sliderBtnHold(false),
    m_pressType( EActionPress::IDLE)
{
    m_type = EControlType::SLIDER;
}

/************************************************************************
*    DESC:  destructor
************************************************************************/
CUISlider::~CUISlider()
{
}

/************************************************************************
*    DESC:  Load the control info from XML node
************************************************************************/
void CUISlider::loadFromNode( const XMLNode & node )
{
    CUISubControl::loadFromNode( node );

    // Get the slider specific settings
    XMLNode settingsNode = node.getChildNode( "settings" );
    if( !settingsNode.isEmpty() )
    {
        if( settingsNode.isAttributeSet( "orientation" ) )
            if( std::strcmp(settingsNode.getAttribute( "orientation" ), "VERT") == 0 )
                m_orientation = EOrientation::VERT;

        if( settingsNode.isAttributeSet( "minValue" ) )
            m_minValue = atof(settingsNode.getAttribute( "minValue" ));

        if( settingsNode.isAttributeSet( "maxValue" ) )
            m_maxValue = atof(settingsNode.getAttribute( "maxValue" ));

        if( settingsNode.isAttributeSet( "incValue" ) )
            m_incValue = atof(settingsNode.getAttribute( "incValue" ));

        if( settingsNode.isAttributeSet( "defValue" ) )
            m_curValue = atof(settingsNode.getAttribute( "defValue" ));

        if( settingsNode.isAttributeSet( "displayValueAsInt" ) )
            if( std::strcmp(settingsNode.getAttribute( "displayValueAsInt" ), "true") == 0 )
                m_displayValueAsInt = true;
    }
}

/************************************************************************
*    DESC:  Load the control specific info from XML node
*
*    param: node - XML node
************************************************************************/
void CUISlider::loadControlFromNode( const XMLNode & node )
{
    // Have the parent load it's stuff
    CUISubControl::loadControlFromNode( node );

    // Get the position of the slider button as the default position
    m_defaultPos = getSubControl()->getPos();

    // Get the slider specific settings
    XMLNode settingsNode = node.getChildNode( "settings" );
    if( !settingsNode.isEmpty() )
    {
        if( settingsNode.isAttributeSet( "maxTravelDistPixels" ) )
            m_travelDistPixels = atof(settingsNode.getAttribute( "maxTravelDistPixels" ));
    }
}

/************************************************************************
*    DESC:  Init the control
************************************************************************/
void CUISlider::init()
{
    CUIControl::init();

    updateSlider();
}

/************************************************************************
*    DESC:  Handle OnLeftAction message
************************************************************************/
void CUISlider::onLeftAction( const SDL_Event & rEvent )
{
    // Handle the slider change
    if( rEvent.user.code == static_cast<int>(EActionPress::DOWN) )
        handleSliderChange( -m_incValue, true );
}

/************************************************************************
*    DESC:  Handle OnRightAction message
************************************************************************/
void CUISlider::onRightAction( const SDL_Event & rEvent )
{
    // Handle the slider change
    if( rEvent.user.code == static_cast<int>(EActionPress::DOWN) )
        handleSliderChange( m_incValue, true );
}

/************************************************************************
*    DESC:  Handle OnLeftScroll message
************************************************************************/
void CUISlider::onLeftScroll( const SDL_Event & rEvent )
{
    handleSliderChange( -m_incValue );
}

/************************************************************************
*    DESC:  Handle OnRightScroll message
************************************************************************/
void CUISlider::onRightScroll( const SDL_Event & rEvent )
{
    handleSliderChange( m_incValue );
}

/************************************************************************
*    DESC:  Handle OnMouseMove message
************************************************************************/
bool CUISlider::onMouseMove( const SDL_Event & rEvent )
{
    bool result = CUISubControl::onMouseMove( rEvent );

    if( isActive() && (m_pressType == EActionPress::DOWN) )
    {
        CMatrix finalMatrix( getMatrix() );
        finalMatrix.mergeMatrix( CMenuMgr::Instance().getCamera().getMatrix() );
        finalMatrix.scale( CSettings::Instance().getOrthoAspectRatioOrientation() );

        if( m_orientation == EOrientation::HORZ )
            incSliderMovePos( (float)rEvent.motion.xrel * (1.f / finalMatrix.getScale().x) );
        else
            incSliderMovePos( (float)rEvent.motion.yrel * (1.f / finalMatrix.getScale().y) );

        // Prepare script function associated with handling this game event
        prepareControlScriptFunction( EControlState::CHANGE );
    }

    return result;
}

/************************************************************************
*    DESC:  Handle the select action
************************************************************************/
bool CUISlider::handleSelectAction( const CSelectMsgCracker & msgCracker )
{
    bool result( isActive() && msgCracker.isDeviceMouse() && isPointInControl( msgCracker.getPos() ) );
    if( result && (msgCracker.getPressType() == getMouseSelectType()) )
    {
        // Get the press type to know if we need to move the slider
        // along with the mouse move
        m_pressType = msgCracker.getPressType();

        if( msgCracker.isPressDown() )
        {
            CMatrix finalMatrix( getMatrix() );
            finalMatrix.mergeMatrix( CMenuMgr::Instance().getCamera().getMatrix() );
            finalMatrix.scale( CSettings::Instance().getOrthoAspectRatioOrientation() );

            CPoint<float> dif = (msgCracker.getPos() - getSubControl()->getCollisionPos());

            // (1.0 / finalMatrix.getScale().#) handles the scaling of the control
            if( m_orientation == EOrientation::HORZ )
                incSliderMovePos( dif.x * (1.f / finalMatrix.getScale().x) );
            else
                incSliderMovePos( dif.y * (1.f / finalMatrix.getScale().y) );

            // Prepare script function associated with handling this game event
            prepareControlScriptFunction( EControlState::SELECT );
        }
    }
    else if( msgCracker.getPressType() != getMouseSelectType() )
    {
        m_pressType = EActionPress::IDLE;
    }

    return result;
}

/************************************************************************
*    DESC:  Deactivate the control
************************************************************************/
void CUISlider::deactivateControl()
{
    CUISubControl::deactivateControl();

    m_pressType = EActionPress::IDLE;
}

/************************************************************************
*    DESC:  Handle the slider change
************************************************************************/
void CUISlider::handleSliderChange( float value, bool prepareOnSelect )
{
    if( isActive() )
    {
        // Send a message to blink the button
        NGenFunc::DispatchEvent(
            NMenuEvent::CONTROL_STATE_CHANGE,
            static_cast<int>(EControlState::SELECT),
            getSubControl() );

        incSlider(value);

        // Prepare script function associated with handling this game event
        if( prepareOnSelect )
            prepareControlScriptFunction( EControlState::SELECT );
    }
}

/************************************************************************
*    DESC:  Set the slider min value
************************************************************************/
void CUISlider::setSliderMinValue( float value )
{
    m_minValue = value;
}

/************************************************************************
*    DESC:  Set the slider max value
************************************************************************/
void CUISlider::setSliderMaxValue( float value )
{
    m_maxValue = value;
}

/************************************************************************
*    DESC:  Set the slider inc value
************************************************************************/
void CUISlider::setSliderPosValue( float value )
{
    m_curValue = value;

    // Update the slider
    updateSlider();
}

/************************************************************************
*    DESC:  Cap the slider value
************************************************************************/
float CUISlider::getSliderPosValue()
{
    return m_curValue;
}

/************************************************************************
*    DESC:  Set the slider inc value
************************************************************************/
void CUISlider::incSlider( float value )
{
    m_curValue += value;

    // Update the slider
    updateSlider();
}

/************************************************************************
*    DESC:  Inc the slider based on mouse movement
************************************************************************/
float CUISlider::incSliderMovePos( float value )
{
    m_curValue += value * ((m_maxValue - m_minValue) / m_travelDistPixels);

    // Update the slider
    updateSlider();

    return m_curValue;
}

/************************************************************************
*    DESC:  Update the slider
************************************************************************/
void CUISlider::updateSlider()
{
    // Cap current value to it's range
    capSliderValue();

    // Set the position of the slider
    setSliderPos();

    // Set the slider label if there is one
    if( !m_stringVec.empty() )
    {
        // See if we specified a format string
        std::string formatStr = m_stringVec.back();

        // Format for display
        std::string valueStr;

        if( m_displayValueAsInt )
            valueStr = boost::str( boost::format(formatStr) % (int)m_curValue );
        else
            valueStr = boost::str( boost::format(formatStr) % m_curValue );

        createFontString( valueStr );
    }
}

/************************************************************************
*    DESC:  Cap the slider value
************************************************************************/
void CUISlider::capSliderValue()
{
    // Cap current value to range
    if( m_curValue < m_minValue )
        m_curValue = m_minValue;

    else if( m_curValue > m_maxValue )
        m_curValue = m_maxValue;
}

/************************************************************************
*    DESC:  Set the position of the slider
************************************************************************/
void CUISlider::setSliderPos()
{
    if( std::fabs(m_maxValue) > 0.001f )
    {
        float startPos = -(m_travelDistPixels / 2);
        float pixelsPerValue = m_travelDistPixels / (m_maxValue - m_minValue);
        float pos = startPos + (pixelsPerValue * (m_curValue - m_minValue));

        if( m_orientation == EOrientation::HORZ )
            getSubControl()->setPos( m_defaultPos + CPoint<float>(pos,0,0) );
        else
            getSubControl()->setPos( m_defaultPos + CPoint<float>(0,pos,0) );
    }
}

/************************************************************************
*    DESC:  Is the mouse down
************************************************************************/
bool CUISlider::isSliderMouseDown()
{
    return (m_pressType == EActionPress::DOWN);
}
