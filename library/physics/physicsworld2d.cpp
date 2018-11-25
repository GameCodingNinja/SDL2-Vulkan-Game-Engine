
/************************************************************************
*    FILE NAME:       physicsworld2d.cpp
*
*    DESCRIPTION:     Wrapper class for Box2D's b2World
************************************************************************/

// Physical component dependency
#include <physics/physicsworld2d.h>

// Game lib dependencies
#include <utilities/xmlParser.h>
#include <utilities/highresolutiontimer.h>
#include <utilities/mathfunc.h>
#include <utilities/exceptionhandling.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Standard lib dependencies
#include <cstring>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CPhysicsWorld2D::CPhysicsWorld2D() :
    m_world( b2Vec2(0,0) ),
    m_active(false),
    m_timer(0),
    m_stepTime(0),
    m_stepTimeSec(0),
    m_timeRatio(0),
    m_velStepCount(6),
    m_posStepCount(2)
{
    setFPS(30);
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CPhysicsWorld2D::~CPhysicsWorld2D()
{
}


/************************************************************************
*    DESC:  Load the physics world from XML node
************************************************************************/
void CPhysicsWorld2D::loadFromNode( const XMLNode & node )
{
    // Get the world's settings, if any are set
    XMLNode settingsNode = node.getChildNode( "settings" );
    if( !settingsNode.isEmpty() )
    {
        if( settingsNode.isAttributeSet( "active" ) )
            m_active = ( std::strcmp(settingsNode.getAttribute( "active" ), "true") == 0 );
    }

    // Get the world's gravity, if any are set
    XMLNode gravityNode = node.getChildNode( "gravity" );
    if( !gravityNode.isEmpty() )
    {
        b2Vec2 gravity;
        gravity.x = std::atof( gravityNode.getAttribute( "x" ) );
        gravity.y = std::atof( gravityNode.getAttribute( "y" ) );

        m_world.SetGravity( gravity );
    }

    // Get the stepping which determines how accurate the physics are
    XMLNode steppingNode = node.getChildNode( "stepping" );
    if( !steppingNode.isEmpty() )
    {
        m_velStepCount = std::atoi( steppingNode.getAttribute( "velocity" ) );
        m_posStepCount = std::atoi( steppingNode.getAttribute( "position" ) );

        float fps = std::atof( steppingNode.getAttribute( "fps" ) );

        // If the number is negative, get the current refresh rate
        if( fps < 0.f )
        {
            SDL_DisplayMode dm;
            SDL_GetDesktopDisplayMode(0, &dm);

            if( dm.refresh_rate == 0 )
                fps = 60.f;
            else
                fps = dm.refresh_rate;
        }

        setFPS( fps );
    }

    // Get the conversion of pixels per meter because Box2D works in meters
    XMLNode conversionNode = node.getChildNode( "conversion" );
    if( !conversionNode.isEmpty() )
    {
        m_pixelsPerMeter = std::atof( conversionNode.getAttribute( "pixelsPerMeter" ) );
    }
}


/************************************************************************
*    DESC:  Get the world
************************************************************************/
const b2World & CPhysicsWorld2D::getWorld() const
{
    return m_world;
}

b2World & CPhysicsWorld2D::getWorld()
{
    return m_world;
}


/************************************************************************
*    DESC:  Create a physics body
*           NOTE: Function should only be called from physics component
************************************************************************/
b2Body * CPhysicsWorld2D::createBody( const b2BodyDef & pDef )
{
    // NOTE: Class doesn't not own the data. Do Not Delete!
    b2Body * pTmpBody = m_world.CreateBody( &pDef );

    if( pTmpBody == nullptr )
        throw NExcept::CCriticalException("Physics Body Error!",
            boost::str( boost::format("Error creating physics body.\n\n%s\nLine: %s")
                % __FUNCTION__ % __LINE__ ));

    m_pBodySet.insert( pTmpBody );

    return pTmpBody;
}


/************************************************************************
*    DESC:  Destroy a physics body
*           NOTE: Function should only be called from physics component
************************************************************************/
void CPhysicsWorld2D::destroyBody( b2Body * pBody )
{
    auto iter = m_pBodySet.find( pBody );

    if( iter != m_pBodySet.end() )
    {
        // Destroy the body
        m_world.DestroyBody( pBody );

        // Remove the body from the set
        m_pBodySet.erase( iter );
    }
}


/************************************************************************
*    DESC:  Perform fixed time step physics simulation
************************************************************************/
void CPhysicsWorld2D::fixedTimeStep()
{
    if( m_active )
    {
        // Increment the timer
        m_timer += CHighResTimer::Instance().getElapsedTime();

        if( m_timer > m_stepTime )
        {
            m_timer = NMathFunc::Modulus( m_timer, m_stepTime );

            // Begin the physics world step
            m_world.Step( m_stepTimeSec, m_velStepCount, m_posStepCount );
        }

        m_timeRatio = m_timer / m_stepTime;
    }
}

/************************************************************************
*    DESC:  Perform variable time step physics simulation
************************************************************************/
void CPhysicsWorld2D::variableTimeStep()
{
    if( m_active )
    {
        // Begin the physics world step
        m_world.Step( CHighResTimer::Instance().getElapsedTime() / 1000.f, m_velStepCount, m_posStepCount );
    }
}


/************************************************************************
*    DESC:  Get the focus
*
*	 ret:	const CPointInt & - focus of the physics world
************************************************************************/
const CPoint<int> & CPhysicsWorld2D::getFocus() const
{
    return m_focus;
}


/************************************************************************
*    DESC:  Set the fps to run the simulation at
************************************************************************/
void CPhysicsWorld2D::setFPS( float fps )
{
    // Make sure we don't have a negative or zero fps
    if( fps > 1.f )
    {
        // Calculate the step paramaters
	m_stepTimeSec = 1.f / fps;
        m_stepTime = m_stepTimeSec * 1000.f;

        // Set the timer so that we'll begin a step next time we call Update
        m_timer = m_stepTime;
    }
}


/************************************************************************
*    DESC:  The the time ratio
************************************************************************/
float CPhysicsWorld2D::getTimeRatio() const
{
    return m_timeRatio;
}


/************************************************************************
*    DESC:  Set the activity of the physics world
************************************************************************/
void CPhysicsWorld2D::setActive( bool value )
{
    m_active = value;
}


/************************************************************************
*    DESC:  Is the physics world active
************************************************************************/
bool CPhysicsWorld2D::isActive() const
{
    return m_active;
}


/************************************************************************
*    DESC:  Get the pixels per meter
************************************************************************/
double CPhysicsWorld2D::getPixelsPerMeter() const
{
    return m_pixelsPerMeter;
}
