
/************************************************************************
*    FILE NAME:       spinprofile.cpp
*
*    DESCRIPTION:     Spin profile data
*                     Movement is in pixels per second.
*                     Time is in milliseconds.
************************************************************************/

// Physical component dependency
#include <slot/spinprofile.h>

// Game lib dependencies
#include <utilities/xmlParser.h>

// Boost lib dependencies

// Standard lib dependencies
#include <cstdlib>
#include <string>


/************************************************************************
*    DESC:  Constructor
************************************************************************/
CSpinProfile::CSpinProfile() :
    m_startDelay(0),
    m_accelation(.0004f),
    m_impulse(0.0),
    m_maxVelocity(2.5),
    m_maxVelocityTime(700),
    m_bounceDrag(0.0),
    m_bounceCorrection(0),
    m_timeOutDelay(0.0),
    m_decelerationRotationCount(1),
    m_safetyCheckDivisor(0)
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CSpinProfile::~CSpinProfile()
{
}


/************************************************************************
*    DESC:  Load thes reel strip data from node
************************************************************************/
void CSpinProfile::loadFromNode( const XMLNode & node )
{
    // Get the symbols per reel
    if( node.isAttributeSet( "startDelay" ) )
        m_startDelay = std::atoi(node.getAttribute( "startDelay" ));

    // Get the acceleration
    if( node.isAttributeSet( "accelation" ) )
        m_accelation = std::atof(node.getAttribute( "accelation" )) / 1000.f;

    // Get the acceleration
    if( node.isAttributeSet( "impulse" ) )
        m_impulse = std::atof(node.getAttribute( "impulse" )) / 1000.f;

    // Get the max velocity
    if( node.isAttributeSet( "maxVelocity" ) )
        m_maxVelocity = std::atof(node.getAttribute( "maxVelocity" )) / 1000.f;

    // Get the max speed time
    if( node.isAttributeSet( "maxVelocityTime" ) )
        m_maxVelocityTime = std::atoi(node.getAttribute( "maxVelocityTime" ));

    // Get the bounce drag
    if( node.isAttributeSet( "bounceDrag" ) )
        m_bounceDrag = std::atof(node.getAttribute( "bounceDrag" )) / 1000.f;

    // Get the bounce distance
    if( node.isAttributeSet( "bounceCorrection" ) )
        m_bounceCorrection = std::atoi(node.getAttribute( "bounceCorrection" ));

    // Get the time out delay
    if( node.isAttributeSet( "timeOutDelay" ) )
        m_timeOutDelay = std::atof(node.getAttribute( "timeOutDelay" ));


    // Get the time out delay
    if( node.isAttributeSet( "decelerationRotationCount" ) )
        m_decelerationRotationCount = std::atof(node.getAttribute( "decelerationRotationCount" ));

    // Get the time out delay
    if( node.isAttributeSet( "safetyCheckDivisor" ) )
        m_safetyCheckDivisor = std::atof(node.getAttribute( "safetyCheckDivisor" ));
}


/************************************************************************
*    DESC:  Get the start delay
************************************************************************/
int CSpinProfile::getStartDelay() const
{
    return m_startDelay;
}


/************************************************************************
*    DESC:  Get the acceleration
************************************************************************/
double CSpinProfile::getAccelation() const
{
    return m_accelation;
}

/************************************************************************
*    DESC:  Get the impulse
************************************************************************/
double CSpinProfile::getImpulse() const
{
    return m_impulse;
}


/************************************************************************
*    DESC:  Get the max velocity
************************************************************************/
double CSpinProfile::getMaxVelocity() const
{
    return m_maxVelocity;
}


/************************************************************************
*    DESC:  Get the max speed time
************************************************************************/
int CSpinProfile::getMaxVelocityTime() const
{
    return m_maxVelocityTime;
}


/************************************************************************
*    DESC:  Get the bounce drag
************************************************************************/
double CSpinProfile::getBounceDrag() const
{
    return m_bounceDrag;
}


/************************************************************************
*    DESC:  Get the bounce correction
************************************************************************/
int CSpinProfile::getBounceCorrection() const
{
    return m_bounceCorrection;
}


/************************************************************************
*    DESC:  Get the time out delay
************************************************************************/
double CSpinProfile::getTimeOutDelay() const
{
    return m_timeOutDelay;
}


/************************************************************************
*    DESC:  Get the deceleration rotation count
************************************************************************/
double CSpinProfile::getDecelerationRotationCount() const
{
    return m_decelerationRotationCount;
}


/************************************************************************
*    DESC:  Get the safety check divisor
************************************************************************/
double CSpinProfile::getSafetyCheckDivisor() const
{
    return m_safetyCheckDivisor;
}
