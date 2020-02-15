
/************************************************************************
 *    FILE NAME:       physicsworld3d.cpp
 *
 *    DESCRIPTION:     Wrapper class for Bullet physics
 ************************************************************************/

// Physical component dependency
#include <physics/physicsworld3d.h>

// Game lib dependencies
#include <utilities/xmlParser.h>
#include <utilities/highresolutiontimer.h>
#include <utilities/mathfunc.h>

// Standard lib dependencies
#include <cstring>

/************************************************************************
 *    DESC:  Constructor
 ************************************************************************/
CPhysicsWorld3D::CPhysicsWorld3D() :
m_colDisp( &m_defColConf ),
m_world( &m_colDisp, &m_broadphase, &m_conSolv, &m_defColConf ),
m_active(false),
m_timer(0),
m_stepTime(0),
m_stepTimeSec(0),
m_timeRatio(0)
{
    // Init with default values
    m_world.setGravity( btVector3(0.f, -10.f, 0.f) );
    setFPS(30);
}

/************************************************************************
 *    DESC:  destructor
 ************************************************************************/
CPhysicsWorld3D::~CPhysicsWorld3D()
{
}

/************************************************************************
 *    DESC:  Load the physics world from the passed in XML node
 ************************************************************************/
void CPhysicsWorld3D::loadFromNode( const XMLNode & node )
{
    // Get the world's settings, if any are set
    XMLNode settingsNode = node.getChildNode( "settings" );
    if( !settingsNode.isEmpty() )
    {
        if( settingsNode.isAttributeSet( "active" ) )
        {
            if( std::strcmp(settingsNode.getAttribute( "active" ), "true") == 0 )
                m_active = true;
        }
    }

    // Get the world's gravity, if any are set
    XMLNode gravityNode = node.getChildNode( "gravity" );
    if( !gravityNode.isEmpty() )
    {
        float x = std::atof( gravityNode.getAttribute( "x" ) );
        float y = std::atof( gravityNode.getAttribute( "y" ) );
        float z = std::atof( gravityNode.getAttribute( "z" ) );

        m_world.setGravity( btVector3(x, y, z) );
    }

    // Get the stepping which determins how accurate the physics are
    XMLNode steppingNode = node.getChildNode( "stepping" );
    if( !steppingNode.isEmpty() )
        setFPS( std::atof( steppingNode.getAttribute( "fps" ) ) );
}

/************************************************************************
 *    DESC:  Get the world
 ************************************************************************/
const btDiscreteDynamicsWorld & CPhysicsWorld3D::getWorld() const
{
    return m_world;
}

/************************************************************************
 *    DESC:  Add a rigid body to the world
 ************************************************************************/
void CPhysicsWorld3D::addRigidBody( btRigidBody * pBody )
{
    m_world.addRigidBody( pBody );
}

/************************************************************************
 *    DESC:  Add collision filtering when adding a rigid body
 ************************************************************************/
void CPhysicsWorld3D::addRigidBody( btRigidBody * pBody, short filterGroup, short mask )
{
    m_world.addRigidBody( pBody, filterGroup, mask );
}

/************************************************************************
 *    DESC:  Remove a rigid body to the world
 ************************************************************************/
void CPhysicsWorld3D::removeRigidBody( btRigidBody * pBody )
{
    m_world.removeRigidBody( pBody );
}

/************************************************************************
 *    DESC:  Perform fixed time step physics simulation
 ************************************************************************/
void CPhysicsWorld3D::fixedTimeStep()
{
    if( m_active )
    {
        // Increment the timer
        m_timer += CHighResTimer::Instance().getElapsedTime();

        if( m_timer > m_stepTime )
        {
            m_timer = NMathFunc::Modulus( m_timer, m_stepTime );

            // Begin the physics world step - same as m_stepTime / 1000.0f
            m_world.stepSimulation( m_stepTimeSec, 1, m_stepTimeSec );
        }

        m_timeRatio = m_timer / m_stepTime;
    }
}

/************************************************************************
 *    DESC:  Perform variable time step physics simulation
 ************************************************************************/
void CPhysicsWorld3D::variableTimeStep()
{
    if( m_active )
    {
        auto elapsedTime = CHighResTimer::Instance().getElapsedTime() / 1000.f;
        m_world.stepSimulation( elapsedTime, 1, elapsedTime );
    }
}

/************************************************************************
 *    DESC:  Set the fps to run the simulation at
 ************************************************************************/
void CPhysicsWorld3D::setFPS( float fps )
{
    // Make sure we don't have a negative or zero fps
    if( fps > 0.f )
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
float CPhysicsWorld3D::getTimeRatio() const
{
    return m_timeRatio;
}

/************************************************************************
 *    DESC:  Set the activity of the physics world
 ************************************************************************/
void CPhysicsWorld3D::setActive( bool value )
{
    m_active = value;
}

/************************************************************************
 *    DESC:  Is the physics world active
 ************************************************************************/
bool CPhysicsWorld3D::isActive() const
{
    return m_active;
}
