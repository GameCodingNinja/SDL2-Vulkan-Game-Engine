
/************************************************************************
*    FILE NAME:       objectphysicsdata3d.cpp
*
*    DESCRIPTION:     Class containing the 3D object's physics data
************************************************************************/

// Physical component dependency
#include <objectdata/objectphysicsdata3d.h>

// Game lib dependencies
#include <utilities/xmlparsehelper.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CObjectPhysicsData3D::CObjectPhysicsData3D() :
    m_planeNormal(0.f, 1.f, 0.f),
    m_mass(0.f),
    m_friction(0.5f),
    m_rollingFiction(0.f),
    m_restitution(0.f),
    m_linearDamping(0.f),
    m_angularDamping(0.f),
    m_collisionGroup(0),
    m_collisionMask(0)
{
}


/************************************************************************
*    DESC:  Destructor
************************************************************************/
CObjectPhysicsData3D::~CObjectPhysicsData3D()
{
}


/************************************************************************
*    DESC:  Load thes object data from node
************************************************************************/
void CObjectPhysicsData3D::loadFromNode( const XMLNode & objectNode )
{
    const XMLNode physicsNode = objectNode.getChildNode( "physics" );

    // Check if the object has any physics data
    if( !physicsNode.isEmpty() )
    {
        if( physicsNode.isAttributeSet( "world" ) )
            m_world = physicsNode.getAttribute( "world" );

        // The body of the physics sprite used for physics
        const XMLNode bodyNode = physicsNode.getChildNode( "body" );
        if( !bodyNode.isEmpty() )
        {
            // Get the body shape
            if( bodyNode.isAttributeSet( "shape" ) )
                m_bodyShape = bodyNode.getAttribute( "shape" );

            // The mass of the shape
            if( bodyNode.isAttributeSet("mass") )
                m_mass = std::atof( bodyNode.getAttribute( "mass" ) );

            // The damping is the constant decceleration of movement
            if( bodyNode.isAttributeSet( "linearDamping" ) )
                    m_linearDamping = std::atof( bodyNode.getAttribute( "linearDamping" ) );

            // The angular damping is the constant decceleration of rotation
            if( bodyNode.isAttributeSet( "angularDamping" ) )
                    m_angularDamping = std::atof( bodyNode.getAttribute( "angularDamping" ) );

            // The friction is how much drag this object has on another object
            if( bodyNode.isAttributeSet( "friction" ) )
                    m_friction = std::atof( bodyNode.getAttribute( "friction" ) );

            // The amount of friction when this object rolls
            if( bodyNode.isAttributeSet( "rollingFriction" ) )
                    m_rollingFiction = std::atof( bodyNode.getAttribute( "rollingFriction" ) );

            // The restitution is the percentage of velocity retained upon physics
            if( bodyNode.isAttributeSet( "restitution" ) )
                    m_restitution = std::atof( bodyNode.getAttribute( "restitution" ) );
        }

        // Get the plane normal
        const XMLNode filterNode = physicsNode.getChildNode( "collisionFilter" );
        if( !filterNode.isEmpty() )
        {
            m_collisionGroup = std::atoi( filterNode.getAttribute( "group" ) );
            m_collisionMask = std::atoi( filterNode.getAttribute( "mask" ) );
        }

        // Get the plane normal
        const XMLNode planeNormalNode = physicsNode.getChildNode( "planeNormal" );
        if( !planeNormalNode.isEmpty() )
            m_planeNormal = NParseHelper::LoadXYZ( planeNormalNode );
    }
}


/************************************************************************
*    DESC:  Get the name of the physics world
************************************************************************/
const std::string & CObjectPhysicsData3D::getWorld() const
{
    return m_world;
}


/************************************************************************
*    DESC:  Get the type of physics body
************************************************************************/
const std::string & CObjectPhysicsData3D::getBodyShape() const
{
    return m_bodyShape;
}


/************************************************************************
*    DESC:  Get the mass of physics body
************************************************************************/
float CObjectPhysicsData3D::getMass() const
{
    return m_mass;
}


/************************************************************************
*    DESC:  Get the friction
************************************************************************/
float CObjectPhysicsData3D::getFriction() const
{
    return m_friction;
}


/************************************************************************
*    DESC:  Get the rolling friction
************************************************************************/
float CObjectPhysicsData3D::getRollingFriction() const
{
    return m_rollingFiction;
}


/************************************************************************
*    DESC:  Get the linear damping
************************************************************************/
float CObjectPhysicsData3D::getLinearDamping() const
{
    return m_linearDamping;
}


/************************************************************************
*    DESC:  Get the angular damping
************************************************************************/
float CObjectPhysicsData3D::getAngularDamping() const
{
    return m_angularDamping;
}


/************************************************************************
*    DESC:  Get the restitution - the amount of bounce
************************************************************************/
float CObjectPhysicsData3D::getRestitution() const
{
    return m_restitution;
}


/************************************************************************
*    DESC:  Get the plane normal
************************************************************************/
const CPoint<float> & CObjectPhysicsData3D::getPlaneNormal() const
{
    return m_planeNormal;
}


/************************************************************************
*    DESC:  Specify if physics is active
************************************************************************/
bool CObjectPhysicsData3D::isActive() const
{
    return (!m_world.empty() && !m_bodyShape.empty());
}


/************************************************************************
*    DESC:  Get the collision filter
************************************************************************/
short CObjectPhysicsData3D::getCollisionGroup() const
{
    return m_collisionGroup;
}


/************************************************************************
*    DESC:  Get the collision mask
************************************************************************/
short CObjectPhysicsData3D::getCollisionMask() const
{
    return m_collisionMask;
}
